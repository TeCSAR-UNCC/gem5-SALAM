#include "head_defines.h"
#define IN_SIZE		           	CONV_IN_SIZE
#define OUT_SIZE	           	CONV_OUT_SIZE
#define IN_CH	          		CONV_IN_CH
#define OUT_CH	         		CONV_OUT_CH
#define STRIDE	         		CONV_STRIDE
#define BIAS_ZP	        		CONV_BIAS_ZP
#define INPUT_ZP        		CONV_INPUT_ZP
#define OUTPUT_ZP 		        CONV_OUTPUT_ZP

#define t_MAX_INPUT_CHANNEL     IN_CH
#define t_MAX_OUTPUT_CHANNEL    OUT_CH
#define t_MAX_INPUT_SIZE        IN_SIZE
#define t_ACCUMULATE_TYPE       dType_Reg
#define t_STRIDE                STRIDE
#define t_KSIZE                 KERNEL_SIZE

void NormalConv() {
	volatile dType_8u * STR_IN  	= (dType_8u *)(HEAD_STREAM_DMA0_Stream);
	volatile dType_8u * BUFFER 		= (dType_8u *)(head_NormalConvQBuffer);
	volatile dType_8u * WINDOW 		= (dType_8u *)(head_NormalConvWindow);
	volatile dType_8u * WEIGHT		= (dType_8u *)(head_NormalConvWeights);
	volatile dType_8u * BIAS		= (dType_8u *)(head_NormalConvBias);
	volatile dType_8u * IMULTBIAS	= (dType_8u *)(head_NormalConvIMultBias);
	volatile dType_8t * NSHIFTBIAS	= (dType_8t *)(head_NormalConvNShiftBias);
	volatile dType_8u * IMULTOUT	= (dType_8u *)(head_NormalConvIMultOut);
	volatile dType_8u * NSHIFTOUT	= (dType_8u *)(head_NormalConvNShiftOut);
	volatile dType_8u * WEIGHTZP 	= (dType_8u *)(head_NormalConvWeightZP);
	volatile dType_8u * STR_OUT		= (dType_8u *)(head_NormalConvOut);

	dType_Reg	o_size 		= OUT_SIZE;
    dType_Reg	i_size 		= IN_SIZE;
    dType_Reg	o_c_size 	= OUT_CH;
    dType_Reg	i_c_size 	= IN_CH;
	dType_8u	bias_zp 	= BIAS_ZP;
	dType_8u	input_zp 	= INPUT_ZP;
	dType_8u	output_zp	= OUTPUT_ZP;
	dType_8u	weight_zp	= 0;
	dType_8u	bias 		= 0;
	dType_Reg	y_com 		= 0;
    dType_Reg	x_com 		= 0;
    dType_Reg	y_looper 	= 0;
    dType_Reg	x_looper 	= 0;
    dType_Reg	read_count 	= (i_size * HALF_SIZE + HALF_SIZE + 1) * i_c_size;

// Warmup
buf_x1_n:
	#pragma nounroll
    for (dType_Reg x = i_size - HALF_SIZE - 1; x < i_size; x++) {
    itr_channel_x1:
		#pragma nounroll
        for (dType_Reg ch = 0; ch < i_c_size; ch++) {
            // BUFFER[ch][HALF_SIZE - 1][x] = *STR_IN;
            BUFFER[ch*(t_KSIZE-1)*i_size + (HALF_SIZE-1)*i_size + x] = *STR_IN;
        }
    }

buf_y_n:
	#pragma nounroll
    for (dType_8u y = HALF_SIZE; y < t_KSIZE - 1; y++) {
    buf_x2_n:
		#pragma nounroll
        for (dType_Reg x = 0; x < i_size; x++) {
        itr_channel_x2:
			#pragma nounroll
            for (dType_Reg ch = 0; ch < i_c_size; ch++) {
                // BUFFER[ch][y][x] = *STR_IN;
                BUFFER[ch*(t_KSIZE-1)*i_size + y*i_size + x] = *STR_IN;
            }
        }
    }

win_y_n:
	#pragma nounroll
    for (dType_8u y = HALF_SIZE; y < t_KSIZE; y++) {
    win_x_n:
		#pragma nounroll
        for (dType_8u x = HALF_SIZE; x < t_KSIZE; x++) {
        win_itr_channel:
			#pragma nounroll
            for (dType_Reg ch = 0; ch < i_c_size; ch++) {
                // WINDOW[ch][y][x] = BUFFER[ch][y - 1][x + i_size - t_KSIZE];
                WINDOW[ch*t_KSIZE*t_KSIZE + y*t_KSIZE + x] = BUFFER[ch*(t_KSIZE-1)*i_size + (y-1)*i_size + (x+i_size-t_KSIZE)];
            }
        }
    }

// Start Convolution
    dType_Reg yVal, xVal;
    if (t_STRIDE == 2) {
        x_looper = y_looper = o_size * 2;
    } else {
        x_looper = y_looper = o_size;
    }

for_y_n:
	#pragma nounroll
    for (dType_Reg y = 0; y < y_looper; y++) {
    	y_com = y;
    for_x_n:
		#pragma nounroll
        for (dType_Reg x = 0; x < x_looper; x++) {
        	x_com = x;
    	compute_loop_channel_n:
			#pragma nounroll
        	for (dType_Reg oc = 0; oc < o_c_size; oc++) {
        		bias = BIAS[oc];
        		weight_zp = WEIGHTZP[oc];
        		dType_16t mul_res;
			    dType_Reg val_out = 0;

			    int count = 0;

			win_i_n:
				#pragma unroll
			    for (int i = -HALF_SIZE; i <= HALF_SIZE; i++) {
			    win_j_n:
					#pragma unroll
			        for (int j = -HALF_SIZE; j <= HALF_SIZE; j++) {
			        acc_depth_ch:
						#pragma unroll
			            for (int ch = 0; ch < i_c_size; ch++, count++) {
			                if (0 <= (y+i) && (y+i) < i_size && 0 <= (x+j) && (x+j) < i_size) {
			                    mul_res = (WINDOW[ch*t_KSIZE*t_KSIZE + (i+HALF_SIZE)*t_KSIZE + (j+HALF_SIZE)] - input_zp) * (WEIGHT[count] - weight_zp);
			                    val_out += mul_res;
			                }
			            }
			        }
			    }

        		t_ACCUMULATE_TYPE out_i32;
        		dType_Reg scaled_bias;
        		dType_16t bias_calib = bias - bias_zp;

        		if (NSHIFTBIAS[oc] >= 0) {
        			scaled_bias = (bias_calib * IMULTBIAS[oc]) >> NSHIFTBIAS[oc];
        		} else {
        			scaled_bias = (bias_calib * IMULTBIAS[oc]) >> -NSHIFTBIAS[oc];
        		}
        		out_i32 = (val_out + scaled_bias);
        		out_i32 = out_i32 * IMULTOUT[oc];
        		out_i32 = (out_i32 >> NSHIFTOUT[oc]) + output_zp;
        		dType_8u out_8bit = (t_ACCUMULATE_TYPE)(MAX(0,MIN(out_i32,255)));

        		if (t_STRIDE == 2) {
        			if (!(y_com % 2) && !(x_com % 2)) {
        				*STR_OUT = out_8bit;
        			}
        		} else {
        			*STR_OUT = out_8bit;
        		}
        	}
		Data_movement_loop:
			#pragma nounroll
		    for (dType_Reg ch = 0; ch < i_c_size; ch++) {
		    shift_win_y:
				#pragma unroll
		        for (dType_8u y = 0; y < t_KSIZE; y++) {
		        shift_win_x:
					#pragma unroll
		            for (dType_8u x = 0; x < t_KSIZE - 1; x++) {
		                // WINDOW[ch][y][x] = WINDOW[ch][y][x + 1];
		                WINDOW[ch*t_KSIZE*t_KSIZE + y*t_KSIZE + x] = WINDOW[ch*t_KSIZE*t_KSIZE + y*t_KSIZE + (x+1)];
		            }
		        }
		        // WINDOW[ch][0][t_KSIZE - 1] = BUFFER[ch][0][x];
		        WINDOW[ch*t_KSIZE*t_KSIZE + 0*t_KSIZE + (t_KSIZE-1)] = BUFFER[ch*(t_KSIZE-1)*i_size + 0*i_size + x];
		    update_idx1:
				#pragma unroll
		        for (int y = 1; y < t_KSIZE - 1; y++) {
		            // WINDOW[ch][y][t_KSIZE - 1] = BUFFER[ch][y - 1][x] = BUFFER[ch][y][x];
		            int winidx = ch*t_KSIZE*t_KSIZE + y*t_KSIZE + (t_KSIZE-1);
		            int buffidx1 = ch*(t_KSIZE-1)*i_size + (y-1)*i_size + x;
		            int buffidx2 = ch*(t_KSIZE-1)*i_size + y*i_size + x;
		            WINDOW[winidx] = BUFFER[buffidx1] = BUFFER[buffidx2];
		        }
		        dType_8u val_in = 0;
		        if (read_count < i_size * i_size * i_c_size) {
		            val_in = *STR_IN;
		            read_count++;
		        }
		        // WINDOW[ch][t_KSIZE - 1][t_KSIZE - 1] = BUFFER[ch][t_KSIZE - 2][x] = val_in;
		        int winidx = ch*t_KSIZE*t_KSIZE + (t_KSIZE-1)*t_KSIZE + (t_KSIZE-1);
		        int buffidx = ch*(t_KSIZE-1)*i_size + (t_KSIZE-2)*i_size + x;
		        WINDOW[winidx] = BUFFER[buffidx] = val_in;
		    }
		}
	}
}