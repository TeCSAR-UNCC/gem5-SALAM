#include "../common/support.h"
#include "../../../head_hw_defines.h"
#include "../../../network_params.h"
/***********************************************************
 * Computation Defines
 ***********************************************************/
#define KERNEL_SIZE		3
#define HALF_SIZE		(((KERNEL_SIZE)-1) / 2)

// Normal Conv
#define CONV_IN_SIZE	__FEATURES_0_0__INPUT_SIZE__
#define CONV_OUT_SIZE	__FEATURES_0_0__OUTPUT_SIZE__
#define CONV_IN_CH		__FEATURES_0_0__INPUT_CHAN__
#define CONV_OUT_CH		__FEATURES_0_0__OUTPUT_CHAN__
#define CONV_STRIDE		__FEATURES_0_0__STRIDE_SIZE__
#define CONV_BIAS_ZP	0
#define CONV_INPUT_ZP	0
#define CONV_OUTPUT_ZP	0
#define ConvInputSize	CONV_IN_SIZE*CONV_IN_SIZE*CONV_IN_CH
#define ConvBuffSize	CONV_IN_SIZE*(KERNEL_SIZE-1)*CONV_IN_CH
#define ConvWindowSize	KERNEL_SIZE*KERNEL_SIZE*CONV_IN_CH
#define ConvWeightSize	ConvWindowSize*CONV_OUT_CH
#define ConvQParamSize	CONV_OUT_CH

// DW Conv
#define DW_IN_SIZE		__FEATURES_1_CONV_0__INPUT_SIZE__
#define DW_OUT_SIZE		__FEATURES_1_CONV_0__OUTPUT_SIZE__
#define DW_IN_CH		__FEATURES_1_CONV_0__INPUT_CHAN__
#define DW_OUT_CH		__FEATURES_1_CONV_0__OUTPUT_CHAN__
#define DW_STRIDE		__FEATURES_1_CONV_0__STRIDE_SIZE__ 
#define DW_BIAS_ZP		0
#define DW_INPUT_ZP		0
#define DW_OUTPUT_ZP	0
#define DWBuffSize		DW_IN_SIZE*(KERNEL_SIZE-1)*DW_IN_CH
#define DWWindowSize	KERNEL_SIZE*KERNEL_SIZE*DW_IN_CH
#define DWWeightSize	DW_OUT_CH*((KERNEL_SIZE*KERNEL_SIZE)+1)
#define DWQParamSize	DW_OUT_CH

// PW Conv
#define PW_IN_SIZE		__FEATURES_1_CONV_2__INPUT_SIZE__
#define PW_OUT_SIZE		__FEATURES_1_CONV_2__OUTPUT_SIZE__
#define PW_IN_CH		__FEATURES_1_CONV_2__INPUT_CHAN__
#define PW_OUT_CH		__FEATURES_1_CONV_2__OUTPUT_CHAN__
#define PW_CORE_SIZE	24 // Probably need to set this to input channel size, for now that doesn't matter
#define PW_BIAS_ZP		0
#define PW_INPUT_ZP		0
#define PW_OUTPUT_ZP	0
#define PWWeightSize	PW_OUT_CH*PW_IN_CH
#define PWQParamSize	PW_OUT_CH
#define PWOutputSize	PW_OUT_SIZE*PW_OUT_SIZE*PW_OUT_CH

// StreamDMA
#define INPUT_SIZE		CONV_IN_SIZE*CONV_IN_SIZE*CONV_IN_CH
#define OUTPUT_SIZE		PW_OUT_SIZE*PW_OUT_SIZE*PW_OUT_CH