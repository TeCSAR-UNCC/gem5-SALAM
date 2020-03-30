#include "../common/support.h"

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
#define BASE			0x2F0B8000
/***********************************************************
 * MMR Addresses
 ***********************************************************/
#define TOP_MMR			BASE + 0x000000
#define STREAM_DMA_MMR	BASE + 0x000021
#define CLUSTER_DMA_MMR	BASE + 0x000049
#define LINEAR_MMR		BASE + 0x00005E

/***********************************************************
 * Memory Buffer and SPM Addresses
 ***********************************************************/
#define FeatSPM			BASE + 0x00005F
#define WeightSPM		BASE + 0x00055F
#define Bias			BASE + 0x138D5F
#define iMultBias		BASE + 0x139147
#define nShiftBias		BASE + 0x13952F
#define iMultOut		BASE + 0x139917
#define nShiftOut		BASE + 0x139CFF
#define WeightZP		BASE + 0x13A0E7
#define SumSPM			BASE + 0x13A4CF
#define OutputBuff		BASE + 0x000041