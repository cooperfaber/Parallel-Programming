#include <atomic>
#include <iostream>
#include <thread>
using namespace std;

#define TEST_ITERATIONS (1024*256)

atomic_int x(0);
atomic_int y(0);
void t0_function(int* output) {
    x.store(1,memory_order_relaxed);
    *output = y.load(memory_order_relaxed);
    
}

void t1_function(int* output) {
    y.store(1,memory_order_relaxed);
    *output = x.load(memory_order_relaxed);
}


int main() {

  int output0 = 0;
  int output1 = 0;
  int output2 = 0;
  int output3 = 0;
  int xval = 0;
  int yval = 0;
  std::thread threads[2];
  for (int i = 0; i < TEST_ITERATIONS; i++) {
    // Run a test iteration
    threads[0] = std::thread(t0_function,&yval);
    threads[1] = std::thread(t1_function,&xval);
    for(int i = 0; i < 2; i++){
        threads[i].join();
    }
    // Record a histogram, fill in the conditions
    
    if ((xval == 1)&&(yval==1)) {
      output0++;
    }
    else if ((xval == 0)&&(yval==1)) {
      output1++;
    }
    else if ((xval == 1)&&(yval==0)) {
      output2++;
    }

    // This should be the relaxed behavior
    else if ((xval == yval)&&(yval==0)) {
      output3++;
    }

    //reset conditionals
    x.store(0);
    y.store(0);

  }

  // Complete the print out using your output instantiations
  cout << "histogram of different observations:" << endl;
  cout << "output0: both 1 " << output0 << endl;
  cout << "output1: x: 0, y: 1 " << output1 << endl;
  cout << "output2: x: 1, y: 0 " << output2 << endl;
  cout << "output3: both 0 " << output3 << endl << endl;
  cout << "relaxed behavior frequency: " << float(output3)/float(TEST_ITERATIONS) << endl;
  
  return 0;
  
}
