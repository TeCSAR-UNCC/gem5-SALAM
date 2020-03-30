#include "../common/support.h"

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
#define BASE			0x2F065000
/***********************************************************
 * MMR Addresses
 ***********************************************************/
#define TOP_MMR			BASE + 0x000000
#define STREAM_DMA_MMR	BASE + 0x000020
#define CLUSTER_DMA_MMR	BASE + 0x000048
#define PW_MMR			BASE + 0x00005D
#define RS_MMR			BASE + 0x00005E
#define POOL_MMR		BASE + 0x00005F

/***********************************************************
 * Memory Buffer and SPM Addresses
 ***********************************************************/
#define StreamIn		BASE + 0x000040
#define StreamOut		BASE + 0x000040

#define PWIn 			StreamIn
#define PWWeights		BASE + 0x000060
#define PWBias			BASE + 0x04B060
#define PWIMultBias		BASE + 0x04B560
#define PWNShiftBias  	BASE + 0x04BA60
#define PWIMultOut		BASE + 0x04BF60
#define PWNShiftOut		BASE + 0x04C460
#define PWWeightZP		BASE + 0x04C960
#define PWLocalFeat		BASE + 0x04CE60
#define PWOut			BASE + 0x04CF50

#define RSIn			PWOut
#define RSOut 			BASE + 0x04CF51 //Ends at 0x054C51

#define PoolIn			RSOut
#define PoolOut			StreamOut