#include "hw_defines.h"

void top(uint64_t m1_addr,
		 uint64_t m2_addr,
		 uint64_t m3_addr) {

	//Define Device MMRs
	volatile uint8_t  * GEMMFlags  = (uint8_t *)GEMM;
	volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA_Flags);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA_RdAddr);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA_WrAddr);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA_CopyLen);

	//Transfer Input Matrices
	//Transfer M1
	*DmaRdAddr  = m1_addr;
	*DmaWrAddr  = MATRIX1;
	*DmaCopyLen = mat_size;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer M2
	*DmaRdAddr  = m2_addr;
	*DmaWrAddr  = MATRIX2;
	*DmaCopyLen = mat_size;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Start the accelerated function
	*GEMMFlags = DEV_INIT;
	//Poll function for finish
	while ((*GEMMFlags & DEV_INTR) != DEV_INTR);

	//Transfer M3
	*DmaRdAddr  = MATRIX3;
	*DmaWrAddr  = m3_addr;
	*DmaCopyLen = mat_size;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	return;
}