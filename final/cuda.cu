

__global__ void blur(double * d_input, double * d_output, int d_size) {
    int start = threadIdx.x;
    //cover first index
    if(start == 0)start = 1;
    //blur
    for (int i = start; i < d_size-1; i+= blockDim.x) {
        d_output[i] = (d_input[i-1] + d_input[i] + d_input[i+1]) / 3;
    }
}



int main() {
 double *input = new double[SIZE];
 double *output = new double[SIZE];
 
 // input initialization (not shown)

 //copy inputs to graphics card
 cudaMalloc(&d_input,SIZE*sizeof(double));
 cudaMalloc(&d_output,SIZE*sizeof(double));
 cudaMalloc(&d_size,sizeof(int));

 cudaMemcpy(d_input, input, SIZE*sizeof(double),cudaMemcpyHostToDevice);
 cudaMemcpy(d_output, output, SIZE*sizeof(double),cudaMemcpyHostToDevice);
 cudaMemcpy(d_size, SIZE, sizeof(int),cudaMemcpyHostToDevice);

 vector_add<<<1,512>>>blur(d_input, d_output, d_size);
 // check results in output (not shown)
 return 0;
}