void matrix_mult(int* A, int* B, int* output, int d0, int d1, int d2, int tid){
    int chunk_size = d1/4;
    int start = tid*chunk_size;
    int end = start + chunk_size;
    for (int i = 0; i < d0; i++) {
        for (int j = 0; j < d2; j++) {
            //each thread takes "chunk_size" amount of rows of A
            //and the respective columns of B
            for (int k = start; k < end; k++) {
                //again, assuming that the data race this operation causes
                //will not cause conflicts
                //if it would, you could just slap a mutex here and solve that
                output[i][j] += A[i][k] * B[k][j];
            }
        }
    }
}

int main(){
    //assume A, B & output are initialized
    //with respective dimensions d0xd1, d1xd2
    std::thread threads[4];
    for(int i = 0; i < 4; i++){
        threads[i] = std::thread(matrix_mult,&A, &B, &output, d0, d1, d2, i);
    }
    for(int i = 0; i < 4; i++){
        threads[i].join();
    }
    //assume output is checked
}