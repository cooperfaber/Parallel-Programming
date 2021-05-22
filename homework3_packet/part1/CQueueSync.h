#include <atomic>

class CQueue {
 public:

  CQueue() {
    box.store(0);
    flag.store(false);
  }

  void enq(float e) {
    box.store(e);
    flag.store(true);
    while(flag.load()){
        //you spin me right round
    }
  }

  float deq() {
    // Implement me!
    float retval = 0;
    while(!flag.load()){
        //like a record baby, right round right round
    }
    //take value before setting flag
    retval = box.load();
    flag.store(false);
    return retval;
  }
  
 private:
  // Give me some private variables
    std::atomic<float> box;
    std::atomic_bool flag;
};
