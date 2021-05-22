#include <math.h>
#include <thread>
#include <atomic>
#include <iostream>

#if defined(SYNCQUEUE)
#include "CQueueSync.h"
#else
#include "CQueue.h"
#endif

#define SIZE (1024 * 1024 * 8)

CQueue memory_to_trig;
CQueue trig_to_memory;

void memory_thread_func(float *a, int size) {

  for (int i = 0; i < size; i++) {
    float loaded = a[i];
    memory_to_trig.enq(loaded);
    float to_store = trig_to_memory.deq();
    a[i] = to_store;
  }
}

void trig_thread_func(int size) {

  for (int i = 0; i < size; i++) {
    float consume = memory_to_trig.deq();
    consume = cos(consume);
    trig_to_memory.enq(consume);    
  }
}

int main() {
  float *a = new float[SIZE];
  for (int i = 0; i < SIZE; i++) {
    a[i] = 0.5 + i;
  }
  /*float *b = new float[SIZE];
  for (int i = 0; i < SIZE; i++) {
    b[i] = a[i];
  }
  bool output_bool = false;
  for (int i = 0; i < SIZE; i++) {
    b[i] = cos(b[i]);
  }*/

  auto start = std::chrono::high_resolution_clock::now();
  int duration = 0.0;
  std::thread memory_thread(memory_thread_func,a,SIZE);
  std::thread trig_thread(trig_thread_func, SIZE);

  memory_thread.join();
  trig_thread.join();
  auto now = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count();
  std::cout << "Time: " << duration << "s" << std::endl;   

  //extra check
  /*for (int i = 0; i < SIZE; i++) {
    if(a[i] != b[i] && !output_bool){
        std::cout << "rip at i = " << i << " " << a[i] << " vs " << b[i] << std::endl;
        output_bool = true;
    }
  }
  if(!output_bool)std::cout << "correct order" << std::endl;*/
  
  return 0;
}
