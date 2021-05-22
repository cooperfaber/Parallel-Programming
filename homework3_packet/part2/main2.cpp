#include <thread>
#include <atomic>
#include <iostream>
#include "util.h"

std::atomic_int counter(0);
void parallel_mult(float * result, int *mult, int size, int tid, int num_threads) {
  // Write me a parallel way using the global counter
  // as an implicit worklist. That is, every thread
  // should get its index from incrementing the counter
  for (int i = atomic_fetch_add(&counter,1); i < size; i = atomic_fetch_add(&counter,1)) {
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

  // Launch NUM_THREADS threads and then join them
  auto start = std::chrono::high_resolution_clock::now();
  int duration = 0.0;
  for(int i = 0; i < NUM_THREADS; i++){
      threads[i] = std::thread(parallel_mult, result_parallel, mult, SIZE, i,NUM_THREADS);
  }
  for(int i = 0; i < NUM_THREADS; i++){
      threads[i].join();
  }
  auto now = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
  std::cout << "Time: " << duration << "s" << std::endl;   
}
