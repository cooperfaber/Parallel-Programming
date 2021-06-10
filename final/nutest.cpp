#include <iostream>
#include <thread>

void matrix_mult(int* input, int* output){
    for (int i = 0; i < 12; i++) {
        output[i] += input[i];
    }
}

int main(){
    //assume A, B & output are initialized
    //with respective dimensions d0xd1, d1xd2
    int* nu = new int[12];
    int* done = new int[12];
    for(int i = 0; i < 12; i++){
        nu[i] = 1;
        done[i] = 0;
    }
    std::thread threads[4];
    for(int i = 0; i < 4; i++){
        threads[i] = std::thread(matrix_mult, nu, done);
    }
    for(int i = 0; i < 4; i++){
        threads[i].join();
    }
    for(int i = 0; i < 12; i++){
        std::cout << done[i] << std::endl;
    }
}