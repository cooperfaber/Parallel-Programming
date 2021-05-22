#include <thread>
#include <iostream>
#include "util.h"

void parallel_mult(float * result, int *mult, int size, int tid, int num_threads) {
  // write me in a SPMD parallel way!  partition the data using
  // "chunks" rather than strides.  That is thread 0 computes elements
  // 0 through size/num_threads.  thread 1 computes elements
  // size/num_threads through 2*(size/num_threads)
    int chunk_size = size/num_threads;
    int start = chunk_size*tid;
    int end = chunk_size + start;
    for (int i = start; i < end; i++) {
        float base = result[i];
        for (int w = 0; w < mult[i]-1; w++) {
            result[i] = result[i]+base;
        }
    }
}

int main() {
  std::thread threads[NUM_THREADS];
  float* result_parallel = new float[SIZE];
  int* mult = new int[SIZE];
  for (int i = 0; i < SIZE; i++) {
    result_parallel[i] = i;
  }
  linear_work(mult, SIZE);

  //timing
  auto start = std::chrono::high_resolution_clock::now();
  int duration = 0.0;
  // Launch NUM_THREADS threads and then join them
  for(int i = 0; i < NUM_THREADS; i++){
      threads[i] = std::thread(parallel_mult, result_parallel,mult,SIZE,i,NUM_THREADS);
  }
  
  for(int i = 0; i < NUM_THREADS; i++){
      threads[i].join();
  }

  auto now = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
  std::cout << "Time: " << duration << "s" << std::endl; 
}
