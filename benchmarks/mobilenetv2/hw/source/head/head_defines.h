#include "../common/support.h"
#include "../../../head_hw_defines.h"
/***********************************************************
 * Computation Defines
 ***********************************************************/
#define KERNEL_SIZE		3
#define HALF_SIZE		(((KERNEL_SIZE)-1) / 2)

// Normal Conv
#define CONV_IN_SIZE	96
#define CONV_OUT_SIZE	40
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
#define DW_IN_SIZE		40
#define DW_OUT_SIZE		40
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
#define PW_IN_SIZE		40
#define PW_OUT_SIZE		40
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
 * MMR Addresses
 ***********************************************************/
#define TOP_MMR			TOP
#define STREAM_DMA_MMR	STREAM_DMA0_Flags
#define CLUSTER_DMA_MMR	DMA_Flags
#define CONV_MMR		NORMALCONV
#define DW_MMR			DWCONV
#define PW_MMR			PWCONV

/***********************************************************
 * Memory Buffer and SPM Addresses
 ***********************************************************/
#define StreamIn		STREAM_DMA0_Stream
#define StreamOut		STREAM_DMA0_Stream

#define ConvIn 			StreamIn
#define ConvBuffer		NormalConvQBuffer
#define ConvWindow		NormalConvWindow
#define ConvWeights		NormalConvWeights
#define ConvBias		NormalConvQParams
#define ConvIMultBias	NormalConvQParams
#define ConvNShiftBias  NormalConvQParams
#define ConvIMultOut	NormalConvQParams
#define ConvNShiftOut	NormalConvQParams
#define ConvWeightZP	NormalConvQParams
#define ConvOut			NormalConvOut

#define DWIn 			ConvOut
#define DWBuffer		DWConvQBuffer
#define DWWindow		DWConvWindow
#define DWOutBuffer		DWConvOutBuffer
#define DWWeights		DWConvWeights
#define DWBias			DWConvQParams
#define DWIMultBias		DWConvQParams
#define DWNShiftBias  	DWConvQParams
#define DWIMultOut		DWConvQParams
#define DWNShiftOut		DWConvQParams
#define DWWeightZP		DWConvQParams
#define DWOut			DWConvOut

#define PWIn 			DWOut
#define PWLocalFeat		PWConvLocalFeatSize
#define PWWeights		PWConvWeights
#define PWBias			PWConvQParams
#define PWIMultBias		PWConvQParams
#define PWNShiftBias  	PWConvQParams
#define PWIMultOut		PWConvQParams
#define PWNShiftOut		PWConvQParams
#define PWWeightZP		PWConvQParams
#define PWOut			StreamOut