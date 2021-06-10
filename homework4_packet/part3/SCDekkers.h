#include <atomic>

// this should be the same as SCDekkers with the change that every
// relaxed access should be memory_order_relaxed

class dekkers_mutex {
public:
  dekkers_mutex() {
    wants_to_enter = new std::atomic_bool[2];
    turn.store(0);  
    for(int i = 0; i < 2; i++){
        wants_to_enter[i].store(false);
    }
  }

  void lock(int tid) {
    //us & them
    //after all, we're only ordinary men
    int them = 0;
    if(tid == 0)them = 1;
    wants_to_enter[tid] = true;
    while(wants_to_enter[them].load(std::memory_order_seq_cst)){
        if(turn.load(std::memory_order_seq_cst) != tid){
            wants_to_enter[tid].store(false);
            while(turn.load(std::memory_order_seq_cst) != tid){
                //busy wait
            }
            wants_to_enter[tid].store(true);
        }
    }
  }

  void unlock(int tid) {
    int them = 0;
    if(tid == 0)them = 1;
    turn.store(them);
    wants_to_enter[tid].store(false);
  }

private:
  std::atomic_bool* wants_to_enter;
  std::atomic_int turn;
  // Give me some private variables  
};
