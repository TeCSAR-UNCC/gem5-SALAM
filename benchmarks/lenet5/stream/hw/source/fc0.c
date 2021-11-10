#include "../../lenet5_clstr_hw_defines.h"

typedef uint32_t array3d_in[fc0KSize][fc0KSize][fc0InChan];
typedef uint32_t array4d_t[fc0KSize][fc0KSize][fc0InChan][fc0OutChan];
typedef uint32_t kern_array[fc0InChan][fc0OutChan];

void readStream(uint32_t* strIn, uint32_t* buffer) {
    int inchan;

    #pragma nounroll
    for (inchan=0; inchan<fc0InChan; inchan++) {
        buffer[inchan] = *strIn;
    }
}

void compute(uint32_t* buffer, kern_array kernel, uint32_t* strOut) {

    uint32_t sum;

    int inchan, outchan;
    for (outchan=0; outchan<fc0OutChan; outchan++) {
        sum = 0;
        #pragma unroll
        for (inchan=0; inchan<fc0InChan; inchan++) {
            sum += buffer[inchan] * kernel[inchan][outchan];
        }
        *strOut = sum;
    }
}

void top() {
    void* strIn = (void*)Conv2Out;
    void* buffer = (void*)FC0LineBuff;
    void* kernel = (void*)FC0Weights;
    void* strOut = (void*)STREAMDMA_Stream;

    readStream(strIn,buffer);
	compute(buffer,kernel,strOut);

	return;
}