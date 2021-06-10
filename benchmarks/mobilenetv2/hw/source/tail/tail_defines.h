#include "../common/support.h"
#include "../../../head_hw_defines.h"

/***********************************************************
 * Computation Defines
 ***********************************************************/

// PW Conv
#define PW_IN_SIZE		5
#define PW_OUT_SIZE		5
#define PW_IN_CH		240
#define PW_OUT_CH		1280
#define PW_CORE_SIZE	80
#define PW_BIAS_ZP		0
#define PW_INPUT_ZP		0
#define PW_OUTPUT_ZP	0
#define PWWeightSize	PW_OUT_CH*PW_IN_CH
#define PWQParamSize	PW_OUT_CH*6
#define PWOutputSize	PW_OUT_SIZE*PW_OUT_SIZE*PW_OUT_CH

// Reshape (Matrix Transpose)
#define RS_OUT_SIZE		5
#define RS_OUT_CH		1280

// Avg Pooling
#define POOL_CH			1280
#define POOL_SIZE		5
#define POOL_IMULT		163
#define POOL_NSHIFT		12

// StreamDMA
#define INPUT_SIZE		PW_IN_SIZE*PW_IN_SIZE*PW_IN_CH
#define OUTPUT_SIZE		POOL_CH

/***********************************************************
 * Cluster Base Address
 ***********************************************************/
#define BASE			0x2F063000
/***********************************************************
 * MMR Addresses
 ***********************************************************/
#define TOP_MMR			BASE + 0x000000
#define STREAM_DMA_MMR	BASE + 0x000021
#define CLUSTER_DMA_MMR	BASE + 0x000049
#define PW_MMR			BASE + 0x00005E
#define RS_MMR			BASE + 0x00005F
#define POOL_MMR		BASE + 0x000060

/***********************************************************
 * Memory Buffer and SPM Addresses
 ***********************************************************/
#define StreamIn		BASE + 0x000041
#define StreamOut		BASE + 0x000041

#define PWIn 			StreamIn
#define PWLocalFeat		BASE + 0x000061
#define PWWeights		BASE + 0x000151
#define PWBias			BASE + 0x04B151
#define PWIMultBias		BASE + 0x04B651
#define PWNShiftBias  	BASE + 0x04BB51
#define PWIMultOut		BASE + 0x04C051
#define PWNShiftOut		BASE + 0x04C551
#define PWWeightZP		BASE + 0x04CA51
#define PWOut			BASE + 0x04CF51

#define RSIn			PWOut
#define RSOut 			BASE + 0x04CF52 //Ends at 0x054C52

#define PoolIn			RSOut
#define PoolOut			StreamOut