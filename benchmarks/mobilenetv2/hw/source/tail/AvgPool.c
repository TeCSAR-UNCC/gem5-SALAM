#include "tail_defines.h"

#define t_ACCUMULATE_TYPE 	dType_16t
#define t_I_O_CHAN			POOL_CH
#define t_KERNEL_SIZE		POOL_SIZE

void Avg_Pooling() {
	dType_8u   iMult_avg  = POOL_IMULT;
	dType_8t   nShift_avg = POOL_NSHIFT;
	volatile dType_8u * inBuffer   = (dType_8u *)tail_ReshapeOut;
	volatile dType_8u * outFifo    = (dType_8u *)TAIL_STREAM_DMA0_Stream;

	AVG_LOOP_OVER_I_CHAN:
    #pragma nounroll
    for (dType_32u jdx = 0, hdx = 0; jdx < t_I_O_CHAN; jdx++)
    {
        t_ACCUMULATE_TYPE acc = 0;
    AVG_LOOP_OVER_K_SIZE:
        #pragma nounroll
        for (dType_32u idx = 0; idx < t_KERNEL_SIZE * t_KERNEL_SIZE; idx++, hdx++)
        {
            acc += inBuffer[hdx];
        }
        t_ACCUMULATE_TYPE downScaled_out = (acc * iMult_avg);
        t_ACCUMULATE_TYPE mul = downScaled_out >> nShift_avg;
        dType_8u out_8bit = (dType_8u)(MAX(0,MIN(mul,255)));
        *outFifo = out_8bit;
    }
}