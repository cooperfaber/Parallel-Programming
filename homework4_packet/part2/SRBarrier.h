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
    localSense = new bool[num_threads];
    for(int i = 0; i < num_threads; i++){
        localSense[i] = false;
    }
    barrSense.store(true);
  }

  void barrier(int tid) {
    int position = std::atomic_fetch_sub(&count,1);
    if(position == 1){
        count.store(size);
        barrSense.store(localSense[tid]);
    }
    else{
        while(localSense[tid] != barrSense.load()){
            //https://youtu.be/PGNiXGX2nLU?t=64
        }
    }
    localSense[tid] = !localSense[tid];
  }

private:
  // Give me some private variables
    std::atomic_int count;
    int size;
    bool* localSense;
    std::atomic_bool barrSense;
};
