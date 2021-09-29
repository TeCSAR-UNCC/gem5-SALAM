#include "head_defines.h"

void top(uint64_t img_rd_addr, uint64_t feat_wr_addr,
		 uint64_t conv_weights, uint64_t conv_quant,
		 uint64_t dw_weights, uint64_t dw_quant,
		 uint64_t pw_weights, uint64_t pw_quant) {

	//Initialize Accelerators
	volatile uint8_t * NormConv 	= (uint8_t *)HEAD_NORMALCONV;
	volatile uint8_t * DWConv 		= (uint8_t *)HEAD_DWCONV;
	volatile uint8_t * PWConv 		= (uint8_t *)HEAD_PWCONV;

	//Initialize DMAs
	//StreamDma
	volatile uint8_t  * StrDmaFlags				= (uint8_t  *)(HEAD_STREAM_DMA0_Flags);
	volatile uint64_t * StrDmaRdAddr			= (uint64_t *)(HEAD_STREAM_DMA0_RdAddr);
	volatile uint64_t * StrDmaWrAddr			= (uint64_t *)(HEAD_STREAM_DMA0_WrAddr);
	volatile uint32_t * StrDmaRdFrameSize		= (uint32_t *)(HEAD_STREAM_DMA0_RdFrameSize);
	volatile uint8_t  * StrDmaNumRdFrames		= (uint8_t  *)(HEAD_STREAM_DMA0_NumRdFrames);
	volatile uint8_t  * StrDmaRdFrameBuffSize	= (uint8_t  *)(HEAD_STREAM_DMA0_RdFrameBufSize);
	volatile uint32_t * StrDmaWrFrameSize		= (uint32_t *)(HEAD_STREAM_DMA0_WrFrameSize);
	volatile uint8_t  * StrDmaNumWrFrames		= (uint8_t  *)(HEAD_STREAM_DMA0_NumWrFrames);
	volatile uint8_t  * StrDmaWrFrameBuffSize	= (uint8_t  *)(HEAD_STREAM_DMA0_WrFrameBufSize);
	//MemDma
	volatile uint8_t  * MemDmaFlags				= (uint8_t  *)(HEAD_DMA_Flags);
	volatile uint64_t * MemDmaRdAddr			= (uint64_t *)(HEAD_DMA_RdAddr);
	volatile uint64_t * MemDmaWrAddr			= (uint64_t *)(HEAD_DMA_WrAddr);
	volatile uint32_t * MemDmaCopyLen			= (uint32_t *)(HEAD_DMA_CopyLen);
	//Initialize DRAM-Stream DMA
	*StrDmaRdAddr = img_rd_addr;
	*StrDmaRdFrameSize = ConvInputSize;
	*StrDmaNumRdFrames = 1;
	*StrDmaRdFrameBuffSize = 1;
	//Initialize Stream-DRAM DMA
	*StrDmaWrAddr = feat_wr_addr;
	*StrDmaWrFrameSize = PWOutputSize;
	*StrDmaNumWrFrames = 1;
	*StrDmaWrFrameBuffSize = 1;
	//Start Stream DMAs
	*StrDmaFlags = STR_DMA_INIT_RD | STR_DMA_INIT_WR;

	//Transfer Weights and Quantization Params from DRAM to SPMs
	
	//Start Norm Conv Weight Xfer
	*MemDmaRdAddr  = conv_weights;
	*MemDmaWrAddr  = head_NormalConvWeights;
	*MemDmaCopyLen = ConvWeightSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	// Start Normal Conv QParams Xfer
	*MemDmaRdAddr  = conv_quant + ConvQParamSize*0;
	*MemDmaWrAddr  = head_NormalConvBias;
	*MemDmaCopyLen = ConvQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = conv_quant + ConvQParamSize*1;
	*MemDmaWrAddr  = head_NormalConvIMultBias;
	*MemDmaCopyLen = ConvQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = conv_quant + ConvQParamSize*2;
	*MemDmaWrAddr  = head_NormalConvNShiftBias;
	*MemDmaCopyLen = ConvQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = conv_quant + ConvQParamSize*3;
	*MemDmaWrAddr  = head_NormalConvIMultOut;
	*MemDmaCopyLen = ConvQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = conv_quant + ConvQParamSize*4;
	*MemDmaWrAddr  = head_NormalConvNShiftOut;
	*MemDmaCopyLen = ConvQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = conv_quant + ConvQParamSize*5;
	*MemDmaWrAddr  = head_NormalConvWeightZP;
	*MemDmaCopyLen = ConvQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	//Start DW Conv Weight Xfer
	*MemDmaRdAddr  = dw_weights;
	*MemDmaWrAddr  = head_DWConvWeights;
	*MemDmaCopyLen = DWWeightSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	// Start DW Conv QParams Xfer
	*MemDmaRdAddr  = dw_quant + DWQParamSize*0;
	*MemDmaWrAddr  = head_DWConvBias;
	*MemDmaCopyLen = DWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = dw_quant + DWQParamSize*1;
	*MemDmaWrAddr  = head_DWConvIMultBias;
	*MemDmaCopyLen = DWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = dw_quant + DWQParamSize*2;
	*MemDmaWrAddr  = head_DWConvNShiftBias;
	*MemDmaCopyLen = DWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = dw_quant + DWQParamSize*3;
	*MemDmaWrAddr  = head_DWConvIMultOut;
	*MemDmaCopyLen = DWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = dw_quant + DWQParamSize*4;
	*MemDmaWrAddr  = head_DWConvNShiftOut;
	*MemDmaCopyLen = DWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = dw_quant + DWQParamSize*5;
	*MemDmaWrAddr  = head_DWConvWeightZP;
	*MemDmaCopyLen = DWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	//Start PW Conv Weight Xfer
	*MemDmaRdAddr  = pw_weights;
	*MemDmaWrAddr  = head_PWConvWeights;
	*MemDmaCopyLen = PWWeightSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	// Start PW Conv QParams Xfer
	*MemDmaRdAddr  = pw_quant + PWQParamSize*0;
	*MemDmaWrAddr  = head_PWConvBias;
	*MemDmaCopyLen = PWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw_quant + PWQParamSize*1;
	*MemDmaWrAddr  = head_PWConvIMultBias;
	*MemDmaCopyLen = PWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw_quant + PWQParamSize*2;
	*MemDmaWrAddr  = head_PWConvNShiftBias;
	*MemDmaCopyLen = PWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw_quant + PWQParamSize*3;
	*MemDmaWrAddr  = head_PWConvIMultOut;
	*MemDmaCopyLen = PWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw_quant + PWQParamSize*4;
	*MemDmaWrAddr  = head_PWConvNShiftOut;
	*MemDmaCopyLen = PWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw_quant + PWQParamSize*5;
	*MemDmaWrAddr  = head_PWConvWeightZP;
	*MemDmaCopyLen = PWQParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	// Start Norm Conv
	*NormConv = 0x01;
	//Start DW Conv
	*DWConv = 0x01;
	//Start PW Conv
	*PWConv = 0x01;

	//Wait for all accelerators to finish before sending interrupt to CPU
	while ((*StrDmaFlags & STR_DMA_WR_RUNNING) == STR_DMA_WR_RUNNING);
	return;
}