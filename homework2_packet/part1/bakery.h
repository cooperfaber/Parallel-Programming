#pragma once

#include <atomic>
#include <algorithm>
using namespace std;

class mutex {
 public:
  mutex() {
  }
  
  void init (int num_threads) {
    flag = new atomic_bool[num_threads];
    label = new atomic_int[num_threads];
    threads.store(num_threads);
    for(int i = 0; i < num_threads; i++){
        flag[i] = false;
        label[i].store(0);
    }
  }
  
  void lock(int thread_id) {

    flag[thread_id].store(true);
    label[thread_id].store(getMax());
    //spin loop
    for(int j = 0; j < threads; ++j){
        //for each level, see if there threads waiting with lower numbers
        //make sure you don't wait for yourself
        if (j != thread_id){
            while((flag[j].load()) && ((label[j].load() < label[thread_id].load())||(label[thread_id].load() == label[j].load() && j < thread_id))){
                //if your label is a lower number, i'll wait
                //if your label is the same as mine, but you got here first, i'll wait
                //do i have to wait?
                //std::this_thread::yield();
            }
        } 
    }
  }
  
  void unlock(int thread_id) {
      flag[thread_id].store(false);
  }

  int getMax(){
    int max = 0;
    for(int i = 0; i < threads; i++){
        int temp = label[i].load();
        if (temp > max) max = temp;
    }
    return max+1;
  }

 private:
    atomic_int* label;
    atomic_bool* flag;

    //implemented this as sizeof(flag) was giving 8 every time, and it's needed for the lock() loops
    atomic_int threads;

};