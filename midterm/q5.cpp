classCAS_mutex 
{
    public:CAS_mutex() {
        flag = 0;
        //store -1, which isn't a possible thread id
        taken_thread.store(-1);
        taken_next.store(0);
    }
    void lock(int thread_id) {
        int e = 0;
        while(atomic_compare_exchange(&flag, &e, 1) == false) {
            e = 0;
        }
        //once lock is taken(after while loop)
        //if we took it last time, count that
        if(thread_id == taken_thread){
            taken_next++;
        }
        //otherwise set taken thread
        else{
            taken_thread.load(thread_id);
            taken_next.store(0);
        } 
    }
    void unlock(int thread_id) {
        flag.store(0);
        //tolerance amount can be changed, but setting it at 3
        if(taken_next > 2){
            //yield
            std::this_thread.yield();
            //then sleep for a half-second, for good measure
            sleep(0.5);
        }
    }
    private:
        atomic_int flag;
        atomic_int taken_thread;
        atomic_int taken_next;
}