#include "classifier_defines.h"

#define t_ROW_SIZE		OUTPUT_SIZE
#define t_DEPTH_SIZE	INPUT_SIZE

void linear() {
	volatile dType_8u * input           = (dType_8u *)classifier_LinearFeats;
	volatile dType_8u * weight          = (dType_8u *)classifier_LinearWeights; //dType_8u[t_ROW_SIZE][t_DEPTH_SIZE]
	volatile dType_8u * weight_zp       = (dType_8u *)classifier_PWWeightZP;
	volatile dType_8u * biases          = (dType_8u *)classifier_PWBias;
	volatile dType_8u * iMult_bias_acc  = (dType_8u *)classifier_PWIMultBias;
    volatile dType_8t * nShift_bias_acc = (dType_8t *)classifier_PWNShiftBias;
    volatile dType_8u * iMult_output    = (dType_8u *)classifier_PWIMultOut;
    volatile dType_8u * nShift_output   = (dType_8u *)classifier_PWNShiftOut;
    volatile dType_8u * res             = (dType_8u *)CLASSIFIER_STREAM_DMA0_Stream;
    volatile dType_8u * sum             = (dType_8u *)classifier_LinearSum;

	dType_8u   bias_zp   = BIAS_ZP;
	dType_8u   input_zp  = INPUT_ZP;
	dType_8u   output_zp = OUTPUT_ZP;

LINEAR_CORE_COMP_ROW_LOOP_SUM:
    #pragma nounroll
    for (int i = 0; i < t_ROW_SIZE; i++) {
        sum[i] = 0;
        dType_8u w_zp = weight_zp[i];
    LINEAR_CORE_COMP_DEPTH_LOOP:
    	#pragma unroll 320
        for (int j = 0; j < t_DEPTH_SIZE; j++) {
            dType_Reg k_weight = weight[i*t_DEPTH_SIZE + j] - w_zp;
            dType_Reg input_recalib = input[j] - input_zp;
            dType_Reg term = k_weight * input_recalib;
            sum[i] += term;
        }
    }
LINEAR_CORE_COMP_ROW_LOOP:
    #pragma nounroll
    for (int i = 0; i < t_ROW_SIZE; i++) {
        dType_8u out_8bit;
        dType_32t out_i32;

        dType_Reg bias_calib = biases[i] - bias_zp;
        dType_Reg upScaled_Bias = (bias_calib * iMult_bias_acc[i]);
        dType_Reg scaled_bias;

        if (nShift_bias_acc[i] >= 0) {
            scaled_bias = upScaled_Bias >> nShift_bias_acc[i];
        } else {
            scaled_bias = upScaled_Bias << -nShift_bias_acc[i];
        }
        out_i32 = (sum[i] + scaled_bias);
        dType_Reg downScaled_out = (out_i32 * iMult_output[i]);

        out_i32 = (downScaled_out >> nShift_output[i]) + output_zp;
        out_8bit = out_i32;
        *res = out_8bit;
    }
}