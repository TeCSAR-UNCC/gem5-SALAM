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
#define TOP_MMR			BASE + 0x0000
#define STREAM_DMA_MMR	BASE + 0x0041
#define CLUSTER_DMA_MMR	BASE + 0x0069
#define CONV_MMR		BASE + 0x007E
#define DW_MMR			BASE + 0x007F
#define PW_MMR			BASE + 0x0080

/***********************************************************
 * Memory Buffer and SPM Addresses
 ***********************************************************/
#define StreamIn		BASE + 0x0061
#define StreamOut		BASE + 0x0061

#define ConvIn 			StreamIn
#define ConvBuffer		BASE + 0x0081
#define ConvWindow		BASE + 0x0441
#define ConvWeights		BASE + 0x045C
#define ConvBias		BASE + 0x06E4
#define ConvIMultBias	BASE + 0x06FC
#define ConvNShiftBias  BASE + 0x0714
#define ConvIMultOut	BASE + 0x072C
#define ConvNShiftOut	BASE + 0x0744
#define ConvWeightZP	BASE + 0x075C
#define ConvOut			BASE + 0x0774

#define DWIn 			ConvOut
#define DWBuffer		BASE + 0x0775
#define DWWindow		BASE + 0x1675
#define DWOutBuffer		BASE + 0x174D
#define DWWeights		BASE + 0x1765
#define DWBias			BASE + 0x1855
#define DWIMultBias		BASE + 0x186D
#define DWNShiftBias  	BASE + 0x1885
#define DWIMultOut		BASE + 0x189D
#define DWNShiftOut		BASE + 0x18B5
#define DWWeightZP		BASE + 0x18CD
#define DWOut			BASE + 0x18E5

#define PWIn 			DWOut
#define PWLocalFeat		BASE + 0x18E6
#define PWWeights		BASE + 0x18FE
#define PWBias			BASE + 0x1A7E
#define PWIMultBias		BASE + 0x1A8E
#define PWNShiftBias  	BASE + 0x1A9E
#define PWIMultOut		BASE + 0x1AAE
#define PWNShiftOut		BASE + 0x1ABE
#define PWWeightZP		BASE + 0x1ACE
#define PWOut			StreamOut