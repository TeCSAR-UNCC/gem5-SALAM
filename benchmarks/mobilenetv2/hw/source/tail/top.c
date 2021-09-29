#include "tail_defines.h"

void top(uint64_t feat_rd_addr, uint64_t feat_wr_addr,
		 uint64_t pw_weights, uint64_t pw_quant) {

	//Initialize Accelerators
	volatile uint8_t * PWConv 		= (uint8_t *)TAIL_PWCONV;
	volatile uint8_t * Reshape		= (uint8_t *)TAIL_RESHAPE;
	volatile uint8_t * AvgPool 		= (uint8_t *)TAIL_AVGPOOL;

	//Initialize DMAs
	//StreamDma
	volatile uint8_t  * StrDmaFlags				= (uint8_t  *)(TAIL_STREAM_DMA0_Flags);
	volatile uint64_t * StrDmaRdAddr			= (uint64_t *)(TAIL_STREAM_DMA0_RdAddr);
	volatile uint64_t * StrDmaWrAddr			= (uint64_t *)(TAIL_STREAM_DMA0_WrAddr);
	volatile uint32_t * StrDmaRdFrameSize		= (uint32_t *)(TAIL_STREAM_DMA0_RdFrameSize);
	volatile uint8_t  * StrDmaNumRdFrames		= (uint8_t  *)(TAIL_STREAM_DMA0_NumRdFrames);
	volatile uint8_t  * StrDmaRdFrameBuffSize	= (uint8_t  *)(TAIL_STREAM_DMA0_RdFrameBufSize);
	volatile uint32_t * StrDmaWrFrameSize		= (uint32_t *)(TAIL_STREAM_DMA0_WrFrameSize);
	volatile uint8_t  * StrDmaNumWrFrames		= (uint8_t  *)(TAIL_STREAM_DMA0_NumWrFrames);
	volatile uint8_t  * StrDmaWrFrameBuffSize	= (uint8_t  *)(TAIL_STREAM_DMA0_WrFrameBufSize);
	//MemDma
	volatile uint8_t  * MemDmaFlags				= (uint8_t  *)(TAIL_DMA_Flags);
	volatile uint64_t * MemDmaRdAddr			= (uint64_t *)(TAIL_DMA_RdAddr);
	volatile uint64_t * MemDmaWrAddr			= (uint64_t *)(TAIL_DMA_WrAddr);
	volatile uint32_t * MemDmaCopyLen			= (uint32_t *)(TAIL_DMA_CopyLen);
	//Initialize DRAM-Stream DMA
	*StrDmaRdAddr = feat_rd_addr;
	*StrDmaRdFrameSize = INPUT_SIZE;
	*StrDmaNumRdFrames = 1;
	*StrDmaRdFrameBuffSize = 1;
	//Initialize Stream-DRAM DMA
	*StrDmaWrAddr = feat_wr_addr;
	*StrDmaWrFrameSize = OUTPUT_SIZE;
	*StrDmaNumWrFrames = 1;
	*StrDmaWrFrameBuffSize = 1;
	//Start Stream DMAs
	*StrDmaFlags = STR_DMA_INIT_RD | STR_DMA_INIT_WR;

	//Transfer Weights and Quantization Params from DRAM to SPMs
	//Start PW Conv Weight Xfer
	*MemDmaRdAddr  = pw_weights;
	*MemDmaWrAddr  = tail_PWConvWeights;
	*MemDmaCopyLen = PWWeightSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	// Start PW Conv QParams Xfer
	*MemDmaRdAddr  = pw_quant + PWQParamSize*0;
	*MemDmaWrAddr  = tail_PWBias;
	*MemDmaCopyLen = PWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw_quant + PWQParamSize*1;
	*MemDmaWrAddr  = tail_PWIMultBias;
	*MemDmaCopyLen = PWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw_quant + PWQParamSize*2;
	*MemDmaWrAddr  = tail_PWNShiftBias;
	*MemDmaCopyLen = PWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw_quant + PWQParamSize*3;
	*MemDmaWrAddr  = tail_PWIMultOut;
	*MemDmaCopyLen = PWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw_quant + PWQParamSize*4;
	*MemDmaWrAddr  = tail_PWNShiftOut;
	*MemDmaCopyLen = PWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw_quant + PWQParamSize*5;
	*MemDmaWrAddr  = tail_PWWeightZP;
	*MemDmaCopyLen = PWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	//Start PW Conv
	*PWConv = 0x01;
	//Start Reshape
	*Reshape = 0x01;
	//Start AVG Pool
	*AvgPool = 0x01;

	//Wait for all accelerators to finish before sending interrupt to CPU
	while ((*StrDmaFlags & STR_DMA_WR_RUNNING) == STR_DMA_WR_RUNNING);
	return;
}