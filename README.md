# Merge path algorithm with CUDA
Implementation of a recent merge sort parallelized algorithm, **merge path** from [O. Green et al., 201](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.307.6456&rep=rep1&type=pdf).

## Description

  - 
  - 
  - 

## Compiling code
You need to compile `merge_sort_path.cu` with [cuRAND](https://docs.nvidia.com/cuda/curand/index.html) library :

```console
nvcc merge_sort_path.cu -lcurand -std=c++11 -o merge_sort_path
```
The other `.cpp` programs only need `-std=c++11` to be compiled.
