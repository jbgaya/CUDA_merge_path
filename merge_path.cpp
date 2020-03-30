#include <iostream>
#include <random>
#include <fstream>
#include <ctime>
#include <vector>
using namespace std;

const int d = 2; // Size of arrays generated
const int n = 1048576; // Number of arrays generated

// Generate one random sorted array
void generate_sorted_array(vector<int> &A,mt19937 &G){
  uniform_int_distribution<> U(0,100);
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

//Merge path
void merge_array(vector<int> & M, vector<int> &A, vector<int> &B){
  int size_A = A.size();
  int size_B = B.size();
  int size_m = size_A + size_B;
  int offset;
  vector<int> m(size_m);
  M.resize(size_m);

  for(int i=0; i<m.size(); i++){
    vector<int> K(2),P(2),Q(2);
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
    if (Q[1] >= 0 && Q[0] <= size_B && (Q[1] == size_A || Q[0] == 0 || A[Q[1]]>B[Q[0]-1])){
      if(Q[0] == size_B || Q[1] == 0 || A[Q[1]-1] <= B[Q[0]]){
        if(Q[1] < size_A && (Q[0] == size_B || A[Q[1]] <= B[Q[0]])){
        m[i] = A[Q[1]];
        }
        else{
          m[i] = B[Q[0]];
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
  M = m;
}

//Scaling merge path procedure
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

  cout << "\n---- MERGING SORTED ARRAYS BY MERGE PATH ----" << endl;
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
