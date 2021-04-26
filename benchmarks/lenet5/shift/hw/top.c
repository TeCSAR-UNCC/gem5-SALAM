#include "../lenet5_hw_defines.h"

void top(uint64_t mainMem) {

	// Define ACC MMRs
	volatile uint8_t  * CONV0Flags  = (uint8_t *)CONV0;
	volatile uint8_t  * POOL0Flags  = (uint8_t *)POOL0;
	volatile uint8_t  * CONV1Flags  = (uint8_t *)CONV1;
	volatile uint8_t  * POOL1Flags  = (uint8_t *)POOL1;
	volatile uint8_t  * FC0Flags  = (uint8_t *)FC0;
	volatile uint8_t  * FC1Flags  = (uint8_t *)FC1;

	//StreamDma
	volatile uint8_t  * StrDmaFlags				= (uint8_t  *)(STREAMDMA_Flags);
	volatile uint64_t * StrDmaRdAddr			= (uint64_t *)(STREAMDMA_RdAddr);
	volatile uint64_t * StrDmaWrAddr			= (uint64_t *)(STREAMDMA_WrAddr);
	volatile uint32_t * StrDmaRdFrameSize		= (uint32_t *)(STREAMDMA_RdFrameSize);
	volatile uint8_t  * StrDmaNumRdFrames		= (uint8_t  *)(STREAMDMA_NumRdFrames);
	volatile uint8_t  * StrDmaRdFrameBuffSize	= (uint8_t  *)(STREAMDMA_RdFrameBufSize);
	volatile uint32_t * StrDmaWrFrameSize		= (uint32_t *)(STREAMDMA_WrFrameSize);
	volatile uint8_t  * StrDmaNumWrFrames		= (uint8_t  *)(STREAMDMA_NumWrFrames);
	volatile uint8_t  * StrDmaWrFrameBuffSize	= (uint8_t  *)(STREAMDMA_WrFrameBufSize);

	// Define DMA MMR
	volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA_Flags);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA_RdAddr);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA_WrAddr);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA_CopyLen);

	//Initialize DRAM-Stream DMA
	*StrDmaRdAddr = 0x90000000;
	*StrDmaRdFrameSize = 4096;
	*StrDmaNumRdFrames = 2;
	*StrDmaRdFrameBuffSize = 1;
	//Initialize Stream-DRAM DMA
	*StrDmaWrAddr = 0x9000000F;
	*StrDmaWrFrameSize = 336;
	*StrDmaNumWrFrames = 1;
	*StrDmaWrFrameBuffSize = 1;
	//Start Stream DMAs
	*StrDmaFlags = STR_DMA_INIT_RD | STR_DMA_INIT_WR;

	//Transfer Input Weights
	*DmaRdAddr  = 0x90000000;
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

	// //Start conv0
	// *CONV0Flags = DEV_INIT;
	// //Start Pool0
	// *POOL0Flags = DEV_INIT;
	// // Start conv1
	// *CONV1Flags = DEV_INIT;
	// // Start pool1
	// *POOL1Flags = DEV_INIT;
	// Start fc0
	*FC0Flags = DEV_INIT;
	// Start fc1
	*FC1Flags = DEV_INIT;

	// Wait for last ACC to finish out to avoid weird issues
	while ((*FC1Flags & DEV_INTR) != DEV_INTR);
	// Verify frame was written
	while ((*StrDmaFlags & STR_DMA_WR_RUNNING) == STR_DMA_WR_RUNNING);
	// while ((*StrDmaFlags & STR_DMA_RD_RUNNING) == STR_DMA_RD_RUNNING);

	return;
}