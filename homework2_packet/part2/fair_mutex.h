#pragma once

#include <atomic>
#include <mutex>
using namespace std;

class rw_mutex {
 public:
  rw_mutex() {
      writer.store(false);
      num_readers.store(0);
      writer_waiting.store(false);
      writer_cap.store(0);
      reader_cap.store(0);
    // Implement me!
  }

  void lock_reader() {
    bool acquired = false;
    while (!acquired) {
        internal_mutex.lock();
        //either it's the writers turn, or there's no writer waiting
        if (!writer  && (!writer_decider || !writer_waiting)) {
	        num_readers++;
            reader_cap++;
	        acquired = true;
        }
        //control how many readers get to go through before we shut the doors
        if(reader_cap.load() > 24){
            writer_decider = true;
            reader_cap.store(0);
        }
        internal_mutex.unlock();
    }
  }
  
  void unlock_reader() {
    internal_mutex.lock();
    num_readers--;
    internal_mutex.unlock();
  }

  
  void lock() {
    bool acquired = false;
    writer_waiting.store(true);
    while (!acquired) {
        internal_mutex.lock();
        writer_waiting.store(true);
        if (!writer && num_readers == 0) {
	        writer.store(true);
	        acquired = true;
            writer_cap++;
            writer_waiting.store(false);
        }
        if(writer_cap.load() > 4){
            writer_decider = false;
            writer_cap.store(0);
        }
        internal_mutex.unlock();
    }
  }
  
  void unlock() {
    internal_mutex.lock();
    writer = false;
    internal_mutex.unlock();
  }

 private:
    mutex internal_mutex;
    atomic_bool writer;
    atomic_bool writer_decider;
    atomic_int num_readers;
    atomic_int writer_cap;
    atomic_int reader_cap;

    //we respect our authors in these parts
    atomic_bool writer_waiting;
};
