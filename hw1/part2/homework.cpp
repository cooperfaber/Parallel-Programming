
#include <iostream>
#include <assert.h>
#include <chrono>
using namespace std;
using namespace std::chrono;

typedef double reduce_type;


void reference_reduction(reduce_type *b, int size) {
  for (int i = 1; i < size; i++) {
    b[0] += b[i];
  }
}
void homework_reduction(reduce_type *a, int size) {
  int delta_x = size/4;
  for(int i = 1; i < delta_x; i++){
    a[delta_x*0] += a[delta_x*0+i];
    a[delta_x*1] += a[delta_x*1+i];
    a[delta_x*2] += a[delta_x*2+i];
    a[delta_x*3] += a[delta_x*3+i];
  }
  for(int i = 1; i < 4; i++){
    a[0] += a[delta_x*i];
  }
}

#define SIZE (1024*1024*16)


int main() {
  reduce_type *a;
  a = (reduce_type *) malloc(SIZE * sizeof(reduce_type));

  reduce_type *b;
  b = (reduce_type *) malloc(SIZE * sizeof(reduce_type));

  for (int i = 0; i < SIZE; i++) {
    a[i] = 1;
    b[i] = 1;
  }

  auto new_start = high_resolution_clock::now();
  homework_reduction(a,SIZE);
  auto new_stop = high_resolution_clock::now();
  auto new_duration = duration_cast<nanoseconds>(new_stop - new_start);
  double new_seconds = new_duration.count()/1000000000.0;

  auto ref_start = high_resolution_clock::now();
  reference_reduction(b,SIZE);
  auto ref_stop = high_resolution_clock::now();
  auto ref_duration = duration_cast<nanoseconds>(ref_stop - ref_start);
  double ref_seconds = ref_duration.count()/1000000000.0;

  cout << "new loop time: " << new_seconds << endl; 
  cout << "reference loop time: " << ref_seconds << endl; 
  cout << "speedup: " << ref_seconds / new_seconds << endl << endl;

  return 0;
}
