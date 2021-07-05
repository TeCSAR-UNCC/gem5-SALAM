#include "../common/support.h"
#include "../../../tail_hw_defines.h"

#define __FEATURES_16_CONV_0__INPUT_CHAN__          56
#define __FEATURES_16_CONV_0__INPUT_SIZE__          3
#define __FEATURES_16_CONV_0__OUTPUT_CHAN__         336
#define __FEATURES_16_CONV_0__OUTPUT_SIZE__         3
#define __FEATURES_16_CONV_0__KERNEL_SIZE__         1
#define __FEATURES_16_CONV_0__STRIDE_SIZE__         1
#define __FEATURES_16_CONV_0__PADDING_SIZE__        0
#define __FEATURES_16_CONV_2__INPUT_CHAN__          336
#define __FEATURES_16_CONV_2__INPUT_SIZE__          3
#define __FEATURES_16_CONV_2__OUTPUT_CHAN__         336
#define __FEATURES_16_CONV_2__OUTPUT_SIZE__         3
#define __FEATURES_16_CONV_2__KERNEL_SIZE__         3
#define __FEATURES_16_CONV_2__STRIDE_SIZE__         1
#define __FEATURES_16_CONV_2__PADDING_SIZE__        1
#define __FEATURES_16_CONV_4__INPUT_CHAN__          336
#define __FEATURES_16_CONV_4__INPUT_SIZE__          3
#define __FEATURES_16_CONV_4__OUTPUT_CHAN__         56
#define __FEATURES_16_CONV_4__OUTPUT_SIZE__         3
#define __FEATURES_16_CONV_4__KERNEL_SIZE__         1
#define __FEATURES_16_CONV_4__STRIDE_SIZE__         1
#define __FEATURES_16_CONV_4__PADDING_SIZE__        0
#define __CONV_0__INPUT_CHAN__                      112
#define __CONV_0__INPUT_SIZE__                      3
#define __CONV_0__OUTPUT_CHAN__                     1280
#define __CONV_0__OUTPUT_SIZE__                     3
#define __CONV_0__KERNEL_SIZE__                     1
#define __CONV_0__STRIDE_SIZE__                     1
#define __CONV_0__PADDING_SIZE__                    0
#define __TAIL_PW_CONV_MAX_INPUT_CHAN__             112
#define __TAIL_PW_CONV_MAX_INPUT_SIZE__             3
#define __TAIL_PW_CONV_MAX_OUTPUT_CHANNEL__         1280
#define __TAIL_PW_CONV_SP_WIEGHT_SIZE__             143360
#define __AVG_POOLING_iMult_KERNEL__                227
#define __AVG_POOLING__nShift_KERNEL__              11

/***********************************************************
 * Computation Defines
 ***********************************************************/

// PW Conv
#define PW_IN_SIZE		__TAIL_PW_CONV_MAX_INPUT_SIZE__
#define PW_OUT_SIZE		3
#define PW_IN_CH		__TAIL_PW_CONV_MAX_INPUT_CHAN__
#define PW_OUT_CH		__TAIL_PW_CONV_MAX_OUTPUT_CHANNEL__
#define PW_CORE_SIZE	120
#define PW_BIAS_ZP		0
#define PW_INPUT_ZP		0
#define PW_OUTPUT_ZP	0
#define PWWeightSize	PW_OUT_CH*PW_IN_CH
#define PWQParamSize	PW_OUT_CH*6
#define PWOutputSize	PW_OUT_SIZE*PW_OUT_SIZE*PW_OUT_CH

// Reshape (Matrix Transpose)
#define RS_OUT_SIZE		3
#define RS_OUT_CH		__TAIL_PW_CONV_MAX_OUTPUT_CHANNEL__

// Avg Pooling
#define POOL_CH			__TAIL_PW_CONV_MAX_OUTPUT_CHANNEL__
#define POOL_SIZE		3
#define POOL_IMULT		__AVG_POOLING_iMult_KERNEL__
#define POOL_NSHIFT		__AVG_POOLING__nShift_KERNEL__

// StreamDMA
#define INPUT_SIZE		PW_IN_SIZE*PW_IN_SIZE*PW_IN_CH
#define OUTPUT_SIZE		POOL_CH

/***********************************************************
 * Cluster Base Address
 ***********************************************************/
// #define BASE			0x2F063000
/***********************************************************
 * MMR Addresses
 ***********************************************************/
#define TOP_MMR			TOP
#define STREAM_DMA_MMR	STREAM_DMA0_Flags
#define CLUSTER_DMA_MMR	DMA_Flags
#define PW_MMR			PWCONV
#define RS_MMR			RESHAPE
#define POOL_MMR		AVGPOOL

/***********************************************************
 * Memory Buffer and SPM Addresses
 ***********************************************************/
#define StreamIn		STREAM_DMA0_Stream
#define StreamOut		STREAM_DMA0_Stream

#define PWIn 			StreamIn
#define PWLocalFeat		PWConvLocalFeatSize
#define PWWeights		PWConvWeights
#define PWBias			PWConvQParams
#define PWIMultBias		PWConvQParams
#define PWNShiftBias  	PWConvQParams
#define PWIMultOut		PWConvQParams
#define PWNShiftOut		PWConvQParams
#define PWWeightZP		PWConvQParams
#define PWOut			PWConvOut

#define RSIn			PWOut
#define RSOut 			ReshapeOut

#define PoolIn			RSOut
#define PoolOut			StreamOut