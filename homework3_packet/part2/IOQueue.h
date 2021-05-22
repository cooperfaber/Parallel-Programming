#include <atomic>

class IOQueue {
 public:
  IOQueue() {
  }

  // Use this function to initialize the queue to
  // the size that you need.
  void init(int size) {
      buffer = new int[size];
      head.store(0);
      tail.store(0);
  }

  // enqueue the element e into the queue
  void enq(int e) {
      buffer[atomic_fetch_add(&head,1)] = e;
  }

  // return a value from the queue.
  // return -1 if there is no more values
  // from the queue.
  int deq() {
      if( head.load() == tail.load() ){
          //empty
          return -1;
      }
      else{
          return buffer[atomic_fetch_add(&tail,1)];
      }
  }

  // return 32 values from the queue. The
  // values should be returned through
  // the array passed in.
  // return 0 if successful. return -1 if not
  // i.e. if the queue does not have 32 elements
  int deq_32(int ret[32]) {
      if(head.load()-tail.load() < 32){
        return -1;
      }
      else{
          for(int i = 0; i < 32; i++){
              ret[i] = buffer[atomic_fetch_add(&tail,1)];
          }
          //success
          return 0;
      }
  }

 private:
    int* buffer;
    std::atomic_int head;
    std::atomic_int tail;
  // Give me some private variables
};
