#include "body_defines.h"

#define t_MAX_INPUT_CHANNEL		PW0_IN_CH_MAX
#define t_MAX_OUTPUT_CHANNEL	PW0_OUT_CH_MAX
#define t_CORE_SIZE				PW1_CORE_SIZE
#define t_ACCUMULATE_TYPE		dType_Reg
#define t_OUTPUT_TYPE			dType_8u

void PWConv0() {
	volatile dType_8u * 	inFifo 			= (dType_8u *)(PW0In);
    volatile dType_8u * 	outFifo 		= (dType_8u *)(PW0Out);
    volatile dType_8u * 	weights 		= (dType_8u *)(PW0Weights); // dType_8u[t_MAX_OUTPUT_CHANNEL][t_MAX_INPUT_CHANNEL]
    volatile dType_8u * 	iMult_bias_acc 	= (dType_8u *)(PW0IMultBias);
    volatile dType_8t * 	nShift_bias_acc = (dType_8t *)(PW0NShiftBias);
    volatile dType_8u * 	iMult_output 	= (dType_8u *)(PW0IMultOut);
    volatile dType_8u * 	nShift_output 	= (dType_8u *)(PW0NShiftOut);
    volatile dType_8u * 	weight_zp 		= (dType_8u *)(PW0WeightZP);
    volatile dType_8u * 	biases_local 	= (dType_8u *)(PW0Bias);
    volatile dType_8u * 	localFeature 	= (dType_8u *)(PW0LocalFeat); // dType_8u[t_MAX_INPUT_CHANNEL]

pw_convYaxis:
    #pragma clang loop unroll(disable)
    for (int y = 0; y < PW0_0_O_SIZE; y++) {
    pw_convXaxis:
        #pragma clang loop unroll(disable)
        for (int x = 0; x < PW0_0_O_SIZE; x++) {
        rd_buff_loop_img:
            #pragma clang loop unroll(disable)
            for (int i = 0; i < t_MAX_INPUT_CHANNEL; i++) {
                if (i < PW0_0_IC_SIZE) {
                    localFeature[i] = *inFifo;
                } else {
                    localFeature[i] = PW0_0_IN_ZP;
                }
            }
        convOutchan:
            #pragma clang loop unroll(disable)
            for (int oc = 0; oc < PW0_0_OC_SIZE; oc++) {
                dType_8u bias = biases_local[oc];
                dType_Reg sum = 0;
                // Holds temporary accumulator values
                dType_32u weight_idx_offset = oc * PW0_0_IC_SIZE;
                dType_8u weight_zp_local = weight_zp[oc];
            // Runs over filter window
            convInchan_perCore:
                #pragma clang loop unroll(disable)
                for (dType_16u i = 0; i < (t_MAX_INPUT_CHANNEL / t_CORE_SIZE); i++) {
                ADDER_TREE_LOOP:
                    #pragma clang loop unroll(full)
                    for (dType_16u j = 0; j < t_CORE_SIZE; j++) {
                        dType_Reg input = i * t_CORE_SIZE + j;
                        dType_16t input_recalib = localFeature[input] - PW0_0_IN_ZP;
                        dType_8t k_weight = weights[oc*t_MAX_INPUT_CHANNEL + input] - weight_zp_local;
                        dType_16t weighted_input = input_recalib * k_weight;
                        sum = sum + weighted_input;
                    }
                }
                dType_Reg scaled_bias;
                dType_8t bias_calib = bias - PW0_0_BIAS_ZP;

                dType_16t weighted_bias = bias_calib * iMult_bias_acc[oc];

                if (nShift_bias_acc[oc] >= 0) {
                    scaled_bias = (weighted_bias) >> nShift_bias_acc[oc];
                } else {
                    scaled_bias = (weighted_bias) << -nShift_bias_acc[oc];
                }
                dType_Reg biased_input = (sum + scaled_bias);
                t_ACCUMULATE_TYPE out_i32;
                dType_16t signed_imul = iMult_output[oc];
                t_ACCUMULATE_TYPE scaled_output = biased_input * signed_imul;
                out_i32 = (scaled_output >> nShift_output[oc]) + PW0_0_OUT_ZP;
                t_OUTPUT_TYPE out_nBit = (t_OUTPUT_TYPE)(MAX(0,MIN(out_i32,255)));
                *outFifo = out_nBit;
            }
        }
    }
}