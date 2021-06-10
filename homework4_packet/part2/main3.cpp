#include <atomic>
#include <iostream>
#include <thread>
using namespace std;

#include "SRBarrier.h"
barrier_object B;

#define TEST_ITERATIONS (1024*256)

atomic_int x(0);
atomic_int y(0);
void t0_function() {
  // complete me!
}

void t1_function() {
  // complete me!
}


int main() {

  int output0 = 0;
  int output1 = 0;
  int output2 = 0;
  int output3 = 0;
  B.init(2);

  for (int i = 0; i < TEST_ITERATIONS; i++) {
    // Run a test iteration

    // Record a histogram, fill in the conditions
    
    if (false) {
      output0++;
    }
    else if (false) {
      output1++;
    }
    else if (false) {
      output2++;
    }

    // This should be the relaxed behavior
    else if (false) {
      output3++;
    }

  }

  // Complete the print out using your output instantiations
  cout << "histogram of different observations:" << endl;
  cout << "output0: <FILL IN OUTPUT> " << output0 << endl;
  cout << "output1: <FILL IN OUTPUT> " << output1 << endl;
  cout << "output2: <FILL IN OUTPUT> " << output2 << endl;
  cout << "output3: <FILL IN OUTPUT> " << output3 << endl << endl;
  cout << "relaxed behavior frequency: " << float(output3)/float(TEST_ITERATIONS) << endl;
  
  return 0;
  
}
