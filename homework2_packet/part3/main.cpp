#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <stdlib.h> 
using namespace std;
using namespace chrono;

#if defined(COARSE_LOCK)
#include "coarse_lock_stack.h"

#elif defined(RW_LOCK)
#include "rw_lock_stack.h"

#elif defined(SWAPTOP)
#include "swaptop_stack.h"

#else
#error No stack specified!
#endif
CSE113_Stack S;

#define SECONDS 1

#define PEEKERS 2
#define POPPERS 2
#define PUSHERS 2

atomic_int total_operations(0);
atomic_int total_peeks(0);
atomic_int total_pops(0);
atomic_int total_pushes(0);


#if defined(SWAPTOP)
#define SWAPTOPPERS 2
atomic_int total_swaptops(0);
#endif


void peekers() {
  auto start = high_resolution_clock::now();
  int duration = 0.0;
  while (duration < SECONDS) {
    atomic_fetch_add(&total_operations,1);
    atomic_fetch_add(&total_peeks,1);
    
    S.peek();
    
    auto now = high_resolution_clock::now();
    duration = duration_cast<seconds>(now - start).count();      
  }
}

void poppers() {
  auto start = high_resolution_clock::now();
  int duration = 0.0;
  while (duration < SECONDS) {
    atomic_fetch_add(&total_operations,1);
    atomic_fetch_add(&total_pops,1);
    
    S.pop();
    
    auto now = high_resolution_clock::now();
    duration = duration_cast<seconds>(now - start).count();      
  }  
}

void pushers() {
  auto start = high_resolution_clock::now();
  int duration = 0.0;
  while (duration < SECONDS) {
    atomic_fetch_add(&total_operations,1);
    atomic_fetch_add(&total_pushes,1);

    int random_value = rand() % 3;
    S.push(random_value);
    
    auto now = high_resolution_clock::now();
    duration = duration_cast<seconds>(now - start).count();      
  }  
}

#if defined(SWAPTOP)
void swaptoppers() {
  auto start = high_resolution_clock::now();
  int duration = 0.0;
  while (duration < SECONDS) {
    atomic_fetch_add(&total_operations,1);
    atomic_fetch_add(&total_swaptops,1);

    int random_value = rand() % 3;
    S.swaptop(random_value);
    
    auto now = high_resolution_clock::now();
    duration = duration_cast<seconds>(now - start).count();      
  }  
}
#endif


int main() {

  thread pushers_ar[PUSHERS];
  thread poppers_ar[POPPERS];
  thread peekers_ar[PEEKERS];

#if defined(SWAPTOP)
  thread swaptoppers_ar[SWAPTOPPERS];
#endif

  for (int i = 0; i < POPPERS; i++) {
    poppers_ar[i] = thread(poppers);
  }


  for (int i = 0; i < PUSHERS; i++) {
    pushers_ar[i] = thread(pushers);
  }

  for (int i = 0; i < PEEKERS; i++) {
    peekers_ar[i] = thread(peekers);
  }


#if defined(SWAPTOP)
  for (int i = 0; i < SWAPTOPPERS; i++) {
    swaptoppers_ar[i] = thread(swaptoppers);
  }
#endif

  for (int i = 0; i < PUSHERS; i++) {
    pushers_ar[i].join();
  }

  for (int i = 0; i < PEEKERS; i++) {
    peekers_ar[i].join();
  }

  for (int i = 0; i < POPPERS; i++) {
    poppers_ar[i].join();
  }

#if defined(SWAPTOP)
  for (int i = 0; i < SWAPTOPPERS; i++) {
    swaptoppers_ar[i].join();
  }
#endif

  cout << "total operations: " << total_operations << endl;
  cout << "total pops: " << total_pops << endl;
  cout << "total pushes: " << total_pushes << endl;
  cout << "total peeks: " << total_peeks << endl;
  
#if defined(SWAPTOP)
  cout << "total swaptops: " << total_swaptops << endl;
#endif
}
