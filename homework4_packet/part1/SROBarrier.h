#include <atomic>
#include <iostream>

class barrier_object {
 public:  
  barrier_object() {
    // Probably don't need to do anything here.
  }

  void init(int num_threads) {
    count.store(num_threads);
    size = num_threads;
    localSense = new std::atomic_bool[num_threads];
    for(int i = 0; i < num_threads; i++){
        localSense[i].store(false);
    }
    barrSense.store(true);
    flag = false;
  }

  void barrier(int tid) {
    int position = std::atomic_fetch_sub(&count,1);
    if(position == 1){
        count.store(size);
        barrSense.store(localSense[tid].load(std::memory_order_relaxed));
    }
    else{
        while(localSense[tid] != barrSense.load(std::memory_order_relaxed)){
            std::this_thread::yield();
        }
    }
    //swap vars and release
    bool barr = barrSense.load();
    std::atomic_compare_exchange_strong(&localSense[tid], &barr, !localSense[tid]);
  }
    //bool reverse = localSense[tid].load();
    //localSense[tid].store(!reverse); 

  

private:
  // Give me some private variables
    std::atomic_int count;
    int size;
    std::atomic_bool* localSense;
    std::atomic_bool barrSense;
    std::atomic_bool flag;
};
