#include "classifier_defines.h"

void top(uint64_t feat_rd_addr, uint64_t feat_wr_addr,
		 uint64_t weights, uint64_t quant) {

	// //Initialize Accelerators
	volatile uint8_t * Linear 		= (uint8_t *)LINEAR_MMR;
	//Start Linear
	*Linear = 0x01;

	//Initialize DMAs
	//StreamDma
	volatile uint8_t  * StrDmaFlags				= (uint8_t  *)(STREAM_DMA_MMR);
	volatile uint64_t * StrDmaRdAddr			= (uint64_t *)(STREAM_DMA_MMR+4);
	volatile uint64_t * StrDmaWrAddr			= (uint64_t *)(STREAM_DMA_MMR+12);
	volatile uint32_t * StrDmaRdFrameSize		= (uint32_t *)(STREAM_DMA_MMR+20);
	volatile uint8_t  * StrDmaNumRdFrames		= (uint8_t  *)(STREAM_DMA_MMR+24);
	volatile uint8_t  * StrDmaRdFrameBuffSize	= (uint8_t  *)(STREAM_DMA_MMR+25);
	volatile uint32_t * StrDmaWrFrameSize		= (uint32_t *)(STREAM_DMA_MMR+26);
	volatile uint8_t  * StrDmaNumWrFrames		= (uint8_t  *)(STREAM_DMA_MMR+30);
	volatile uint8_t  * StrDmaWrFrameBuffSize	= (uint8_t  *)(STREAM_DMA_MMR+31);
	//MemDma
	volatile uint8_t  * MemDmaFlags				= (uint8_t  *)(CLUSTER_DMA_MMR);
	volatile uint64_t * MemDmaRdAddr			= (uint64_t *)(CLUSTER_DMA_MMR+1);
	volatile uint64_t * MemDmaWrAddr			= (uint64_t *)(CLUSTER_DMA_MMR+9);
	volatile uint32_t * MemDmaCopyLen			= (uint32_t *)(CLUSTER_DMA_MMR+17);

	//Initialize Stream-DRAM DMA
	*StrDmaWrAddr = feat_wr_addr;
	*StrDmaWrFrameSize = OUTPUT_SIZE;
	*StrDmaNumWrFrames = 1;
	*StrDmaWrFrameBuffSize = 1;
	//Start Stream DMA
	*StrDmaFlags = STR_DMA_INIT_WR;

	//Transfer Features, Weights, and Quantization Params from DRAM to SPMs
	//Start Feature Xfer
	*MemDmaRdAddr  = feat_rd_addr;
	*MemDmaWrAddr  = FeatSPM;
	*MemDmaCopyLen = INPUT_SIZE;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	//Start Weight Xfer
	*MemDmaRdAddr  = weights;
	*MemDmaWrAddr  = WeightSPM;
	*MemDmaCopyLen = WeightSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	//Start QParams Xfer
	*MemDmaRdAddr  = quant;
	*MemDmaWrAddr  = Bias;
	*MemDmaCopyLen = QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;

	//Wait for all accelerators to finish before sending interrupt to CPU
	while ((*StrDmaFlags & STR_DMA_WR_INTR) != STR_DMA_WR_INTR);
	return;
}