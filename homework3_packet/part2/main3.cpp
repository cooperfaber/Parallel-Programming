#include <thread>
#include <atomic>
#include <iostream>
#include "util.h"
#include "IOQueue.h"

IOQueue Q[NUM_THREADS];
std::atomic_int finished_threads(0);
void parallel_enq(int size, int tid, int num_threads) {
  // Use this function to equeue indexes to each of the local
  // workstealing queues
    int chunk_size = size/num_threads;
    Q[tid].init(chunk_size);
    int start = chunk_size*tid;
    int end = chunk_size + start;
    for(int i = start; i < end; i++){
          Q[tid].enq(i);
    }
}



void parallel_mult(float * result, int *mult, int size, int tid, int num_threads) {
  // Implement me using a workstealing approach.
  // You should use the Q IOQueues to compute
  // local work and then try to steal work
  // from others until all the work is completed
  int task = 0;
  for(int i = Q[tid].deq(); i != -1; i = Q[tid].deq()){
    float base = result[i];
    for (int w = 0; w < mult[i]-1; w++) {
        result[i] = result[i]+base;
    }
  }
  atomic_fetch_add(&finished_threads,1);
  int target = 0;
  //queue complete, good work
  //let's do some work for the others
  //the threads have been reading marx
  while(finished_threads.load() != num_threads){
        //loop through all other possible queues
        for(int i = 0; i < num_threads; i++){
            //target acquired
            target = i;
            //check if they're done
            task = Q[target].deq();
            if(task != -1){
                //needs of the many
                //same loop as before the while, just iterating through the queue until it's empty
                for(int i = task; i != -1; i = Q[tid].deq()){
                    float base = result[i];
                    for (int w = 0; w < mult[i]-1; w++) {
                        result[i] = result[i]+base;
                    }
                }
            }
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

  auto start = std::chrono::high_resolution_clock::now();
  int duration = 0.0;

  // Launch NUM_THREADS threads to intialize the IOQueues in Q with
  // the indexes for each thread. Remember to initialize the Queues
  // with the size that they will need. Join the threads afterwards.
  for(int i = 0; i < NUM_THREADS; i++){
      threads[i] = std::thread(parallel_enq,SIZE,i,NUM_THREADS);
  }
  for(int i = 0; i < NUM_THREADS; i++){
      threads[i].join();
  }
  // Next, launch the parallel function that performs the parallel_mult
  // function from main1.cpp and main2.cpp but using workstealing
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
