#include <iostream>
#include <ctime>
#include <curand.h>
using namespace std;

#define n 262144 // Size of array M (should be a power of 2)

// Function that catches the error
void testCUDA(cudaError_t error, const char *file, int line)  {
	if (error != cudaSuccess) {
	   printf("There is an error in file %s at line %d\n", file, line);
       exit(EXIT_FAILURE);
	}
}
#define testCUDA(error) (testCUDA(error, __FILE__ , __LINE__))

//Sort initialization
__global__
void sort_array(unsigned int *M){
	int index = blockIdx.x * 2;
	if(M[index] > M[index+1]){
		int swap = M[index];
		M[index] = M[index+1];
		M[index+1] = swap;
	}
}

//Merge path
__global__
void merge_array(unsigned int *m,unsigned int *M,int array_size){
  int index_array = blockIdx.x * array_size;
	int stride = blockDim.x;
	for(int i=threadIdx.x;i<array_size;i+=stride){

	  int size_A = array_size / 2;
	  int size_B = array_size / 2;
	  int offset;

	  int K[2],P[2],Q[2];
	  if( i> size_A ){
	      K[0] = P[1] = i - size_A;
	      K[1] = P[0] = size_A;
	    }
	    else{
	      K[0] = P[1] = 0;
	      K[1] = P[0] = i;
	    }
	    while(true){
	        offset = abs(K[1]-P[1])/2;
	        Q[0] = K[0] + offset;
	        Q[1] = K[1] - offset;
	    if (Q[1] >= 0 && Q[0] <= size_B && (Q[1] == size_A || Q[0] == 0 || M[index_array+Q[1]]>M[index_array+size_A+Q[0]-1])){
	      if(Q[0] == size_B || Q[1] == 0 || M[index_array+Q[1]-1] <= M[index_array+size_A+Q[0]]){
	        if(Q[1] < size_A && (Q[0] == size_B || M[index_array+Q[1]] <= M[index_array+size_A+Q[0]])){
	        m[index_array+i] = M[index_array+Q[1]];
	        }
	        else{
	          m[index_array+i] = M[index_array+size_A+Q[0]];
	        }
	        break;
	      }
	      else {
	        K[0] = Q[0] + 1;
	        K[1] = Q[1] - 1;
	      }
	    }
	    else{
	      P[0] = Q[0] - 1;
	      P[1] = Q[1] + 1;
	    }
		}
	}
	__syncthreads();
}

//Function to Sanity check if the array is sorted or not (CPU)
void sanity_check(unsigned int *M){
  bool sorted = true;
  for (int i = 1; i < n; i++){
      if(M[i]<M[i-1]){
        sorted = false;
				break;
      }
    }
  if(sorted==true){
    cout << "*** Is array sorted : True " << endl;
  }
  else{
      cout << "*** Is array sorted : False " << endl;
  }
}

int main () {


// *** Defining variables ***
  unsigned int *M_dev, *M_dev_next;	//Array and its copy to scale merge path procedure
	curandGenerator_t G; 							//Curand RNG
	clock_t timer; 										//Timer for CPU
  cudaEvent_t start,stop;						//Timer for GPU
	float elapsedTime;								//Timer for GPU


// *** Allocating memory ***
	cudaEventCreate(&start);
	cudaEventCreate(&stop);
  testCUDA(cudaMallocManaged(&M_dev, n*sizeof(unsigned int)));
	curandCreateGenerator(&G,CURAND_RNG_PSEUDO_DEFAULT);
	testCUDA(cudaMallocManaged(&M_dev_next, n*sizeof(unsigned int)));


// *** Performing custom random array generation ***
  cout << "---- GENERATING RANDOM ARRAY WITH CURAND ----" << endl;
  cudaEventRecord(start,0);
	curandGenerate(G, M_dev, n); //Generate n pseudo random unsigned int on device
  cudaEventRecord(stop,0);
  cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime,start,stop);
  cout << "*** Length of array created : " << n << endl;
  cout << "*** Execution time : " << elapsedTime << " ms" << endl;


// *** Performing sort initialization ***
  cout << "\n---- SORTING ARRAYS OF LENGTH 2 TO INITIALIZE MERGE PATH ----" << endl;
  cudaEventRecord(start,0);
	sort_array<<<n/2,1>>>(M_dev);
	cudaEventRecord(stop,0);
  cudaEventSynchronize(stop);
	cudaEventElapsedTime(&elapsedTime,start,stop);
	cout << "*** Execution time : " << elapsedTime << " ms" << endl;


// *** Performing merge path***
	cout << "\n---- MERGING SORTED ARRAYS BY MERGE PATH ----" << endl;
	cudaEventRecord(start,0);
  int nb_arrays = n/2;
  int array_size = 2;
  while (nb_arrays!=1){
		nb_arrays /= 2; //nb of array to merge on next merge_path
    array_size *= 2; //size of arrays on next merge_path
    merge_array<<<nb_arrays,min(array_size,1024)>>>(M_dev_next,M_dev,array_size);
		M_dev = M_dev_next;
		testCUDA(cudaMallocManaged(&M_dev_next, n*sizeof(unsigned int)));
  }
  cudaEventRecord(stop,0);
  cudaEventSynchronize(stop);
  cudaEventElapsedTime(&elapsedTime,start,stop);
  cout << "*** Execution time : " << elapsedTime << " ms" << endl;


//*** Sanity check ***
  unsigned int *M_host = (unsigned int*)malloc(n*sizeof(unsigned int));
  testCUDA(cudaMemcpy(M_host, M_dev, n*sizeof(unsigned int), cudaMemcpyDeviceToHost));

	/*
	//Code to vizualize array sorted
  for(int i=0;i<n;i++){
      cout <<  M_host[i] << "\t";
		}
	*/

	cout << "\n---- SANITY CHECK ----" << endl;
	timer = clock();
	sanity_check(M_host);
	timer = clock() - timer;
	cout << "*** Execution time : " << (float)timer/(CLOCKS_PER_SEC/1000) << " ms" << endl;


//*** free memory on device and host ***
	free(M_host);
  cudaEventDestroy(start);
	cudaEventDestroy(stop);
	cudaFree(M_dev);
	cudaFree(M_dev_next);


  return EXIT_SUCCESS;
}
