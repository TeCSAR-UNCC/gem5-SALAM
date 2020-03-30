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
#define BASE			0x2F0BF000
/***********************************************************
 * MMR Addresses
 ***********************************************************/
#define TOP_MMR			BASE + 0x000000
#define STREAM_DMA_MMR	BASE + 0x000020
#define CLUSTER_DMA_MMR	BASE + 0x000048
#define LINEAR_MMR		BASE + 0x00005D

/***********************************************************
 * Memory Buffer and SPM Addresses
 ***********************************************************/
#define FeatSPM			BASE + 0x00005E
#define WeightSPM		BASE + 0x00055E
#define Bias			BASE + 0x138D5E
#define iMultBias		BASE + 0x139146
#define nShiftBias		BASE + 0x13952E
#define iMultOut		BASE + 0x139916
#define nShiftOut		BASE + 0x139CFE
#define WeightZP		BASE + 0x13A0E6
#define SumSPM			BASE + 0x13A4CE
#define OutputBuff		BASE + 0x000040