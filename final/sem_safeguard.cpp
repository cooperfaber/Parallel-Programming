class Semaphore {
private: 
    std::atomic_int counter;
    int cap;
    int* flag;
    //flag: 0 represents outside semaphore
    //1 represents inside enter()
    //2 represents inside critical section

public:
 Semaphore (int num_allowed_threads) {
    cap = num_allowed_threads;
    counter.store(0);
    sizing = false;
    flag = new int[NUM_THREADS];
    for(int i = 0; i < NUM_THREADS; i++){
        flag[i] = 0;
    }
 }
 int enter(int tid) {
    bool acquired = false;
    if(flag[tid] == 0){
        //flag = 0 indicates the thread has not tried to take the lock
        //if it passes this check, it will return 0 always eventually
        flag[tid] = 1;
        //flag = 1 indicates it is waiting
        while(!acquired){
            while(counter.load() == cap){
                //semaphore is full, you must wait
            }
            //semaphore is open, go ahead and add to counter
            std::atomic_fetch_add(&counter,1);
            //verify that we're the only thread that's gonna get out
            if(counter.load() > cap){
                //multiple threads incremented, so we have to undo our increment and start waiting again
                std::atomic_fetch_sub(&counter,1);
            }
            else{
                acquired = true;
                flag[tid] = 2
                //flag = 2 implies it is in critical section
            }
        }
        return 0;
    }
    //otherwise, it returns immediately
    else{
        return -1;
    }
 }
 int exit(int tid) {
    // threads that exit should make room for more threads to enter.
    if(flag[tid] == 2){
        std::atomic_fetch_sub(&counter,1);
        //reset flag to 0, indicating it is done with critical section and may try to take lock again
        flag[tid] = 0;
        return 0;
    }
    else{
        //flag is bad, can't exit
        return -1;
    }
 }
}