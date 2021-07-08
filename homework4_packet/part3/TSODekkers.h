#include <atomic>

// this should be the same as RDekkers with the change that every
// relaxed access should be memory_order_relaxed.
// your job is to use the following MACRO to fix the mutex.
// Think about the reorderings allowed under TSO and where the fence
// needs to be placed.

#define FENCE asm volatile("mfence":::"memory");

class dekkers_mutex {
public:
  dekkers_mutex() {
    wants_to_enter = new std::atomic_bool[2];
    turn.store(0);  
    for(int i = 0; i < 2; i++){
        wants_to_enter[i].store(false,std::memory_order_relaxed);
    }
    FENCE;
  }

  void lock(int tid) {
    //us & them
    //after all, we're only ordinary men
    int them = 0;
    if(tid == 0)them = 1;
    wants_to_enter[tid].store(true,std::memory_order_relaxed);
    FENCE;
    while(wants_to_enter[them].load(std::memory_order_relaxed)){
        if(turn.load(std::memory_order_relaxed) != tid){
            wants_to_enter[tid].store(false,std::memory_order_relaxed);
            FENCE;
            while(turn.load(std::memory_order_relaxed) != tid){
                //busy wait
            }
            wants_to_enter[tid].store(true,std::memory_order_relaxed);
            FENCE;
        }
    }
  }

  void unlock(int tid) {
    int them = 0;
    if(tid == 0)them = 1;
    turn.store(them,std::memory_order_relaxed);
    FENCE;
    wants_to_enter[tid].store(false,std::memory_order_relaxed);
    FENCE;
  }

private:
  std::atomic_bool* wants_to_enter;
  std::atomic_int turn;
  // Give me some private variables  
};
