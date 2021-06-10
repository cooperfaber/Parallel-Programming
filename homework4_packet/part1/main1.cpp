#include "common.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <cassert>


void single_blur(double *input, double *output, int size, int tid, int num_threads) {
    int chunk_size = size/num_threads;
    int start = chunk_size*tid;
    int end = chunk_size + start;
    //get rid of first and last indexes
    if(start == 0)start = 1;
    if(end == size)end = size-1;
    for (int i = start; i < end; i++) {
        output[i] = (input[i] + input[i+1] + input[i-1])/3;
    }
}

void repeated_blur(double *input, double *output, int size, int repeats) {
    for (int r = 0; r < repeats; r++) {
        for (int i = 1; i < size - 1; i++) {
            output[i] = (input[i] + input[i+1] + input[i-1])/3;
        }
        double *tmp = input;
        input = output;
        output = tmp;
    }
}

int main(int argc, char *argv[]) {
  int num_threads = 8;
  if (argc > 1) {
    num_threads = atoi(argv[1]);
  }

  double *input = new double[SIZE];
  double *output = new double[SIZE];
  double *input2 = new double[SIZE];
  double *output2 = new double[SIZE];


  for (int i = 0; i < SIZE; i++) {
    double randval = fRand(-100.0, 100.0);
    input[i] = randval;
    output[i] = randval;
    input2[i] = randval;
    output2[i] = randval;    
  }

  auto time_start = std::chrono::high_resolution_clock::now();

  //declare thread array
  std::thread threads[num_threads];
  for (int r = 0; r < REPEATS; r++) {
    // Launch threads to compute the blur
    for(int i = 0; i < num_threads; i++){
        threads[i] = std::thread(single_blur,input,output,SIZE,i,num_threads);
    }
    // Join threads
    for(int i = 0; i < num_threads; i++){
        threads[i].join();
    }
    // Swap input and output pointers
    double *tmp = input;
    input = output;
    output = tmp;
  }

  auto time_end = std::chrono::high_resolution_clock::now();
  auto time_duration = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start);
  double time_seconds = time_duration.count()/1000000000.0;
  std::cout << "timings: " << time_seconds << std::endl;


  /*repeated_blur(input2, output2, SIZE, REPEATS);
  for(int i = 0; i < SIZE; i++){
      if(input[i]!=input2[i])std::cout << input[i] << "==" << input2[i] << std::endl;
  }*/    
}


