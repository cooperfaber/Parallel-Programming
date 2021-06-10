class Semaphore {
private: 
    std::atomic_int counter;
    int cap;
    bool sizing;
public:
 Semaphore (int num_allowed_threads) {
    cap = num_allowed_threads;
    counter.store(0);
    sizing = false;
 }
 void enter(int tid) {
    bool acquired = false;
    while(!acquired){
        while(counter.load() == cap || sizing){
            //semaphore is full, you must wait
        }
        //semaphore is open, go ahead and add to counter
        std::atomic_fetch_add(&counter,1);
        //verify that we're the only thread that's gonna get out
        if(counter.load() > cap || sizing){
            //multiple threads incremented, so we have to undo our increment and start waiting again
            std::atomic_fetch_sub(&counter,1);
        }
        else{
            acquired = true;
        }
    }
 }
 void exit(int tid) {
    // threads that exit should make room for more threads to enter.
    std::atomic_fetch_sub(&counter,1);
 }
 void resize(int new_num_allowed_threads){
    sizing = true;
    while(counter.load() > new_num_allowed_threads){
        //spin until it's safe
    }
    cap = new_num_allowed_threads;
    sizing = false;
 }
}