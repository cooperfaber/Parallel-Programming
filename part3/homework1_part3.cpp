#include <iostream>
#include <assert.h>
#include <chrono>
#include <thread>
using namespace std;
using namespace std::chrono;


void sequential_comp(volatile int *a){
    for(int i = 0; i < 1024; i++){
        for(int j = 0; j < 524288; j++){
            a[i]++;
        }
    }
}

void robin_comp(volatile int *b, int tid, int thr_count){
    for(int i = tid; i < 1024; i+= thr_count){
        for(int j = 0; j < 524288; j++){
            b[i]++;
        }
    }
}   

void fancy_comp(volatile int *c, int tid, int thr_count){
    for(int i = tid; i < 1024; i+= thr_count){
        for(int j = 0; j < 524288; j++){
            c[i]++;
        }
    }
}   

void thread_init(int opcode, volatile int *b, int thr_count){
    std::thread threads[thr_count];
    int tid;
    for(int i = 0; i < thr_count; i++){
        tid = i % thr_count;
        if(opcode == 1)threads[tid] = std::thread(robin_comp, b, tid, thr_count);
        else if(opcode == 2){
            threads[tid] = std::thread(fancy_comp, b, tid, thr_count);
        }
    }
    for(int i = 0; i < thr_count; i++){
        threads[i].join();
    }
    for(int i = 0; i < thr_count; i++){
        threads[i].~thread();
    }
}

int main(int argc, char *argv[]){
    //check for argument thread count
    int thr_count;
    if(argc == 1){
        thr_count = 8;
        cout << "No thread argument detected! Using 8 threads.\n";
    }
    else if(argc > 1){
        thr_count = atoi(argv[1]);
        if(thr_count > 100 || thr_count < 1){
            cout << "Max amount of threads exceeded, using 8.\n";
            thr_count = 8;
        }
    }

    //initialize arrays, fill with zeros
    volatile int a[1024];
    volatile int b[1024];
    volatile int c[1024];
    for(int i = 0; i < 1024; i++){
        a[i] = 0;
        b[i] = 0;
        c[i] = 0;
    }

    //nicked from part 1, sequential (reference) timing
    auto sequential_start = high_resolution_clock::now();
    sequential_comp(a);
    auto sequential_end = high_resolution_clock::now();

    //round robin (part 2) timing
    auto robin_start = high_resolution_clock::now();
    thread_init(1, b, thr_count);
    auto robin_end = high_resolution_clock::now();

    //personal attempt (part 3) timing
    auto fancy_start = high_resolution_clock::now();
    thread_init(2, c, thr_count);
    auto fancy_end = high_resolution_clock::now();
    //correctness check
    for(int i = 0; i < 1024; i++){
        assert(a[i] == b[i]);
        assert(a[i] == c[i]);
    }
    auto sequential_duration = duration_cast<nanoseconds>(sequential_end - sequential_start);
    double sequential_seconds = sequential_duration.count()/1000000000.0;
    cout << "sequential computation time: " << sequential_seconds << endl; 
    auto robin_duration = duration_cast<nanoseconds>(robin_end - robin_start);
    double robin_seconds = robin_duration.count()/1000000000.0;
    cout << "round robin computation time: " << robin_seconds << endl; 
    auto fancy_duration = duration_cast<nanoseconds>(fancy_end - fancy_start);
    double fancy_seconds = fancy_duration.count()/1000000000.0;
    cout << "personal attempt computation time: " << fancy_seconds << endl;
    cout << "relative round robin speedup: " << sequential_seconds/robin_seconds << endl;
    cout << "relative personal attempt speedup: " << sequential_seconds/fancy_seconds << endl;
    cout << "speedup between robin and personal: " << robin_seconds/fancy_seconds << endl; 

}


