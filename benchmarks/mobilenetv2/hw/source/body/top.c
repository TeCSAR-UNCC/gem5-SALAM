#include "body_defines.h"

void top(uint8_t stage, uint64_t feat_rd_addr,
		 uint64_t res_rd_addr, uint64_t feat_wr_addr,
		 uint64_t pw0_weights, uint64_t pw0_quant,
		 uint64_t dw0_weights, uint64_t dw0_quant,
		 uint64_t pw1_weights, uint64_t pw1_quant) {

	uint32_t InputSize;
	uint32_t OutputSize;
	uint8_t  resEnable;

	//Initialize Accelerators
	volatile uint8_t	* ResFlags		= (uint8_t	*)(RES_MMR);
	volatile uint8_t	* ResEnable		= (uint8_t	*)(RES_MMR+1);
	volatile uint32_t	* ResLength		= (uint32_t *)(RES_MMR+2);
	volatile uint8_t	* PW0Flags 		= (uint8_t	*)(PW0_MMR);
	volatile uint32_t	* PW0OSize 		= (uint32_t *)(PW0_MMR+1);
	volatile uint32_t	* PW0ISize 		= (uint32_t	*)(PW0_MMR+5);
	volatile uint32_t	* PW0ICSize 	= (uint32_t	*)(PW0_MMR+9);
	volatile uint32_t	* PW0OCSize 	= (uint32_t	*)(PW0_MMR+13);
	volatile uint8_t	* PW0BiasZP 	= (uint8_t	*)(PW0_MMR+17);
	volatile uint8_t	* PW0InputZP 	= (uint8_t	*)(PW0_MMR+18);
	volatile uint8_t	* PW0OutputZP 	= (uint8_t	*)(PW0_MMR+19);
	volatile uint8_t	* DW0Flags 		= (uint8_t	*)(DW0_MMR);
	volatile uint32_t	* DW0OSize 		= (uint32_t	*)(DW0_MMR+1);
	volatile uint32_t	* DW0ISize 		= (uint32_t	*)(DW0_MMR+5);
	volatile uint32_t	* DW0ICSize 	= (uint32_t	*)(DW0_MMR+9);
	volatile uint32_t	* DW0OCSize 	= (uint32_t	*)(DW0_MMR+13);
	volatile uint32_t	* DW0Stride 	= (uint32_t	*)(DW0_MMR+17);
	volatile uint8_t	* DW0BiasZP 	= (uint8_t	*)(DW0_MMR+21);
	volatile uint8_t	* DW0InputZP 	= (uint8_t	*)(DW0_MMR+22);
	volatile uint8_t	* DW0OutputZP 	= (uint8_t	*)(DW0_MMR+23);
	volatile uint8_t	* PW1Flags 		= (uint8_t	*)(PW1_MMR);
	volatile uint32_t	* PW1OSize 		= (uint32_t	*)(PW1_MMR+1);
	volatile uint32_t	* PW1ISize 		= (uint32_t	*)(PW1_MMR+5);
	volatile uint32_t	* PW1ICSize 	= (uint32_t	*)(PW1_MMR+9);
	volatile uint32_t	* PW1OCSize 	= (uint32_t	*)(PW1_MMR+13);
	volatile uint8_t	* PW1BiasZP 	= (uint8_t	*)(PW1_MMR+17);
	volatile uint8_t	* PW1InputZP 	= (uint8_t	*)(PW1_MMR+18);
	volatile uint8_t	* PW1OutputZP 	= (uint8_t	*)(PW1_MMR+19);

	InputSize    = PW0_0_I_SIZE * PW0_0_I_SIZE * PW0_0_IC_SIZE;
	OutputSize   = PW1_0_O_SIZE * PW1_0_O_SIZE * PW1_0_OC_SIZE;
	*ResLength   = InputSize;
	resEnable    = RES_0_ENABLE;
	*PW0OSize    = PW0_0_O_SIZE;
	*PW0ISize    = PW0_0_I_SIZE;
	*PW0ICSize   = PW0_0_IC_SIZE;
	*PW0OCSize   = PW0_0_OC_SIZE;
	*PW0BiasZP   = PW0_0_BIAS_ZP;
	*PW0InputZP  = PW0_0_IN_ZP;
	*PW0OutputZP = PW0_0_OUT_ZP;
	*DW0OSize    = DW0_0_O_SIZE;
	*DW0ISize    = DW0_0_I_SIZE;
	*DW0ICSize   = DW0_0_IC_SIZE;
	*DW0OCSize   = DW0_0_OC_SIZE;
	*DW0Stride   = DW0_0_STRIDE;
	*DW0BiasZP   = DW0_0_BIAS_ZP;
	*DW0InputZP  = DW0_0_IN_ZP;
	*DW0OutputZP = DW0_0_OUT_ZP;
	*PW1OSize    = PW1_0_O_SIZE;
	*PW1ISize    = PW1_0_I_SIZE;
	*PW1ICSize   = PW1_0_IC_SIZE;
	*PW1OCSize   = PW1_0_OC_SIZE;
	*PW1BiasZP   = PW1_0_BIAS_ZP;
	*PW1InputZP  = PW1_0_IN_ZP;
	*PW1OutputZP = PW1_0_OUT_ZP;

	//Initialize DMAs
	//StreamDma0
	volatile uint8_t  * StrDma0Flags			= (uint8_t  *)(STREAM_DMA0_MMR);
	volatile uint64_t * StrDma0RdAddr			= (uint64_t *)(STREAM_DMA0_MMR+4);
	volatile uint64_t * StrDma0WrAddr			= (uint64_t *)(STREAM_DMA0_MMR+12);
	volatile uint32_t * StrDma0RdFrameSize		= (uint32_t *)(STREAM_DMA0_MMR+20);
	volatile uint8_t  * StrDma0NumRdFrames		= (uint8_t  *)(STREAM_DMA0_MMR+24);
	volatile uint8_t  * StrDma0RdFrameBuffSize	= (uint8_t  *)(STREAM_DMA0_MMR+25);
	volatile uint32_t * StrDma0WrFrameSize		= (uint32_t *)(STREAM_DMA0_MMR+26);
	volatile uint8_t  * StrDma0NumWrFrames		= (uint8_t  *)(STREAM_DMA0_MMR+30);
	volatile uint8_t  * StrDma0WrFrameBuffSize	= (uint8_t  *)(STREAM_DMA0_MMR+31);
	//StreamDma1
	volatile uint8_t  * StrDma1Flags			= (uint8_t  *)(STREAM_DMA1_MMR);
	volatile uint64_t * StrDma1RdAddr			= (uint64_t *)(STREAM_DMA1_MMR+4);
	volatile uint64_t * StrDma1WrAddr			= (uint64_t *)(STREAM_DMA1_MMR+12);
	volatile uint32_t * StrDma1RdFrameSize		= (uint32_t *)(STREAM_DMA1_MMR+20);
	volatile uint8_t  * StrDma1NumRdFrames		= (uint8_t  *)(STREAM_DMA1_MMR+24);
	volatile uint8_t  * StrDma1RdFrameBuffSize	= (uint8_t  *)(STREAM_DMA1_MMR+25);
	volatile uint32_t * StrDma1WrFrameSize		= (uint32_t *)(STREAM_DMA1_MMR+26);
	volatile uint8_t  * StrDma1NumWrFrames		= (uint8_t  *)(STREAM_DMA1_MMR+30);
	volatile uint8_t  * StrDma1WrFrameBuffSize	= (uint8_t  *)(STREAM_DMA1_MMR+31);
	//MemDma
	volatile uint8_t  * MemDmaFlags				= (uint8_t  *)(CLUSTER_DMA_MMR);
	volatile uint64_t * MemDmaRdAddr			= (uint64_t *)(CLUSTER_DMA_MMR+1);
	volatile uint64_t * MemDmaWrAddr			= (uint64_t *)(CLUSTER_DMA_MMR+9);
	volatile uint32_t * MemDmaCopyLen			= (uint32_t *)(CLUSTER_DMA_MMR+17);
	//Initialize DRAM-Stream DMA
	*StrDma0RdAddr = 0x8f000000;
	*StrDma0RdFrameSize = InputSize;
	*StrDma0NumRdFrames = 1;
	*StrDma0RdFrameBuffSize = 1;
	//Initialize Stream-DRAM DMA
	*StrDma0WrAddr = 0x9f000000;
	*StrDma0WrFrameSize = 100;
	*StrDma0NumWrFrames = 1;
	*StrDma0WrFrameBuffSize = 1;
	//Initialize Res DMA
	*StrDma1RdAddr = 0x8f100000;
	*StrDma1RdFrameSize = InputSize;
	*StrDma1NumRdFrames = 1;
	*StrDma1RdFrameBuffSize = 1;
	//Start Stream DMAs
	*StrDma0Flags = STR_DMA_INIT_RD | STR_DMA_INIT_WR;
	if (resEnable == 1) *StrDma1Flags = STR_DMA_INIT_RD;

	//Transfer Weights and Quantization Params from DRAM to SPMs
	//Start PW0 Conv Weight Xfer
	*MemDmaRdAddr  = 0x8f000000;
	*MemDmaWrAddr  = PW0Weights;
	*MemDmaCopyLen = PW0WeightSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	//Start PW0 Conv QParams Xfer
	*MemDmaRdAddr  = 0x8f000000;
	*MemDmaWrAddr  = PW0Bias;
	*MemDmaCopyLen = PW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	//Start DW0 Conv Weight Xfer
	*MemDmaRdAddr  = 0x8f000000;
	*MemDmaWrAddr  = DW0Weights;
	*MemDmaCopyLen = DW0WeightSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	//Start DW0 Conv QParams Xfer
	*MemDmaRdAddr  = 0x8f000000;
	*MemDmaWrAddr  = DW0Bias;
	*MemDmaCopyLen = DW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	//Start PW1 Conv Weight Xfer
	*MemDmaRdAddr  = 0x8f000000;
	*MemDmaWrAddr  = PW1Weights;
	*MemDmaCopyLen = PW1WeightSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	//Start PW1 Conv QParams Xfer
	*MemDmaRdAddr  = 0x8f000000;
	*MemDmaWrAddr  = PW1Bias;
	*MemDmaCopyLen = PW1QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	//Start Res
	*ResEnable = resEnable;
	*ResFlags = 0x01;
	//Start PW Conv 0
	*PW0Flags = 0x01;
	//Start DW Conv
	*DW0Flags = 0x01;
	//Start PW Conv 1
	*PW1Flags = 0x01;

	//Wait for all accelerators to finish before sending interrupt to CPU
	while ((*StrDma0Flags & STR_DMA_WR_RUNNING) == STR_DMA_WR_RUNNING);
	return;
}