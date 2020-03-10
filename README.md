# Merge path algorithm with CUDA
Implementation of a recent merge sort parallelized algorithm, **merge path** from [O. Green et al., 2014(http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.307.6456&rep=rep1&type=pdf).

## Description

The goal of all these programs is to merge multiple arrays. For simplicity, the number of array and their length is a power of 2. The last program is parallelized and also sort before merging the arrays. Therefore, it performs a parallelized **mergesort** algorithm.

  - **`sequential_merge_path.cpp`** : classical merge algorithm running on CPU
  - **`merge_path.cpp`** : merge path algorithm from O. Green et al. running on CPU.
  - **`merge_sort_path.cu`** : parallelized mergesort algorithm. It starts to sort a big array, then uses a *divide and conquer* method to apply merge path from O Green et al. on GPU.

## Compiling code
You need to compile `merge_sort_path.cu` with [cuRAND](https://docs.nvidia.com/cuda/curand/index.html) library :

```console
nvcc merge_sort_path.cu -lcurand -std=c++11 -o merge_sort_path
```
The other `.cpp` programs only need `-std=c++11` to be compiled.
