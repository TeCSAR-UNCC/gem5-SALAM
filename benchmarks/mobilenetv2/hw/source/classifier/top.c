#include "classifier_defines.h"

void top(uint64_t feat_rd_addr, uint64_t feat_wr_addr,
		 uint64_t weights, uint64_t quant) {

	// //Initialize Accelerators
	volatile uint8_t * Linear 		= (uint8_t *)CLASSIFIER_LINEAR;

	//Initialize DMAs
	//StreamDma
	volatile uint8_t  * StrDmaFlags				= (uint8_t  *)(CLASSIFIER_STREAM_DMA0_Flags);
	volatile uint64_t * StrDmaRdAddr			= (uint64_t *)(CLASSIFIER_STREAM_DMA0_RdAddr);
	volatile uint64_t * StrDmaWrAddr			= (uint64_t *)(CLASSIFIER_STREAM_DMA0_WrAddr);
	volatile uint32_t * StrDmaRdFrameSize		= (uint32_t *)(CLASSIFIER_STREAM_DMA0_RdFrameSize);
	volatile uint8_t  * StrDmaNumRdFrames		= (uint8_t  *)(CLASSIFIER_STREAM_DMA0_NumRdFrames);
	volatile uint8_t  * StrDmaRdFrameBuffSize	= (uint8_t  *)(CLASSIFIER_STREAM_DMA0_RdFrameBufSize);
	volatile uint32_t * StrDmaWrFrameSize		= (uint32_t *)(CLASSIFIER_STREAM_DMA0_WrFrameSize);
	volatile uint8_t  * StrDmaNumWrFrames		= (uint8_t  *)(CLASSIFIER_STREAM_DMA0_NumWrFrames);
	volatile uint8_t  * StrDmaWrFrameBuffSize	= (uint8_t  *)(CLASSIFIER_STREAM_DMA0_WrFrameBufSize);
	//MemDma
	volatile uint8_t  * MemDmaFlags				= (uint8_t  *)(CLASSIFIER_DMA_Flags);
	volatile uint64_t * MemDmaRdAddr			= (uint64_t *)(CLASSIFIER_DMA_RdAddr);
	volatile uint64_t * MemDmaWrAddr			= (uint64_t *)(CLASSIFIER_DMA_WrAddr);
	volatile uint32_t * MemDmaCopyLen			= (uint32_t *)(CLASSIFIER_DMA_CopyLen);

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
	*MemDmaWrAddr  = classifier_LinearFeats;
	*MemDmaCopyLen = INPUT_SIZE;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	
	//Start Weight Xfer
	*MemDmaRdAddr  = weights;
	*MemDmaWrAddr  = classifier_LinearWeights;
	*MemDmaCopyLen = WeightSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	
	// Start PW Conv QParams Xfer
	*MemDmaRdAddr  = quant + QParamSize*0;
	*MemDmaWrAddr  = classifier_PWBias;
	*MemDmaCopyLen = QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = quant + QParamSize*1;
	*MemDmaWrAddr  = classifier_PWIMultBias;
	*MemDmaCopyLen = QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = quant + QParamSize*2;
	*MemDmaWrAddr  = classifier_PWNShiftBias;
	*MemDmaCopyLen = QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = quant + QParamSize*3;
	*MemDmaWrAddr  = classifier_PWIMultOut;
	*MemDmaCopyLen = QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = quant + QParamSize*4;
	*MemDmaWrAddr  = classifier_PWNShiftOut;
	*MemDmaCopyLen = QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = quant + QParamSize*5;
	*MemDmaWrAddr  = classifier_PWWeightZP;
	*MemDmaCopyLen = QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	//Start Linear
	*Linear = 0x01;

	//Wait for all accelerators to finish before sending interrupt to CPU
	while ((*StrDmaFlags & STR_DMA_WR_RUNNING) == STR_DMA_WR_RUNNING);
	return;
}