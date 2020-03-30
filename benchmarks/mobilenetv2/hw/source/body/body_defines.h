#include "../common/support.h"

/***********************************************************
 * Base Computation Defines
 ***********************************************************/
#define KERNEL_SIZE		3
#define HALF_SIZE		(((KERNEL_SIZE)-1) / 2)

// PW Conv 0
#define PW0_IN_CH_MAX	120
#define PW0_OUT_CH_MAX	720
#define PW0_CORE_SIZE	120
#define PW0WeightSize	PW0_OUT_CH_MAX*PW0_IN_CH_MAX
#define PW0QParamSize	PW0_OUT_CH_MAX*6

// DW Conv
#define DW0_IN_SIZE_MAX	80
#define DW0_IN_CH_MAX	720
#define DW0_OUT_CH_MAX	720
#define DW0BuffSize		DW0_IN_SIZE_MAX*(KERNEL_SIZE-1)*DW0_IN_CH_MAX
#define DW0WindowSize	KERNEL_SIZE*KERNEL_SIZE*DW0_IN_CH_MAX
#define DW0WeightSize	DW0_OUT_CH_MAX*((KERNEL_SIZE*KERNEL_SIZE)+1)
#define DW0QParamSize	DW0_OUT_CH_MAX*6

// PW Conv 1
#define PW1_IN_CH_MAX	720
#define PW1_OUT_CH_MAX	240
#define PW1_CORE_SIZE	120
#define PW1WeightSize	PW1_OUT_CH_MAX*PW1_IN_CH_MAX
#define PW1QParamSize	PW1_OUT_CH_MAX*6

/***********************************************************
 * Cluster Base Address
 ***********************************************************/
#define BASE			0x2F002000
/***********************************************************
 * MMR Addresses
 ***********************************************************/
#define TOP_MMR			BASE + 0x000000
#define STREAM_DMA0_MMR	BASE + 0x00004A
#define STREAM_DMA1_MMR	BASE + 0x000072
#define CLUSTER_DMA_MMR	BASE + 0x00009A
#define RES_MMR			BASE + 0x0000AF
#define PW0_MMR			BASE + 0x0000B5
#define DW0_MMR			BASE + 0x0000C9
#define PW1_MMR			BASE + 0x0000E1

/***********************************************************
 * Memory Buffer and SPM Addresses
 ***********************************************************/
#define StreamIn0		BASE + 0x00006A
#define StreamOut		BASE + 0x00006A
#define StreamIn1		BASE + 0x000092

#define ResIn0			StreamIn0
#define ResIn1			StreamIn1
#define ResOut			BASE + 0x0000F5

#define PW0In 			ResOut
#define PW0LocalFeat	BASE + 0x0000F6
#define PW0Weights		BASE + 0x00016E
#define PW0Bias			BASE + 0x0152EE
#define PW0IMultBias	BASE + 0x0155BE
#define PW0NShiftBias  	BASE + 0x01588E
#define PW0IMultOut		BASE + 0x015B5E
#define PW0NShiftOut	BASE + 0x015E2E
#define PW0WeightZP		BASE + 0x0160FE
#define PW0Out			BASE + 0x0163CE

#define DW0In 			PW0Out
#define DW0Buffer		BASE + 0x0163CF
#define DW0Window		BASE + 0x0325CF
#define DW0OutBuffer	BASE + 0x033F1F
#define DW0Weights		BASE + 0x0341EF
#define DW0Bias			BASE + 0x035E0F
#define DW0IMultBias	BASE + 0x0360DF
#define DW0NShiftBias  	BASE + 0x0363AF
#define DW0IMultOut		BASE + 0x03667F
#define DW0NShiftOut	BASE + 0x03694F
#define DW0WeightZP		BASE + 0x036C1F
#define DW0Out			BASE + 0x036EEF

#define PW1In 			DW0Out
#define PW1LocalFeat	BASE + 0x036EF0
#define PW1Weights		BASE + 0x0371C0
#define PW1Bias			BASE + 0x0614C0
#define PW1IMultBias	BASE + 0x0615B0
#define PW1NShiftBias  	BASE + 0x0616A0
#define PW1IMultOut		BASE + 0x061790
#define PW1NShiftOut	BASE + 0x061880
#define PW1WeightZP		BASE + 0x061970
#define PW1Out			StreamOut

/***********************************************************
 * Per-Layer Computation Defines
 ***********************************************************/
// Iteration 0
#define RES_0_ENABLE	0

#define PW0_0_I_SIZE 	80
#define PW0_0_O_SIZE 	80
#define PW0_0_IC_SIZE	16
#define PW0_0_OC_SIZE	96
#define PW0_0_BIAS_ZP	0
#define PW0_0_IN_ZP		0
#define PW0_0_OUT_ZP	0

#define DW0_0_I_SIZE 	80
#define DW0_0_O_SIZE 	40
#define DW0_0_IC_SIZE	96
#define DW0_0_OC_SIZE	96
#define DW0_0_STRIDE	2
#define DW0_0_BIAS_ZP	0
#define DW0_0_IN_ZP		0
#define DW0_0_OUT_ZP	0

#define PW1_0_I_SIZE 	40
#define PW1_0_O_SIZE 	40
#define PW1_0_IC_SIZE	96
#define PW1_0_OC_SIZE	24
#define PW1_0_BIAS_ZP	0
#define PW1_0_IN_ZP		0
#define PW1_0_OUT_ZP	0

// Iteration 1
#define RES_1_ENABLE	0

#define PW0_1_I_SIZE 	40
#define PW0_1_O_SIZE 	40
#define PW0_1_IC_SIZE	24
#define PW0_1_OC_SIZE	144
#define PW0_1_BIAS_ZP	0
#define PW0_1_IN_ZP		0
#define PW0_1_OUT_ZP	0

#define DW0_1_I_SIZE 	40
#define DW0_1_O_SIZE 	40
#define DW0_1_IC_SIZE	144
#define DW0_1_OC_SIZE	144
#define DW0_1_STRIDE	1
#define DW0_1_BIAS_ZP	0
#define DW0_1_IN_ZP		0
#define DW0_1_OUT_ZP	0

#define PW1_1_I_SIZE 	40
#define PW1_1_O_SIZE 	40
#define PW1_1_IC_SIZE	144
#define PW1_1_OC_SIZE	24
#define PW1_1_BIAS_ZP	0
#define PW1_1_IN_ZP		0
#define PW1_1_OUT_ZP	0

// Iteration 2
#define RES_2_ENABLE	1

#define PW0_2_I_SIZE 	40
#define PW0_2_O_SIZE 	40
#define PW0_2_IC_SIZE	24
#define PW0_2_OC_SIZE	144
#define PW0_2_BIAS_ZP	0
#define PW0_2_IN_ZP		0
#define PW0_2_OUT_ZP	0

#define DW0_2_I_SIZE 	40
#define DW0_2_O_SIZE 	20
#define DW0_2_IC_SIZE	144
#define DW0_2_OC_SIZE	144
#define DW0_2_STRIDE	2
#define DW0_2_BIAS_ZP	0
#define DW0_2_IN_ZP		0
#define DW0_2_OUT_ZP	0

#define PW1_2_I_SIZE 	20
#define PW1_2_O_SIZE 	20
#define PW1_2_IC_SIZE	144
#define PW1_2_OC_SIZE	24
#define PW1_2_BIAS_ZP	0
#define PW1_2_IN_ZP		0
#define PW1_2_OUT_ZP	0

// Iteration 3
#define RES_3_ENABLE	0

#define PW0_3_I_SIZE 	20
#define PW0_3_O_SIZE 	20
#define PW0_3_IC_SIZE	24
#define PW0_3_OC_SIZE	144
#define PW0_3_BIAS_ZP	0
#define PW0_3_IN_ZP		0
#define PW0_3_OUT_ZP	0

#define DW0_3_I_SIZE 	20
#define DW0_3_O_SIZE 	20
#define DW0_3_IC_SIZE	144
#define DW0_3_OC_SIZE	144
#define DW0_3_STRIDE	1
#define DW0_3_BIAS_ZP	0
#define DW0_3_IN_ZP		0
#define DW0_3_OUT_ZP	0

#define PW1_3_I_SIZE 	20
#define PW1_3_O_SIZE 	20
#define PW1_3_IC_SIZE	144
#define PW1_3_OC_SIZE	24
#define PW1_3_BIAS_ZP	0
#define PW1_3_IN_ZP		0
#define PW1_3_OUT_ZP	0

// Iteration 4
#define RES_4_ENABLE	1

#define PW0_4_I_SIZE 	20
#define PW0_4_O_SIZE 	20
#define PW0_4_IC_SIZE	24
#define PW0_4_OC_SIZE	144
#define PW0_4_BIAS_ZP	0
#define PW0_4_IN_ZP		0
#define PW0_4_OUT_ZP	0

#define DW0_4_I_SIZE 	20
#define DW0_4_O_SIZE 	20
#define DW0_4_IC_SIZE	144
#define DW0_4_OC_SIZE	144
#define DW0_4_STRIDE	1
#define DW0_4_BIAS_ZP	0
#define DW0_4_IN_ZP		0
#define DW0_4_OUT_ZP	0

#define PW1_4_I_SIZE 	20
#define PW1_4_O_SIZE 	20
#define PW1_4_IC_SIZE	144
#define PW1_4_OC_SIZE	24
#define PW1_4_BIAS_ZP	0
#define PW1_4_IN_ZP		0
#define PW1_4_OUT_ZP	0

// Iteration 5
#define RES_5_ENABLE	1

#define PW0_5_I_SIZE 	20
#define PW0_5_O_SIZE 	20
#define PW0_5_IC_SIZE	24
#define PW0_5_OC_SIZE	144
#define PW0_5_BIAS_ZP	0
#define PW0_5_IN_ZP		0
#define PW0_5_OUT_ZP	0

#define DW0_5_I_SIZE 	20
#define DW0_5_O_SIZE 	10
#define DW0_5_IC_SIZE	144
#define DW0_5_OC_SIZE	144
#define DW0_5_STRIDE	2
#define DW0_5_BIAS_ZP	0
#define DW0_5_IN_ZP		0
#define DW0_5_OUT_ZP	0

#define PW1_5_I_SIZE 	10
#define PW1_5_O_SIZE 	10
#define PW1_5_IC_SIZE	144
#define PW1_5_OC_SIZE	48
#define PW1_5_BIAS_ZP	0
#define PW1_5_IN_ZP		0
#define PW1_5_OUT_ZP	0


// Iteration 6
#define RES_6_ENABLE	0

#define PW0_6_I_SIZE 	10
#define PW0_6_O_SIZE 	10
#define PW0_6_IC_SIZE	48
#define PW0_6_OC_SIZE	288
#define PW0_6_BIAS_ZP	0
#define PW0_6_IN_ZP		0
#define PW0_6_OUT_ZP	0

#define DW0_6_I_SIZE 	10
#define DW0_6_O_SIZE 	10
#define DW0_6_IC_SIZE	288
#define DW0_6_OC_SIZE	288
#define DW0_6_STRIDE	1
#define DW0_6_BIAS_ZP	0
#define DW0_6_IN_ZP		0
#define DW0_6_OUT_ZP	0

#define PW1_6_I_SIZE 	10
#define PW1_6_O_SIZE 	10
#define PW1_6_IC_SIZE	288
#define PW1_6_OC_SIZE	48
#define PW1_6_BIAS_ZP	0
#define PW1_6_IN_ZP		0
#define PW1_6_OUT_ZP	0

// Iteration 7
#define RES_7_ENABLE	1

#define PW0_7_I_SIZE 	10
#define PW0_7_O_SIZE 	10
#define PW0_7_IC_SIZE	48
#define PW0_7_OC_SIZE	288
#define PW0_7_BIAS_ZP	0
#define PW0_7_IN_ZP		0
#define PW0_7_OUT_ZP	0

#define DW0_7_I_SIZE 	10
#define DW0_7_O_SIZE 	10
#define DW0_7_IC_SIZE	288
#define DW0_7_OC_SIZE	288
#define DW0_7_STRIDE	1
#define DW0_7_BIAS_ZP	0
#define DW0_7_IN_ZP		0
#define DW0_7_OUT_ZP	0

#define PW1_7_I_SIZE 	10
#define PW1_7_O_SIZE 	10
#define PW1_7_IC_SIZE	288
#define PW1_7_OC_SIZE	48
#define PW1_7_BIAS_ZP	0
#define PW1_7_IN_ZP		0
#define PW1_7_OUT_ZP	0

// Iteration 8
#define RES_8_ENABLE	1

#define PW0_8_I_SIZE 	10
#define PW0_8_O_SIZE 	10
#define PW0_8_IC_SIZE	48
#define PW0_8_OC_SIZE	288
#define PW0_8_BIAS_ZP	0
#define PW0_8_IN_ZP		0
#define PW0_8_OUT_ZP	0

#define DW0_8_I_SIZE 	10
#define DW0_8_O_SIZE 	10
#define DW0_8_IC_SIZE	288
#define DW0_8_OC_SIZE	288
#define DW0_8_STRIDE	1
#define DW0_8_BIAS_ZP	0
#define DW0_8_IN_ZP		0
#define DW0_8_OUT_ZP	0

#define PW1_8_I_SIZE 	10
#define PW1_8_O_SIZE 	10
#define PW1_8_IC_SIZE	288
#define PW1_8_OC_SIZE	48
#define PW1_8_BIAS_ZP	0
#define PW1_8_IN_ZP		0
#define PW1_8_OUT_ZP	0

// Iteration 9
#define RES_9_ENABLE	1

#define PW0_9_I_SIZE 	10
#define PW0_9_O_SIZE 	10
#define PW0_9_IC_SIZE	48
#define PW0_9_OC_SIZE	288
#define PW0_9_BIAS_ZP	0
#define PW0_9_IN_ZP		0
#define PW0_9_OUT_ZP	0

#define DW0_9_I_SIZE 	10
#define DW0_9_O_SIZE 	10
#define DW0_9_IC_SIZE	288
#define DW0_9_OC_SIZE	288
#define DW0_9_STRIDE	1
#define DW0_9_BIAS_ZP	0
#define DW0_9_IN_ZP		0
#define DW0_9_OUT_ZP	0

#define PW1_9_I_SIZE 	10
#define PW1_9_O_SIZE 	10
#define PW1_9_IC_SIZE	288
#define PW1_9_OC_SIZE	72
#define PW1_9_BIAS_ZP	0
#define PW1_9_IN_ZP		0
#define PW1_9_OUT_ZP	0

// Iteration 10
#define RES_10_ENABLE	0

#define PW0_10_I_SIZE 	10
#define PW0_10_O_SIZE 	10
#define PW0_10_IC_SIZE	72
#define PW0_10_OC_SIZE	432
#define PW0_10_BIAS_ZP	0
#define PW0_10_IN_ZP	0
#define PW0_10_OUT_ZP	0

#define DW0_10_I_SIZE 	10
#define DW0_10_O_SIZE 	10
#define DW0_10_IC_SIZE	432
#define DW0_10_OC_SIZE	432
#define DW0_10_STRIDE	1
#define DW0_10_BIAS_ZP	0
#define DW0_10_IN_ZP	0
#define DW0_10_OUT_ZP	0

#define PW1_10_I_SIZE 	10
#define PW1_10_O_SIZE 	10
#define PW1_10_IC_SIZE	432
#define PW1_10_OC_SIZE	72
#define PW1_10_BIAS_ZP	0
#define PW1_10_IN_ZP	0
#define PW1_10_OUT_ZP	0

// Iteration 11
#define RES_11_ENABLE	1

#define PW0_11_I_SIZE 	10
#define PW0_11_O_SIZE 	10
#define PW0_11_IC_SIZE	72
#define PW0_11_OC_SIZE	432
#define PW0_11_BIAS_ZP	0
#define PW0_11_IN_ZP	0
#define PW0_11_OUT_ZP	0

#define DW0_11_I_SIZE 	10
#define DW0_11_O_SIZE 	10
#define DW0_11_IC_SIZE	432
#define DW0_11_OC_SIZE	432
#define DW0_11_STRIDE	1
#define DW0_11_BIAS_ZP	0
#define DW0_11_IN_ZP	0
#define DW0_11_OUT_ZP	0

#define PW1_11_I_SIZE 	10
#define PW1_11_O_SIZE 	10
#define PW1_11_IC_SIZE	432
#define PW1_11_OC_SIZE	72
#define PW1_11_BIAS_ZP	0
#define PW1_11_IN_ZP	0
#define PW1_11_OUT_ZP	0

// Iteration 12
#define RES_12_ENABLE	1

#define PW0_12_I_SIZE 	10
#define PW0_12_O_SIZE 	10
#define PW0_12_IC_SIZE	72
#define PW0_12_OC_SIZE	432
#define PW0_12_BIAS_ZP	0
#define PW0_12_IN_ZP	0
#define PW0_12_OUT_ZP	0

#define DW0_12_I_SIZE 	10
#define DW0_12_O_SIZE 	5
#define DW0_12_IC_SIZE	432
#define DW0_12_OC_SIZE	432
#define DW0_12_STRIDE	2
#define DW0_12_BIAS_ZP	0
#define DW0_12_IN_ZP	0
#define DW0_12_OUT_ZP	0

#define PW1_12_I_SIZE 	5
#define PW1_12_O_SIZE 	5
#define PW1_12_IC_SIZE	432
#define PW1_12_OC_SIZE	120
#define PW1_12_BIAS_ZP	0
#define PW1_12_IN_ZP	0
#define PW1_12_OUT_ZP	0

// Iteration 13
#define RES_13_ENABLE	0

#define PW0_13_I_SIZE 	5
#define PW0_13_O_SIZE 	5
#define PW0_13_IC_SIZE	120
#define PW0_13_OC_SIZE	720
#define PW0_13_BIAS_ZP	0
#define PW0_13_IN_ZP	0
#define PW0_13_OUT_ZP	0

#define DW0_13_I_SIZE 	5
#define DW0_13_O_SIZE 	5
#define DW0_13_IC_SIZE	720
#define DW0_13_OC_SIZE	720
#define DW0_13_STRIDE	1
#define DW0_13_BIAS_ZP	0
#define DW0_13_IN_ZP	0
#define DW0_13_OUT_ZP	0

#define PW1_13_I_SIZE 	5
#define PW1_13_O_SIZE 	5
#define PW1_13_IC_SIZE	720
#define PW1_13_OC_SIZE	120
#define PW1_13_BIAS_ZP	0
#define PW1_13_IN_ZP	0
#define PW1_13_OUT_ZP	0

// Iteration 14
#define RES_14_ENABLE	1

#define PW0_14_I_SIZE 	5
#define PW0_14_O_SIZE 	5
#define PW0_14_IC_SIZE	120
#define PW0_14_OC_SIZE	720
#define PW0_14_BIAS_ZP	0
#define PW0_14_IN_ZP	0
#define PW0_14_OUT_ZP	0

#define DW0_14_I_SIZE 	5
#define DW0_14_O_SIZE 	5
#define DW0_14_IC_SIZE	720
#define DW0_14_OC_SIZE	720
#define DW0_14_STRIDE	1
#define DW0_14_BIAS_ZP	0
#define DW0_14_IN_ZP	0
#define DW0_14_OUT_ZP	0

#define PW1_14_I_SIZE 	5
#define PW1_14_O_SIZE 	5
#define PW1_14_IC_SIZE	720
#define PW1_14_OC_SIZE	120
#define PW1_14_BIAS_ZP	0
#define PW1_14_IN_ZP	0
#define PW1_14_OUT_ZP	0

// Iteration 15
#define RES_15_ENABLE	1

#define PW0_15_I_SIZE 	5
#define PW0_15_O_SIZE 	5
#define PW0_15_IC_SIZE	120
#define PW0_15_OC_SIZE	720
#define PW0_15_BIAS_ZP	0
#define PW0_15_IN_ZP	0
#define PW0_15_OUT_ZP	0

#define DW0_15_I_SIZE 	5
#define DW0_15_O_SIZE 	5
#define DW0_15_IC_SIZE	720
#define DW0_15_OC_SIZE	720
#define DW0_15_STRIDE	1
#define DW0_15_BIAS_ZP	0
#define DW0_15_IN_ZP	0
#define DW0_15_OUT_ZP	0

#define PW1_15_I_SIZE 	5
#define PW1_15_O_SIZE 	5
#define PW1_15_IC_SIZE	720
#define PW1_15_OC_SIZE	240
#define PW1_15_BIAS_ZP	0
#define PW1_15_IN_ZP	0
#define PW1_15_OUT_ZP	0