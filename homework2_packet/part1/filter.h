#pragma once

#include <atomic>
using namespace std;

class mutex {
 public:
  mutex() {
  }
  
  void init (int num_threads) {
    level = new atomic_int[num_threads];
    victim = new atomic_int[num_threads];
    threads = num_threads;
    for(int i = 0; i < num_threads; i++){
        level[i].store(0);
        victim[i].store(-1);
    }
  }
  
  void lock(int thread_id) {
      for(int i = 1; i < threads; i++){
            level[thread_id].store(i);
            victim[i].store(thread_id);
            //spin loop
            for(int j = 0; j < threads; j++){
                //for each level, see if there are threads above
                if (j != thread_id)while(level[j].load() >= i && victim[i].load() == thread_id){
                       // std::this_thread::yield();
                }
            }
        }
  }
  
  void unlock(int thread_id) {
      level[thread_id].store(0);
  }

 private:
    atomic_int* victim;
    atomic_int* level;

    //implemented this as sizeof(flag) was giving 8 every time, and it's needed for the lock() loops
    atomic_int threads;
};
