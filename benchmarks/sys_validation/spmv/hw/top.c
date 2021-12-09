#include "spmv.h"

void top(uint64_t val,
		 uint64_t cols,
		 uint64_t rowDelims,
		 uint64_t vec,
		 uint64_t out) {

	//Define Device MMRs
	volatile uint8_t  * SPMVFlags  = (uint8_t *)SPMV;
	volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA_Flags);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA_RdAddr);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA_WrAddr);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA_CopyLen);

	//Transfer Input Matrices
	//Transfer M1
	*DmaRdAddr  = val;
	*DmaWrAddr  = VAL;
	*DmaCopyLen = NNZ*sizeof(TYPE);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer M2
	*DmaRdAddr  = cols;
	*DmaWrAddr  = COLS;
	*DmaCopyLen = NNZ*sizeof(int32_t);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer M2
	*DmaRdAddr  = rowDelims;
	*DmaWrAddr  = ROWDELIMS;
	*DmaCopyLen = (N+1)*sizeof(int32_t);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer M2
	*DmaRdAddr  = vec;
	*DmaWrAddr  = VEC;
	*DmaCopyLen = N*sizeof(TYPE);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Start the accelerated function
	*SPMVFlags = DEV_INIT;
	//Poll function for finish
	while ((*SPMVFlags & DEV_INTR) != DEV_INTR);

	//Transfer M3
	*DmaRdAddr  = OUT;
	*DmaWrAddr  = out;
	*DmaCopyLen = N*sizeof(TYPE);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	return;
}