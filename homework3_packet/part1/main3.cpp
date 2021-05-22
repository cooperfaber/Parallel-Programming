#include <math.h>
#include <thread>
#include <atomic>
#include <iostream>
#include "CQueue.h"

#define SIZE (1024 * 1024 * 8)

CQueue memory_to_trig;
CQueue trig_to_memory;

void memory_thread_func(float *a, int size) {

  for (int i = 0; i < size; i+=8) {
    float* loaded = new float[8];
    //load 8 values
    for(int j = 0; j < 8; j++){
        loaded[j] = a[i+j];
    }
    //enqueue 8 values
    for(int j = 0; j < 8; j++){
        memory_to_trig.enq(loaded[j]);
    }
    float* to_store = new float[8];
    //dequeue 8 complete values
    for(int j = 0; j < 8; j++){
        to_store[j] = trig_to_memory.deq();
    }
    //store 8 values
    for(int j = 0; j < 8; j++){
        a[i+j] = to_store[j];
    }
  }
}

void trig_thread_func(int size) {
  for (int i = 0; i < size; i+=8) {
    float consume[8];
    //load 8 values to trig upon
    for(int j = 0; j < 8; j++){
        consume[j] = memory_to_trig.deq();
    }
    //cosigned
    for(int j = 0; j < 8; j++){
        consume[j] = cos(consume[j]);
    }
    //send back 8 trigged values
    for(int j = 0; j < 8; j++){
        trig_to_memory.enq(consume[j]); 
    }
  }
}

int main() {
  float *a = new float[SIZE];
  for (int i = 0; i < SIZE; i++) {
    a[i] = 0.5 + i;
  }
  auto start = std::chrono::high_resolution_clock::now();
  int duration = 0.0;
  std::thread memory_thread(memory_thread_func,a,SIZE);
  std::thread trig_thread(trig_thread_func, SIZE);

  memory_thread.join();
  trig_thread.join();
  auto now = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
  std::cout << "Time: " << duration << "s" << std::endl; 

  
  return 0;
}
