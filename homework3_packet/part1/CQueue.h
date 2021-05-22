#include <atomic>
#include <iostream>

#define CQUEUE_SIZE 2048

class CQueue {
 public:

  CQueue() {
    head.store(0);
    tail.store(0);
    buffer = new float[CQUEUE_SIZE];
  }

  void enq(float e) {      
      while((tail.load() == head.load()+1) || size() >= CQUEUE_SIZE-1){
          //queue full, wait
      }
      buffer[head.load()] = e;
      head.store((head.load()+1)%CQUEUE_SIZE);
      //if we go too high, reset to 0
  }

  void enq_8(float e[8]) {
        while(tail.load() == head.load()+8){
            //queue full, wait
        }
      for(int i = 0; i < 8; i++){
        buffer[head.load()] = e[i];
        head.store((head.load()+1)%CQUEUE_SIZE);
      }
  }


  float deq() {
    while((size() == 0) || head.load() == tail.load()){
        //empty queue, wait
    }
    float retval = buffer[tail.load()];
    tail.store((tail.load()+1)%CQUEUE_SIZE);
    return retval;
  }

  void deq_8(float e[8]) {
    while((size() < 8)){
        //empty queue, wait
    }
    for(int i = 0; i < 8; i++){
        e[i] = buffer[tail.load()];
        tail.store((tail.load()+1)%CQUEUE_SIZE);
    }
  }


  int size() {
    int diff = abs(head.load()-tail.load());
    return diff;
  }

  
 private:
    std::atomic_int head;
    std::atomic_int tail;
    float* buffer;
};
