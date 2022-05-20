#include "body_defines.h"

void top(uint64_t feat_rd_addr,
		 uint64_t res_rd_addr, uint64_t feat_wr_addr,
		 uint64_t pw0_weights, uint64_t pw0_quant,
		 uint64_t dw0_weights, uint64_t dw0_quant,
		 uint64_t pw1_weights, uint64_t pw1_quant,
		 uint64_t stage) {

	uint32_t InputSize;
	uint32_t OutputSize;
	uint8_t  resEnable;

	//Initialize Accelerators
	// Residual Parameters
	volatile uint8_t	* ResFlags		= (uint8_t	*)(BODY_RESIDUAL);
	volatile uint64_t	* ResEnable		= (uint64_t	*)(BODY_RESIDUAL+1);
	volatile uint64_t	* ResLength		= (uint64_t *)(BODY_RESIDUAL+9);
	// Pointwise 0 Parameters
	volatile uint8_t	* PW0Flags 		= (uint8_t	*)(BODY_PWCONV0);
	volatile uint64_t	* PW0OSize 		= (uint64_t *)(BODY_PWCONV0+1);
	volatile uint64_t	* PW0ISize 		= (uint64_t	*)(BODY_PWCONV0+9);
	volatile uint64_t	* PW0OCSize 	= (uint64_t	*)(BODY_PWCONV0+17);
	volatile uint64_t	* PW0ICSize 	= (uint64_t	*)(BODY_PWCONV0+25);
	volatile uint64_t	* PW0BiasZP 	= (uint64_t	*)(BODY_PWCONV0+33);
	volatile uint64_t	* PW0InputZP 	= (uint64_t	*)(BODY_PWCONV0+41);
	volatile uint64_t	* PW0OutputZP 	= (uint64_t	*)(BODY_PWCONV0+49);
	// Depthwise Parameters
	volatile uint8_t	* DW0Flags 		= (uint8_t	*)(BODY_DWCONV);
	volatile uint64_t	* DW0OSize 		= (uint64_t	*)(BODY_DWCONV+1);
	volatile uint64_t	* DW0ISize 		= (uint64_t	*)(BODY_DWCONV+9);
	volatile uint64_t	* DW0OCSize 	= (uint64_t	*)(BODY_DWCONV+17);
	volatile uint64_t	* DW0ICSize 	= (uint64_t	*)(BODY_DWCONV+25);
	volatile uint64_t	* DW0Stride 	= (uint64_t	*)(BODY_DWCONV+33);
	volatile uint64_t	* DW0BiasZP 	= (uint64_t	*)(BODY_DWCONV+41);
	volatile uint64_t	* DW0InputZP 	= (uint64_t	*)(BODY_DWCONV+49);
	volatile uint64_t	* DW0OutputZP 	= (uint64_t	*)(BODY_DWCONV+57);
	// Pointwise 1 Parameters
	volatile uint8_t	* PW1Flags 		= (uint8_t	*)(BODY_PWCONV1);
	volatile uint64_t	* PW1OSize 		= (uint64_t	*)(BODY_PWCONV1+1);
	volatile uint64_t	* PW1ISize 		= (uint64_t	*)(BODY_PWCONV1+9);
	volatile uint64_t	* PW1OCSize 	= (uint64_t	*)(BODY_PWCONV1+17);
	volatile uint64_t	* PW1ICSize 	= (uint64_t	*)(BODY_PWCONV1+25);	
	volatile uint64_t	* PW1BiasZP 	= (uint64_t	*)(BODY_PWCONV1+33);
	volatile uint64_t	* PW1InputZP 	= (uint64_t	*)(BODY_PWCONV1+41);
	volatile uint64_t	* PW1OutputZP 	= (uint64_t	*)(BODY_PWCONV1+49);

	//Initialize DMAs
	//StreamDma0
	volatile uint8_t  * StrDma0Flags			= (uint8_t  *)(BODY_STREAM_DMA0_Flags);
	volatile uint64_t * StrDma0RdAddr			= (uint64_t *)(BODY_STREAM_DMA0_RdAddr);
	volatile uint64_t * StrDma0WrAddr			= (uint64_t *)(BODY_STREAM_DMA0_WrAddr);
	volatile uint32_t * StrDma0RdFrameSize		= (uint32_t *)(BODY_STREAM_DMA0_RdFrameSize);
	volatile uint8_t  * StrDma0NumRdFrames		= (uint8_t  *)(BODY_STREAM_DMA0_NumRdFrames);
	volatile uint8_t  * StrDma0RdFrameBuffSize	= (uint8_t  *)(BODY_STREAM_DMA0_RdFrameBufSize);
	volatile uint32_t * StrDma0WrFrameSize		= (uint32_t *)(BODY_STREAM_DMA0_WrFrameSize);
	volatile uint8_t  * StrDma0NumWrFrames		= (uint8_t  *)(BODY_STREAM_DMA0_NumWrFrames);
	volatile uint8_t  * StrDma0WrFrameBuffSize	= (uint8_t  *)(BODY_STREAM_DMA0_WrFrameBufSize);
	//StreamDma1
	volatile uint8_t  * StrDma1Flags			= (uint8_t  *)(BODY_STREAM_DMA1_Flags);
	volatile uint64_t * StrDma1RdAddr			= (uint64_t *)(BODY_STREAM_DMA1_RdAddr);
	volatile uint64_t * StrDma1WrAddr			= (uint64_t *)(BODY_STREAM_DMA1_WrAddr);
	volatile uint32_t * StrDma1RdFrameSize		= (uint32_t *)(BODY_STREAM_DMA1_RdFrameSize);
	volatile uint8_t  * StrDma1NumRdFrames		= (uint8_t  *)(BODY_STREAM_DMA1_NumRdFrames);
	volatile uint8_t  * StrDma1RdFrameBuffSize	= (uint8_t  *)(BODY_STREAM_DMA1_RdFrameBufSize);
	volatile uint32_t * StrDma1WrFrameSize		= (uint32_t *)(BODY_STREAM_DMA1_WrFrameSize);
	volatile uint8_t  * StrDma1NumWrFrames		= (uint8_t  *)(BODY_STREAM_DMA1_NumWrFrames);
	volatile uint8_t  * StrDma1WrFrameBuffSize	= (uint8_t  *)(BODY_STREAM_DMA1_WrFrameBufSize);
	//MemDma
	volatile uint8_t  * MemDmaFlags				= (uint8_t  *)(BODY_DMA_Flags);
	volatile uint64_t * MemDmaRdAddr			= (uint64_t *)(BODY_DMA_RdAddr);
	volatile uint64_t * MemDmaWrAddr			= (uint64_t *)(BODY_DMA_WrAddr);
	volatile uint32_t * MemDmaCopyLen			= (uint32_t *)(BODY_DMA_CopyLen);

switch(stage) {
		case 1:
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
			
			break;
		case 2:
			InputSize    = PW0_1_I_SIZE * PW0_1_I_SIZE * PW0_1_IC_SIZE;
			OutputSize   = PW1_1_O_SIZE * PW1_1_O_SIZE * PW1_1_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_1_ENABLE;
			*PW0OSize    = PW0_1_O_SIZE;
			*PW0ISize    = PW0_1_I_SIZE;
			*PW0ICSize   = PW0_1_IC_SIZE;
			*PW0OCSize   = PW0_1_OC_SIZE;
			*PW0BiasZP   = PW0_1_BIAS_ZP;
			*PW0InputZP  = PW0_1_IN_ZP;
			*PW0OutputZP = PW0_1_OUT_ZP;
			*DW0OSize    = DW0_1_O_SIZE;
			*DW0ISize    = DW0_1_I_SIZE;
			*DW0ICSize   = DW0_1_IC_SIZE;
			*DW0OCSize   = DW0_1_OC_SIZE;
			*DW0Stride   = DW0_1_STRIDE;
			*DW0BiasZP   = DW0_1_BIAS_ZP;
			*DW0InputZP  = DW0_1_IN_ZP;
			*DW0OutputZP = DW0_1_OUT_ZP;
			*PW1OSize    = PW1_1_O_SIZE;
			*PW1ISize    = PW1_1_I_SIZE;
			*PW1ICSize   = PW1_1_IC_SIZE;
			*PW1OCSize   = PW1_1_OC_SIZE;
			*PW1BiasZP   = PW1_1_BIAS_ZP;
			*PW1InputZP  = PW1_1_IN_ZP;
			*PW1OutputZP = PW1_1_OUT_ZP;
			break;
		case 3:
			InputSize    = PW0_2_I_SIZE * PW0_2_I_SIZE * PW0_2_IC_SIZE;
			OutputSize   = PW1_2_O_SIZE * PW1_2_O_SIZE * PW1_2_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_2_ENABLE;
			*PW0OSize    = PW0_2_O_SIZE;
			*PW0ISize    = PW0_2_I_SIZE;
			*PW0ICSize   = PW0_2_IC_SIZE;
			*PW0OCSize   = PW0_2_OC_SIZE;
			*PW0BiasZP   = PW0_2_BIAS_ZP;
			*PW0InputZP  = PW0_2_IN_ZP;
			*PW0OutputZP = PW0_2_OUT_ZP;
			*DW0OSize    = DW0_2_O_SIZE;
			*DW0ISize    = DW0_2_I_SIZE;
			*DW0ICSize   = DW0_2_IC_SIZE;
			*DW0OCSize   = DW0_2_OC_SIZE;
			*DW0Stride   = DW0_2_STRIDE;
			*DW0BiasZP   = DW0_2_BIAS_ZP;
			*DW0InputZP  = DW0_2_IN_ZP;
			*DW0OutputZP = DW0_2_OUT_ZP;
			*PW1OSize    = PW1_2_O_SIZE;
			*PW1ISize    = PW1_2_I_SIZE;
			*PW1ICSize   = PW1_2_IC_SIZE;
			*PW1OCSize   = PW1_2_OC_SIZE;
			*PW1BiasZP   = PW1_2_BIAS_ZP;
			*PW1InputZP  = PW1_2_IN_ZP;
			*PW1OutputZP = PW1_2_OUT_ZP;
			break;
		case 4:
			InputSize    = PW0_3_I_SIZE * PW0_3_I_SIZE * PW0_3_IC_SIZE;
			OutputSize   = PW1_3_O_SIZE * PW1_3_O_SIZE * PW1_3_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_3_ENABLE;
			*PW0OSize    = PW0_3_O_SIZE;
			*PW0ISize    = PW0_3_I_SIZE;
			*PW0ICSize   = PW0_3_IC_SIZE;
			*PW0OCSize   = PW0_3_OC_SIZE;
			*PW0BiasZP   = PW0_3_BIAS_ZP;
			*PW0InputZP  = PW0_3_IN_ZP;
			*PW0OutputZP = PW0_3_OUT_ZP;
			*DW0OSize    = DW0_3_O_SIZE;
			*DW0ISize    = DW0_3_I_SIZE;
			*DW0ICSize   = DW0_3_IC_SIZE;
			*DW0OCSize   = DW0_3_OC_SIZE;
			*DW0Stride   = DW0_3_STRIDE;
			*DW0BiasZP   = DW0_3_BIAS_ZP;
			*DW0InputZP  = DW0_3_IN_ZP;
			*DW0OutputZP = DW0_3_OUT_ZP;
			*PW1OSize    = PW1_3_O_SIZE;
			*PW1ISize    = PW1_3_I_SIZE;
			*PW1ICSize   = PW1_3_IC_SIZE;
			*PW1OCSize   = PW1_3_OC_SIZE;
			*PW1BiasZP   = PW1_3_BIAS_ZP;
			*PW1InputZP  = PW1_3_IN_ZP;
			*PW1OutputZP = PW1_3_OUT_ZP;
			break;
		case 5:
			InputSize    = PW0_4_I_SIZE * PW0_4_I_SIZE * PW0_4_IC_SIZE;
			OutputSize   = PW1_4_O_SIZE * PW1_4_O_SIZE * PW1_4_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_4_ENABLE;
			*PW0OSize    = PW0_4_O_SIZE;
			*PW0ISize    = PW0_4_I_SIZE;
			*PW0ICSize   = PW0_4_IC_SIZE;
			*PW0OCSize   = PW0_4_OC_SIZE;
			*PW0BiasZP   = PW0_4_BIAS_ZP;
			*PW0InputZP  = PW0_4_IN_ZP;
			*PW0OutputZP = PW0_4_OUT_ZP;
			*DW0OSize    = DW0_4_O_SIZE;
			*DW0ISize    = DW0_4_I_SIZE;
			*DW0ICSize   = DW0_4_IC_SIZE;
			*DW0OCSize   = DW0_4_OC_SIZE;
			*DW0Stride   = DW0_4_STRIDE;
			*DW0BiasZP   = DW0_4_BIAS_ZP;
			*DW0InputZP  = DW0_4_IN_ZP;
			*DW0OutputZP = DW0_4_OUT_ZP;
			*PW1OSize    = PW1_4_O_SIZE;
			*PW1ISize    = PW1_4_I_SIZE;
			*PW1ICSize   = PW1_4_IC_SIZE;
			*PW1OCSize   = PW1_4_OC_SIZE;
			*PW1BiasZP   = PW1_4_BIAS_ZP;
			*PW1InputZP  = PW1_4_IN_ZP;
			*PW1OutputZP = PW1_4_OUT_ZP;
			break;
		case 6:
			InputSize    = PW0_5_I_SIZE * PW0_5_I_SIZE * PW0_5_IC_SIZE;
			OutputSize   = PW1_5_O_SIZE * PW1_5_O_SIZE * PW1_5_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_5_ENABLE;
			*PW0OSize    = PW0_5_O_SIZE;
			*PW0ISize    = PW0_5_I_SIZE;
			*PW0ICSize   = PW0_5_IC_SIZE;
			*PW0OCSize   = PW0_5_OC_SIZE;
			*PW0BiasZP   = PW0_5_BIAS_ZP;
			*PW0InputZP  = PW0_5_IN_ZP;
			*PW0OutputZP = PW0_5_OUT_ZP;
			*DW0OSize    = DW0_5_O_SIZE;
			*DW0ISize    = DW0_5_I_SIZE;
			*DW0ICSize   = DW0_5_IC_SIZE;
			*DW0OCSize   = DW0_5_OC_SIZE;
			*DW0Stride   = DW0_5_STRIDE;
			*DW0BiasZP   = DW0_5_BIAS_ZP;
			*DW0InputZP  = DW0_5_IN_ZP;
			*DW0OutputZP = DW0_5_OUT_ZP;
			*PW1OSize    = PW1_5_O_SIZE;
			*PW1ISize    = PW1_5_I_SIZE;
			*PW1ICSize   = PW1_5_IC_SIZE;
			*PW1OCSize   = PW1_5_OC_SIZE;
			*PW1BiasZP   = PW1_5_BIAS_ZP;
			*PW1InputZP  = PW1_5_IN_ZP;
			*PW1OutputZP = PW1_5_OUT_ZP;
			break;
		case 7:
			InputSize    = PW0_6_I_SIZE * PW0_6_I_SIZE * PW0_6_IC_SIZE;
			OutputSize   = PW1_6_O_SIZE * PW1_6_O_SIZE * PW1_6_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_6_ENABLE;
			*PW0OSize    = PW0_6_O_SIZE;
			*PW0ISize    = PW0_6_I_SIZE;
			*PW0ICSize   = PW0_6_IC_SIZE;
			*PW0OCSize   = PW0_6_OC_SIZE;
			*PW0BiasZP   = PW0_6_BIAS_ZP;
			*PW0InputZP  = PW0_6_IN_ZP;
			*PW0OutputZP = PW0_6_OUT_ZP;
			*DW0OSize    = DW0_6_O_SIZE;
			*DW0ISize    = DW0_6_I_SIZE;
			*DW0ICSize   = DW0_6_IC_SIZE;
			*DW0OCSize   = DW0_6_OC_SIZE;
			*DW0Stride   = DW0_6_STRIDE;
			*DW0BiasZP   = DW0_6_BIAS_ZP;
			*DW0InputZP  = DW0_6_IN_ZP;
			*DW0OutputZP = DW0_6_OUT_ZP;
			*PW1OSize    = PW1_6_O_SIZE;
			*PW1ISize    = PW1_6_I_SIZE;
			*PW1ICSize   = PW1_6_IC_SIZE;
			*PW1OCSize   = PW1_6_OC_SIZE;
			*PW1BiasZP   = PW1_6_BIAS_ZP;
			*PW1InputZP  = PW1_6_IN_ZP;
			*PW1OutputZP = PW1_6_OUT_ZP;
			break;
		case 8:
			InputSize    = PW0_7_I_SIZE * PW0_7_I_SIZE * PW0_7_IC_SIZE;
			OutputSize   = PW1_7_O_SIZE * PW1_7_O_SIZE * PW1_7_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_7_ENABLE;
			*PW0OSize    = PW0_7_O_SIZE;
			*PW0ISize    = PW0_7_I_SIZE;
			*PW0ICSize   = PW0_7_IC_SIZE;
			*PW0OCSize   = PW0_7_OC_SIZE;
			*PW0BiasZP   = PW0_7_BIAS_ZP;
			*PW0InputZP  = PW0_7_IN_ZP;
			*PW0OutputZP = PW0_7_OUT_ZP;
			*DW0OSize    = DW0_7_O_SIZE;
			*DW0ISize    = DW0_7_I_SIZE;
			*DW0ICSize   = DW0_7_IC_SIZE;
			*DW0OCSize   = DW0_7_OC_SIZE;
			*DW0Stride   = DW0_7_STRIDE;
			*DW0BiasZP   = DW0_7_BIAS_ZP;
			*DW0InputZP  = DW0_7_IN_ZP;
			*DW0OutputZP = DW0_7_OUT_ZP;
			*PW1OSize    = PW1_7_O_SIZE;
			*PW1ISize    = PW1_7_I_SIZE;
			*PW1ICSize   = PW1_7_IC_SIZE;
			*PW1OCSize   = PW1_7_OC_SIZE;
			*PW1BiasZP   = PW1_7_BIAS_ZP;
			*PW1InputZP  = PW1_7_IN_ZP;
			*PW1OutputZP = PW1_7_OUT_ZP;
			break;
		case 9:
			InputSize    = PW0_8_I_SIZE * PW0_8_I_SIZE * PW0_8_IC_SIZE;
			OutputSize   = PW1_8_O_SIZE * PW1_8_O_SIZE * PW1_8_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_8_ENABLE;
			*PW0OSize    = PW0_8_O_SIZE;
			*PW0ISize    = PW0_8_I_SIZE;
			*PW0ICSize   = PW0_8_IC_SIZE;
			*PW0OCSize   = PW0_8_OC_SIZE;
			*PW0BiasZP   = PW0_8_BIAS_ZP;
			*PW0InputZP  = PW0_8_IN_ZP;
			*PW0OutputZP = PW0_8_OUT_ZP;
			*DW0OSize    = DW0_8_O_SIZE;
			*DW0ISize    = DW0_8_I_SIZE;
			*DW0ICSize   = DW0_8_IC_SIZE;
			*DW0OCSize   = DW0_8_OC_SIZE;
			*DW0Stride   = DW0_8_STRIDE;
			*DW0BiasZP   = DW0_8_BIAS_ZP;
			*DW0InputZP  = DW0_8_IN_ZP;
			*DW0OutputZP = DW0_8_OUT_ZP;
			*PW1OSize    = PW1_8_O_SIZE;
			*PW1ISize    = PW1_8_I_SIZE;
			*PW1ICSize   = PW1_8_IC_SIZE;
			*PW1OCSize   = PW1_8_OC_SIZE;
			*PW1BiasZP   = PW1_8_BIAS_ZP;
			*PW1InputZP  = PW1_8_IN_ZP;
			*PW1OutputZP = PW1_8_OUT_ZP;
			break;
		case 10:
			InputSize    = PW0_9_I_SIZE * PW0_9_I_SIZE * PW0_9_IC_SIZE;
			OutputSize   = PW1_9_O_SIZE * PW1_9_O_SIZE * PW1_9_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_9_ENABLE;
			*PW0OSize    = PW0_9_O_SIZE;
			*PW0ISize    = PW0_9_I_SIZE;
			*PW0ICSize   = PW0_9_IC_SIZE;
			*PW0OCSize   = PW0_9_OC_SIZE;
			*PW0BiasZP   = PW0_9_BIAS_ZP;
			*PW0InputZP  = PW0_9_IN_ZP;
			*PW0OutputZP = PW0_9_OUT_ZP;
			*DW0OSize    = DW0_9_O_SIZE;
			*DW0ISize    = DW0_9_I_SIZE;
			*DW0ICSize   = DW0_9_IC_SIZE;
			*DW0OCSize   = DW0_9_OC_SIZE;
			*DW0Stride   = DW0_9_STRIDE;
			*DW0BiasZP   = DW0_9_BIAS_ZP;
			*DW0InputZP  = DW0_9_IN_ZP;
			*DW0OutputZP = DW0_9_OUT_ZP;
			*PW1OSize    = PW1_9_O_SIZE;
			*PW1ISize    = PW1_9_I_SIZE;
			*PW1ICSize   = PW1_9_IC_SIZE;
			*PW1OCSize   = PW1_9_OC_SIZE;
			*PW1BiasZP   = PW1_9_BIAS_ZP;
			*PW1InputZP  = PW1_9_IN_ZP;
			*PW1OutputZP = PW1_9_OUT_ZP;
			break;
		case 11:
			InputSize    = PW0_10_I_SIZE * PW0_10_I_SIZE * PW0_10_IC_SIZE;
			OutputSize   = PW1_10_O_SIZE * PW1_10_O_SIZE * PW1_10_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_10_ENABLE;
			*PW0OSize    = PW0_10_O_SIZE;
			*PW0ISize    = PW0_10_I_SIZE;
			*PW0ICSize   = PW0_10_IC_SIZE;
			*PW0OCSize   = PW0_10_OC_SIZE;
			*PW0BiasZP   = PW0_10_BIAS_ZP;
			*PW0InputZP  = PW0_10_IN_ZP;
			*PW0OutputZP = PW0_10_OUT_ZP;
			*DW0OSize    = DW0_10_O_SIZE;
			*DW0ISize    = DW0_10_I_SIZE;
			*DW0ICSize   = DW0_10_IC_SIZE;
			*DW0OCSize   = DW0_10_OC_SIZE;
			*DW0Stride   = DW0_10_STRIDE;
			*DW0BiasZP   = DW0_10_BIAS_ZP;
			*DW0InputZP  = DW0_10_IN_ZP;
			*DW0OutputZP = DW0_10_OUT_ZP;
			*PW1OSize    = PW1_10_O_SIZE;
			*PW1ISize    = PW1_10_I_SIZE;
			*PW1ICSize   = PW1_10_IC_SIZE;
			*PW1OCSize   = PW1_10_OC_SIZE;
			*PW1BiasZP   = PW1_10_BIAS_ZP;
			*PW1InputZP  = PW1_10_IN_ZP;
			*PW1OutputZP = PW1_10_OUT_ZP;
			break;
		case 12:
			InputSize    = PW0_11_I_SIZE * PW0_11_I_SIZE * PW0_11_IC_SIZE;
			OutputSize   = PW1_11_O_SIZE * PW1_11_O_SIZE * PW1_11_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_11_ENABLE;
			*PW0OSize    = PW0_11_O_SIZE;
			*PW0ISize    = PW0_11_I_SIZE;
			*PW0ICSize   = PW0_11_IC_SIZE;
			*PW0OCSize   = PW0_11_OC_SIZE;
			*PW0BiasZP   = PW0_11_BIAS_ZP;
			*PW0InputZP  = PW0_11_IN_ZP;
			*PW0OutputZP = PW0_11_OUT_ZP;
			*DW0OSize    = DW0_11_O_SIZE;
			*DW0ISize    = DW0_11_I_SIZE;
			*DW0ICSize   = DW0_11_IC_SIZE;
			*DW0OCSize   = DW0_11_OC_SIZE;
			*DW0Stride   = DW0_11_STRIDE;
			*DW0BiasZP   = DW0_11_BIAS_ZP;
			*DW0InputZP  = DW0_11_IN_ZP;
			*DW0OutputZP = DW0_11_OUT_ZP;
			*PW1OSize    = PW1_11_O_SIZE;
			*PW1ISize    = PW1_11_I_SIZE;
			*PW1ICSize   = PW1_11_IC_SIZE;
			*PW1OCSize   = PW1_11_OC_SIZE;
			*PW1BiasZP   = PW1_11_BIAS_ZP;
			*PW1InputZP  = PW1_11_IN_ZP;
			*PW1OutputZP = PW1_11_OUT_ZP;
			break;
		case 13:
			InputSize    = PW0_12_I_SIZE * PW0_12_I_SIZE * PW0_12_IC_SIZE;
			OutputSize   = PW1_12_O_SIZE * PW1_12_O_SIZE * PW1_12_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_12_ENABLE;
			*PW0OSize    = PW0_12_O_SIZE;
			*PW0ISize    = PW0_12_I_SIZE;
			*PW0ICSize   = PW0_12_IC_SIZE;
			*PW0OCSize   = PW0_12_OC_SIZE;
			*PW0BiasZP   = PW0_12_BIAS_ZP;
			*PW0InputZP  = PW0_12_IN_ZP;
			*PW0OutputZP = PW0_12_OUT_ZP;
			*DW0OSize    = DW0_12_O_SIZE;
			*DW0ISize    = DW0_12_I_SIZE;
			*DW0ICSize   = DW0_12_IC_SIZE;
			*DW0OCSize   = DW0_12_OC_SIZE;
			*DW0Stride   = DW0_12_STRIDE;
			*DW0BiasZP   = DW0_12_BIAS_ZP;
			*DW0InputZP  = DW0_12_IN_ZP;
			*DW0OutputZP = DW0_12_OUT_ZP;
			*PW1OSize    = PW1_12_O_SIZE;
			*PW1ISize    = PW1_12_I_SIZE;
			*PW1ICSize   = PW1_12_IC_SIZE;
			*PW1OCSize   = PW1_12_OC_SIZE;
			*PW1BiasZP   = PW1_12_BIAS_ZP;
			*PW1InputZP  = PW1_12_IN_ZP;
			*PW1OutputZP = PW1_12_OUT_ZP;
			break;
		case 14:
			InputSize    = PW0_13_I_SIZE * PW0_13_I_SIZE * PW0_13_IC_SIZE;
			OutputSize   = PW1_13_O_SIZE * PW1_13_O_SIZE * PW1_13_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_13_ENABLE;
			*PW0OSize    = PW0_13_O_SIZE;
			*PW0ISize    = PW0_13_I_SIZE;
			*PW0ICSize   = PW0_13_IC_SIZE;
			*PW0OCSize   = PW0_13_OC_SIZE;
			*PW0BiasZP   = PW0_13_BIAS_ZP;
			*PW0InputZP  = PW0_13_IN_ZP;
			*PW0OutputZP = PW0_13_OUT_ZP;
			*DW0OSize    = DW0_13_O_SIZE;
			*DW0ISize    = DW0_13_I_SIZE;
			*DW0ICSize   = DW0_13_IC_SIZE;
			*DW0OCSize   = DW0_13_OC_SIZE;
			*DW0Stride   = DW0_13_STRIDE;
			*DW0BiasZP   = DW0_13_BIAS_ZP;
			*DW0InputZP  = DW0_13_IN_ZP;
			*DW0OutputZP = DW0_13_OUT_ZP;
			*PW1OSize    = PW1_13_O_SIZE;
			*PW1ISize    = PW1_13_I_SIZE;
			*PW1ICSize   = PW1_13_IC_SIZE;
			*PW1OCSize   = PW1_13_OC_SIZE;
			*PW1BiasZP   = PW1_13_BIAS_ZP;
			*PW1InputZP  = PW1_13_IN_ZP;
			*PW1OutputZP = PW1_13_OUT_ZP;
			break;
		case 15:
			InputSize    = PW0_14_I_SIZE * PW0_14_I_SIZE * PW0_14_IC_SIZE;
			OutputSize   = PW1_14_O_SIZE * PW1_14_O_SIZE * PW1_14_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_14_ENABLE;
			*PW0OSize    = PW0_14_O_SIZE;
			*PW0ISize    = PW0_14_I_SIZE;
			*PW0ICSize   = PW0_14_IC_SIZE;
			*PW0OCSize   = PW0_14_OC_SIZE;
			*PW0BiasZP   = PW0_14_BIAS_ZP;
			*PW0InputZP  = PW0_14_IN_ZP;
			*PW0OutputZP = PW0_14_OUT_ZP;
			*DW0OSize    = DW0_14_O_SIZE;
			*DW0ISize    = DW0_14_I_SIZE;
			*DW0ICSize   = DW0_14_IC_SIZE;
			*DW0OCSize   = DW0_14_OC_SIZE;
			*DW0Stride   = DW0_14_STRIDE;
			*DW0BiasZP   = DW0_14_BIAS_ZP;
			*DW0InputZP  = DW0_14_IN_ZP;
			*DW0OutputZP = DW0_14_OUT_ZP;
			*PW1OSize    = PW1_14_O_SIZE;
			*PW1ISize    = PW1_14_I_SIZE;
			*PW1ICSize   = PW1_14_IC_SIZE;
			*PW1OCSize   = PW1_14_OC_SIZE;
			*PW1BiasZP   = PW1_14_BIAS_ZP;
			*PW1InputZP  = PW1_14_IN_ZP;
			*PW1OutputZP = PW1_14_OUT_ZP;
			break;
		case 16:
			InputSize    = PW0_15_I_SIZE * PW0_15_I_SIZE * PW0_15_IC_SIZE;
			OutputSize   = PW1_15_O_SIZE * PW1_15_O_SIZE * PW1_15_OC_SIZE;
			*ResLength   = InputSize;
			resEnable    = RES_15_ENABLE;
			*PW0OSize    = PW0_15_O_SIZE;
			*PW0ISize    = PW0_15_I_SIZE;
			*PW0ICSize   = PW0_15_IC_SIZE;
			*PW0OCSize   = PW0_15_OC_SIZE;
			*PW0BiasZP   = PW0_15_BIAS_ZP;
			*PW0InputZP  = PW0_15_IN_ZP;
			*PW0OutputZP = PW0_15_OUT_ZP;
			*DW0OSize    = DW0_15_O_SIZE;
			*DW0ISize    = DW0_15_I_SIZE;
			*DW0ICSize   = DW0_15_IC_SIZE;
			*DW0OCSize   = DW0_15_OC_SIZE;
			*DW0Stride   = DW0_15_STRIDE;
			*DW0BiasZP   = DW0_15_BIAS_ZP;
			*DW0InputZP  = DW0_15_IN_ZP;
			*DW0OutputZP = DW0_15_OUT_ZP;
			*PW1OSize    = PW1_15_O_SIZE;
			*PW1ISize    = PW1_15_I_SIZE;
			*PW1ICSize   = PW1_15_IC_SIZE;
			*PW1OCSize   = PW1_15_OC_SIZE;
			*PW1BiasZP   = PW1_15_BIAS_ZP;
			*PW1InputZP  = PW1_15_IN_ZP;
			*PW1OutputZP = PW1_15_OUT_ZP;
			break;
		default:
			InputSize    = 0;
			OutputSize   = 0;
			*ResLength   = 0;
			resEnable    = 0;
			*PW0OSize    = 0;
			*PW0ISize    = 0;
			*PW0ICSize   = 0;
			*PW0OCSize   = 0;
			*PW0BiasZP   = 0;
			*PW0InputZP  = 0;
			*PW0OutputZP = 0;
			*DW0OSize    = 0;
			*DW0ISize    = 0;
			*DW0ICSize   = 0;
			*DW0OCSize   = 0;
			*DW0Stride   = 0;
			*DW0BiasZP   = 0;
			*DW0InputZP  = 0;
			*DW0OutputZP = 0;
			*PW1OSize    = 0;
			*PW1ISize    = 0;
			*PW1ICSize   = 0;
			*PW1OCSize   = 0;
			*PW1BiasZP   = 0;
			*PW1InputZP  = 0;
			*PW1OutputZP = 0;
			return;
	}

	//Initialize DRAM-Stream DMA
	*StrDma0RdAddr = feat_rd_addr;
	*StrDma0RdFrameSize = InputSize;
	*StrDma0NumRdFrames = 1;
	*StrDma0RdFrameBuffSize = 1;
	//Initialize Stream-DRAM DMA
	*StrDma0WrAddr = feat_wr_addr;
	*StrDma0WrFrameSize = OutputSize;
	*StrDma0NumWrFrames = 1;
	*StrDma0WrFrameBuffSize = 1;
	//Initialize Res DMA
	*StrDma1RdAddr = res_rd_addr;
	*StrDma1RdFrameSize = InputSize;
	*StrDma1NumRdFrames = 1;
	*StrDma1RdFrameBuffSize = 1;

	//Start Stream DMAs
	*StrDma0Flags = STR_DMA_INIT_RD | STR_DMA_INIT_WR;
	if (resEnable == 1) *StrDma1Flags = STR_DMA_INIT_RD;

	//Transfer Weights and Quantization Params from DRAM to SPMs
	
	//Start PW0 Conv Weight Xfer
	*MemDmaRdAddr  = pw0_weights;
	*MemDmaWrAddr  = body_PWConv0Weights;
	*MemDmaCopyLen = PW0WeightSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	
	// Start PW0 Conv QParams Xfer
	*MemDmaRdAddr  = pw0_quant + PW0QParamSize*0;
	*MemDmaWrAddr  = body_PW0Bias;
	*MemDmaCopyLen = PW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw0_quant + PW0QParamSize*1;
	*MemDmaWrAddr  = body_PW0IMultBias;
	*MemDmaCopyLen = PW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw0_quant + PW0QParamSize*2;
	*MemDmaWrAddr  = body_PW0NShiftBias;
	*MemDmaCopyLen = PW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw0_quant + PW0QParamSize*3;
	*MemDmaWrAddr  = body_PW0IMultOut;
	*MemDmaCopyLen = PW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw0_quant + PW0QParamSize*4;
	*MemDmaWrAddr  = body_PW0NShiftOut;
	*MemDmaCopyLen = PW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	
	*MemDmaRdAddr  = pw0_quant + PW0QParamSize*5;
	*MemDmaWrAddr  = body_PW0WeightZP;
	*MemDmaCopyLen = PW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	
	//Start DW0 Conv Weight Xfer
	*MemDmaRdAddr  = dw0_weights;
	*MemDmaWrAddr  = body_DWConvWeights;
	*MemDmaCopyLen = DW0WeightSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	
	// Start DW0 Conv QParams Xfer
	*MemDmaRdAddr  = dw0_quant + DW0QParamSize*0;
	*MemDmaWrAddr  = body_DWBias;
	*MemDmaCopyLen = DW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = dw0_quant + DW0QParamSize*1;
	*MemDmaWrAddr  = body_DWIMultBias;
	*MemDmaCopyLen = DW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = dw0_quant + DW0QParamSize*2;
	*MemDmaWrAddr  = body_DWNShiftBias;
	*MemDmaCopyLen = DW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = dw0_quant + DW0QParamSize*3;
	*MemDmaWrAddr  = body_DWIMultOut;
	*MemDmaCopyLen = DW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = dw0_quant + DW0QParamSize*4;
	*MemDmaWrAddr  = body_DWNShiftOut;
	*MemDmaCopyLen = DW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	
	*MemDmaRdAddr  = dw0_quant + DW0QParamSize*5;
	*MemDmaWrAddr  = body_DWWeightZP;
	*MemDmaCopyLen = DW0QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	//Start PW1 Conv Weight Xfer
	*MemDmaRdAddr  = pw1_weights;
	*MemDmaWrAddr  = body_PWConv1Weights;
	*MemDmaCopyLen = PW1WeightSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);
	
	// Start PW1 Conv QParams Xfer
	*MemDmaRdAddr  = pw1_quant + PW1QParamSize*0;
	*MemDmaWrAddr  = body_PW1Bias;
	*MemDmaCopyLen = PW1QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw1_quant + PW1QParamSize*1;
	*MemDmaWrAddr  = body_PW1IMultBias;
	*MemDmaCopyLen = PW1QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw1_quant + PW1QParamSize*2;
	*MemDmaWrAddr  = body_PW1NShiftBias;
	*MemDmaCopyLen = PW1QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw1_quant + PW1QParamSize*3;
	*MemDmaWrAddr  = body_PW1IMultOut;
	*MemDmaCopyLen = PW1QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw1_quant + PW1QParamSize*4;
	*MemDmaWrAddr  = body_PW1NShiftOut;
	*MemDmaCopyLen = PW1QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	*MemDmaRdAddr  = pw1_quant + PW1QParamSize*5;
	*MemDmaWrAddr  = body_PW1WeightZP;
	*MemDmaCopyLen = PW1QParamSize;
	*MemDmaFlags   = MEM_DMA_INIT;
	//Poll DMA for finish
	while ((*MemDmaFlags & MEM_DMA_INTR) != MEM_DMA_INTR);

	//Start Res
	*ResEnable = resEnable;
	*ResFlags = 0x01;
	//Start PW Conv 0
	*PW0Flags = 0x01;
	// Start DW Conv
	*DW0Flags = 0x01;
	//Start PW Conv 1
	*PW1Flags = 0x01;
	
	// Wait for all accelerators to finish before sending interrupt to CPU
	// while ((*StrDma0Flags & STR_DMA_WR_RUNNING) == STR_DMA_WR_RUNNING);
	while (*PW0Flags != 0x4);
	while (*DW0Flags != 0x4);
	while (*PW1Flags != 0x4);
	return;
}