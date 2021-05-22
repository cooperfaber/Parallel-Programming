

void linear_work(int* work, int size) {
  for (int i = 0; i < size; i++) {
    int new_i = i / 1024;
    work[i] = new_i;
  }
}

void squared_work(int* work, int size) {
  for (int i = 0; i < size; i++) {
    int new_i = i / (1024*32);
    work[i] = new_i * new_i;
  }
}

void cubed_work(int* work, int size) {
  for (int i = 0; i < size; i++) {
    int new_i = i / (1024*32*2);
    work[i] = new_i * new_i * new_i;
  }
}
