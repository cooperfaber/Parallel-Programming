
class atom_bit_mutex {
    //b = 0 implies lock is free
    //b = 1 implies lock is taken
 public:
  atom_bit_mutex() {
    //set b = 0
    //unsure which bitwise operation to do to an undefined bit
    b.store(0);
  }
   
  void lock(int thread_id) {
    bit n = 0;
    bool acq = false;
    //thank you beautiful engineers
    //for improving this lovely OR function
    //ORs 0 with current state, doing a lil peek
    while(!acq){
        if(atomic_fetch_or(b,n) == 0){
            //recieving a 0 from the above means the lock is open
            //need to close it behind us
            //making sure we're the only ones getting through
            n = 1;
            //OR with 1
            if(atomic_fetch_or(b,n) == 0){
                //if we got 0 again, the lock is closed
                acq = true;
            }
            else{
                //otherwise, another thread beat us to the punch 
                n = 0;
            }
            
        }
    }
  }
  
  void unlock(int thread_id) {
    //need to set the bit from 1 to 0
    bit b = 1;
    atomic_fetch_xor(b,n);
  }

 private:
    atomic_bit b;
};
