#include <iostream>
#include <random>
#include <fstream>
#include <ctime>
#include <vector>
using namespace std;

const int d = 1024; // Size of arrays generated
const int n = 10; // Number of arrays generated

// Generate one random sorted array
void generate_sorted_array(vector<int> &A,mt19937 &G){
  uniform_int_distribution<> U(0,5);
  A[0] = U(G);
  for (int i = 1; i < d; i++){
    A[i] = A[i-1] + U(G);
  }
}

//Generate multiple random sorted arrays using last function
void generate_arrays(vector<vector<int> > &M,mt19937 &G){
  for (int j = 0; j<n; j++){
    generate_sorted_array(M[j],G);
  }
}

//Sequential merge path
void merge_array(vector<int> & M, vector<int> &A, vector<int> &B){
  int i = 0;
  int j = 0;
  vector<int> m(A.size() + B.size());

  while(i+j < m.size()){
    if (i >= A.size()){
      m[i+j] = B[j];
      j++;
    }
    else if (j>=B.size() or A[i] < B[j]){
      m[i+j] = A[i];
      i++;
    }
    else {
      m[i+j] = B[j];
      j++;
    }
  }
  M = m;
}

//Scaling sequential merge path procedure
void merge_arrays(vector< vector<int> > & M){
  while (M.size() != 1){
    for (int i = 0; i < M.size()/2; i++){
      merge_array(M[i],M[2*i],M[2*i+1]);
    }
    if(M.size()%2 == 1){
      M[M.size()/2] = M[M.size()-1];
    }
    M.resize(M.size()/2 + M.size()%2);
  }
}

//Sanity checking of array length and sort
void sanity_check(vector< vector<int> > & M){
  bool sorted = true;
  cout << "*** Expected final length : " << n << " x " << n << " = " << d*n << endl;
  if(M[0].size() == d*n){
    cout << "*** Actual length : " << M[0].size() << " --> OK" << endl;
  }
  else{
    cout << "*** Actual length : " << M[0].size() << " --> ERROR" << endl;
  }
  for (int i = 1; i < M[0].size(); i++){
      if(M[0][i]<M[0][i-1]){
        sorted = false;
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

  // Memory space alocated for array generation
  vector<vector<int> > M(n);
  for (int i = 0; i < n; ++i){
    M[i].resize(d);
  }
  mt19937 G; // RNG
  clock_t timer; // Timer for CPU


  //Generating arrays
  cout << "---- GENERATING RANDOM SORTED ARRAYS ----" << endl;
  timer = clock();
  generate_arrays(M,G);
  timer = clock() - timer;
  cout << "*** Size of arrays created : " << d << endl;
  cout << "*** Number of arrays created : " << n << endl;
  cout << "*** Execution time : " << (float)timer/CLOCKS_PER_SEC << " sec" << endl;

  cout << "\n---- MERGING SORTED ARRAYS BY SEQUENTIAL MERGE PATH ----" << endl;
  timer = clock();
  merge_arrays(M);
  timer = clock() - timer;
  cout << "*** Execution time : " << (float)timer/CLOCKS_PER_SEC << " sec" << endl;

  cout << "\n---- SANITY CHECK ----" << endl;
  timer = clock();
  sanity_check(M);
  timer = clock() - timer;
  cout << "*** Execution time : " << (float)timer/CLOCKS_PER_SEC << " sec" << endl;

}
