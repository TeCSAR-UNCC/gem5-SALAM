#include "../common/support.h"
#include "../../../classifier_hw_defines.h"
/***********************************************************
 * Computation Defines
 ***********************************************************/

// Linear (Fully Connected)
#define INPUT_SIZE		1280
#define OUTPUT_SIZE		1000
#define WeightSize		INPUT_SIZE*OUTPUT_SIZE
#define BIAS_ZP			0
#define INPUT_ZP		0
#define OUTPUT_ZP		0
#define QParamSize		6*OUTPUT_SIZE

/***********************************************************
 * Cluster Base Address
 ***********************************************************/
// #define BASE			0x2F0B8000
/***********************************************************
 * MMR Addresses
 ***********************************************************/
#define TOP_MMR			TOP
#define STREAM_DMA_MMR	STREAM_DMA0_Flags
#define CLUSTER_DMA_MMR	DMA_Flags
#define LINEAR_MMR		LINEAR

/***********************************************************
 * Memory Buffer and SPM Addresses
 ***********************************************************/
#define FeatSPM			LinearFeats
#define WeightSPM		LinearWeights
#define Bias			LinearQParams
#define iMultBias		LinearQParams
#define nShiftBias		LinearQParams
#define iMultOut		LinearQParams
#define nShiftOut		LinearQParams
#define WeightZP		LinearQParams
#define SumSPM			LinearSum
#define OutputBuff		STREAM_DMA0_Stream