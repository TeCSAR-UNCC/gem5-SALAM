#include "../../lenet5_clstr_hw_defines.h"

void top(uint64_t feats, uint64_t weights) {
	// Define ACC MMRs
	volatile uint8_t  * CONV0Flags  = (uint8_t *)CONV0;
	volatile uint8_t  * DATA_MOVER_0_Flags  = (uint8_t *)DATA_MOVER_0;
	volatile uint8_t  * POOL0Flags  = (uint8_t *)POOL0;
	volatile uint8_t  * DATA_MOVER_1_Flags  = (uint8_t *)DATA_MOVER_1;
	volatile uint8_t  * CONV1Flags  = (uint8_t *)CONV1;
	volatile uint8_t  * DATA_MOVER_2_Flags  = (uint8_t *)DATA_MOVER_2;
	volatile uint8_t  * POOL1Flags  = (uint8_t *)POOL1;
	volatile uint8_t  * DATA_MOVER_3_Flags  = (uint8_t *)DATA_MOVER_3;
	volatile uint8_t  * CONV2Flags  = (uint8_t *)CONV2;
	volatile uint8_t  * DATA_MOVER_4_Flags  = (uint8_t *)DATA_MOVER_4;
	volatile uint8_t  * FC0Flags  = (uint8_t *)FC0;

	// Define DMA MMR
	volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA_Flags);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA_RdAddr);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA_WrAddr);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA_CopyLen);

	//Transfer Input Features
	*DmaRdAddr  = feats;
	*DmaWrAddr  = Conv0Input;
	*DmaCopyLen = conv0InSize;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Transfer Input Weights
	*DmaRdAddr  = weights;
	*DmaWrAddr  = Conv0Weights;
	*DmaCopyLen = conv0WeightSize;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Transfer Input Weights
	*DmaRdAddr  = 0x90000000;
	*DmaWrAddr  = Conv1Weights;
	*DmaCopyLen = conv1WeightSize;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Start conv0
	*CONV0Flags = DEV_INIT;
	//Start Data Mover 0
	*DATA_MOVER_0_Flags = DEV_INIT;
	//Start pool0
	*POOL0Flags = DEV_INIT;
	//Start Data Mover 1
	*DATA_MOVER_1_Flags = DEV_INIT;
	//Start conv1
	*CONV1Flags = DEV_INIT;
	//Start Data Mover 2
	*DATA_MOVER_2_Flags = DEV_INIT;
	//Start pool1
	*POOL1Flags = DEV_INIT;
	//Start Data Mover 3
	*DATA_MOVER_3_Flags = DEV_INIT;
	//Start conv2
	*CONV2Flags = DEV_INIT;
	//Start Data Mover 4
	*DATA_MOVER_4_Flags = DEV_INIT;
	//Start fc0
	*FC0Flags = DEV_INIT;
	// Wait for last acc to exit
	while ((*FC0Flags & DEV_INTR) != DEV_INTR);
	// Transfer Results Back to Main Memory
	*DmaRdAddr  = fc0Output;
	*DmaWrAddr  = 0x90000000;
	*DmaCopyLen = fc0OutputSize;
	*DmaFlags   = DEV_INIT;
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	// //Transfer Input Features
	// *DmaRdAddr  = 0x90000000;
	// *DmaWrAddr  = fc1Input;
	// *DmaCopyLen = fc1InSize;
	// *DmaFlags   = DEV_INIT;
	// //Poll DMA for finish
	// while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	// //Start fc1
	// *FC1Flags = DEV_INIT;
	// //Poll function for finish
	// while ((*FC1Flags & DEV_INTR) != DEV_INTR);
	// //Transfer Results Back to Main Memory
	// *DmaRdAddr  = fc1Output;
	// *DmaWrAddr  = 0x90000000;
	// *DmaCopyLen = fc1OutputSize;
	// *DmaFlags   = DEV_INIT;
	// while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	return;
}