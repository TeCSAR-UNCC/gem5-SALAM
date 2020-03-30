#include "../common/support.h"

/***********************************************************
 * Computation Defines
 ***********************************************************/
#define KERNEL_SIZE		3
#define HALF_SIZE		(((KERNEL_SIZE)-1) / 2)

// Normal Conv
#define CONV_IN_SIZE	160
#define CONV_OUT_SIZE	80
#define CONV_IN_CH		3
#define CONV_OUT_CH		24
#define CONV_STRIDE		2
#define CONV_BIAS_ZP	0
#define CONV_INPUT_ZP	0
#define CONV_OUTPUT_ZP	0
#define ConvInputSize	CONV_IN_SIZE*CONV_IN_SIZE*CONV_IN_CH
#define ConvBuffSize	CONV_IN_SIZE*(KERNEL_SIZE-1)*CONV_IN_CH
#define ConvWindowSize	KERNEL_SIZE*KERNEL_SIZE*CONV_IN_CH
#define ConvWeightSize	ConvWindowSize*CONV_OUT_CH
#define ConvQParamSize	CONV_OUT_CH*6

// DW Conv
#define DW_IN_SIZE		80
#define DW_OUT_SIZE		80
#define DW_IN_CH		24
#define DW_OUT_CH		24
#define DW_STRIDE		1
#define DW_BIAS_ZP		0
#define DW_INPUT_ZP		0
#define DW_OUTPUT_ZP	0
#define DWBuffSize		DW_IN_SIZE*(KERNEL_SIZE-1)*DW_IN_CH
#define DWWindowSize	KERNEL_SIZE*KERNEL_SIZE*DW_IN_CH
#define DWWeightSize	DW_OUT_CH*((KERNEL_SIZE*KERNEL_SIZE)+1)
#define DWQParamSize	DW_OUT_CH*6

// PW Conv
#define PW_IN_SIZE		80
#define PW_OUT_SIZE		80
#define PW_IN_CH		24
#define PW_OUT_CH		16
#define PW_CORE_SIZE	24
#define PW_BIAS_ZP		0
#define PW_INPUT_ZP		0
#define PW_OUTPUT_ZP	0
#define PWWeightSize	PW_OUT_CH*PW_IN_CH
#define PWQParamSize	PW_OUT_CH*6
#define PWOutputSize	PW_OUT_SIZE*PW_OUT_SIZE*PW_OUT_CH

// StreamDMA
#define INPUT_SIZE		CONV_IN_SIZE*CONV_IN_SIZE*CONV_IN_CH
#define OUTPUT_SIZE		PW_OUT_SIZE*PW_OUT_SIZE*PW_OUT_CH

/***********************************************************
 * Cluster Base Address
 ***********************************************************/
#define BASE			0x2F000000
/***********************************************************
 * MMR Addresses
 ***********************************************************/
#define TOP_MMR			BASE + 0x1B00
#define STREAM_DMA_MMR	BASE + 0x0001
#define CLUSTER_DMA_MMR	BASE + 0x0029
#define CONV_MMR		BASE + 0x003E
#define DW_MMR			BASE + 0x003F
#define PW_MMR			BASE + 0x0040

/***********************************************************
 * Memory Buffer and SPM Addresses
 ***********************************************************/
#define StreamIn		BASE + 0x0021
#define StreamOut		BASE + 0x0021

#define ConvIn 			StreamIn
#define ConvBuffer		BASE + 0x0041
#define ConvWindow		BASE + 0x0401
#define ConvWeights		BASE + 0x041C
#define ConvBias		BASE + 0x06A4
#define ConvIMultBias	BASE + 0x06BC
#define ConvNShiftBias  BASE + 0x06D4
#define ConvIMultOut	BASE + 0x06EC
#define ConvNShiftOut	BASE + 0x0704
#define ConvWeightZP	BASE + 0x071C
#define ConvOut			BASE + 0x0734

#define DWIn 			ConvOut
#define DWBuffer		BASE + 0x0735
#define DWWindow		BASE + 0x1635
#define DWWeights		BASE + 0x170D
#define DWBias			BASE + 0x17FD
#define DWIMultBias		BASE + 0x1815
#define DWNShiftBias  	BASE + 0x182D
#define DWIMultOut		BASE + 0x1845
#define DWNShiftOut		BASE + 0x185D
#define DWWeightZP		BASE + 0x1875
#define DWOutBuffer		BASE + 0x188D
#define DWOut			BASE + 0x18A5

#define PWIn 			DWOut
#define PWWeights		BASE + 0x18A6
#define PWBias			BASE + 0x1A26
#define PWIMultBias		BASE + 0x1A3E
#define PWNShiftBias  	BASE + 0x1A56
#define PWIMultOut		BASE + 0x1A6E
#define PWNShiftOut		BASE + 0x1A86
#define PWWeightZP		BASE + 0x1A9E
#define PWLocalFeat		BASE + 0x1AB6
#define PWOut			StreamOut