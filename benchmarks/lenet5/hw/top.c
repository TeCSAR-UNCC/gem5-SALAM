#include "../lenet5_hw_defines.h"

void top(uint64_t mainMem) {

	// Define ACC MMRs
	volatile uint8_t  * CONV0Flags  = (uint8_t *)CONV0;
	volatile uint8_t  * POOL0Flags  = (uint8_t *)POOL0;
	volatile uint8_t  * CONV1Flags  = (uint8_t *)CONV1;
	volatile uint8_t  * POOL1Flags  = (uint8_t *)POOL1;
	volatile uint8_t  * CONV2Flags  = (uint8_t *)CONV2;
	// Define DMA MMR
	volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA_Flags);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA_RdAddr);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA_WrAddr);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA_CopyLen);

	//Transfer Input Features
	*DmaRdAddr  = 0x90000000;
	*DmaWrAddr  = Conv0Window;
	*DmaCopyLen = conv0InputSize;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer Input Weights
	*DmaRdAddr  = 0x90000000;
	*DmaWrAddr  = Conv0Weights;
	*DmaCopyLen = conv0WeightSize;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Start the conv0
	*CONV0Flags = DEV_INIT;
	//Poll function for finish
	while ((*CONV0Flags & DEV_INTR) != DEV_INTR);
	//Transfer Results Back to Main Memory
	*DmaRdAddr  = Conv0Output;
	*DmaWrAddr  = 0x90000000;
	*DmaCopyLen = conv0OutputSize;
	*DmaFlags   = DEV_INIT;
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Transfer POOL0 Features
	*DmaRdAddr  = 0x90000000;
	*DmaWrAddr  = pool0Window;
	*DmaCopyLen = pool0InputSize;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer POOL0 Weights
	*DmaRdAddr  = 0x90000000;
	*DmaWrAddr  = pool0Weights;
	*DmaCopyLen = pool0WeightSize;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Start the pool0
	*POOL0Flags = DEV_INIT;
	//Poll function for finish
	while ((*CONV0Flags & DEV_INTR) != DEV_INTR);
	//Transfer Results Back to Main Memory
	*DmaRdAddr  = pool0Output;
	*DmaWrAddr  = 0x90000000;
	*DmaCopyLen = pool0OutputSize;
	*DmaFlags   = DEV_INIT;
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Transfer Input Features
	*DmaRdAddr  = 0x90000000;
	*DmaWrAddr  = Conv1Window;
	*DmaCopyLen = conv1InputSize;
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
	//Start the conv1
	*CONV1Flags = DEV_INIT;
	//Poll function for finish
	while ((*CONV1Flags & DEV_INTR) != DEV_INTR);
	//Transfer Results Back to Main Memory
	*DmaRdAddr  = Conv1Output;
	*DmaWrAddr  = 0x90000000;
	*DmaCopyLen = conv1OutputSize;
	*DmaFlags   = DEV_INIT;
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Transfer Input Features
	*DmaRdAddr  = 0x90000000;
	*DmaWrAddr  = pool1Window;
	*DmaCopyLen = pool1InputSize;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer Input Weights
	*DmaRdAddr  = 0x90000000;
	*DmaWrAddr  = pool1Weights;
	*DmaCopyLen = pool1WeightSize;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Start the pool1
	*POOL1Flags = DEV_INIT;
	//Poll function for finish
	while ((*POOL1Flags & DEV_INTR) != DEV_INTR);
	//Transfer Results Back to Main Memory
	*DmaRdAddr  = pool1Output;
	*DmaWrAddr  = 0x90000000;
	*DmaCopyLen = pool1OutputSize;
	*DmaFlags   = DEV_INIT;
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Transfer Input Features
	*DmaRdAddr  = 0x90000000;
	*DmaWrAddr  = Conv2Window;
	*DmaCopyLen = conv2InputSize;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer Input Weights
	*DmaRdAddr  = 0x90000000;
	*DmaWrAddr  = Conv2Weights;
	*DmaCopyLen = conv2WeightSize;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Start the conv2
	*CONV2Flags = DEV_INIT;
	//Poll function for finish
	while ((*CONV2Flags & DEV_INTR) != DEV_INTR);
	//Transfer Results Back to Main Memory
	*DmaRdAddr  = Conv2Output;
	*DmaWrAddr  = 0x90000000;
	*DmaCopyLen = conv2OutputSize;
	*DmaFlags   = DEV_INIT;
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	return;
}