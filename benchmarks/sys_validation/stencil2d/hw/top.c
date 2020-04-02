#include "hw_defines.h"

void top(uint64_t ORIG_addr,
		 uint64_t SOL_addr,
		 uint64_t FILTER_addr) {

	//Define Device MMRs
	volatile uint8_t  * STENCILFlags  = (uint8_t *)STENCIL;
	volatile uint8_t  * DmaFlags      = (uint8_t  *)(DMA);
	volatile uint64_t * DmaRdAddr     = (uint64_t *)(DMA+1);
	volatile uint64_t * DmaWrAddr     = (uint64_t *)(DMA+9);
	volatile uint32_t * DmaCopyLen    = (uint32_t *)(DMA+17);

	//Transfer Input Matrices
	//Transfer M1
	*DmaRdAddr  = ORIG_addr;
	*DmaWrAddr  = ORIGADDR;
	*DmaCopyLen = row_size * col_size * sizeof(TYPE);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer M2
	*DmaRdAddr  = FILTER_addr;
	*DmaWrAddr  = FILTERADDR;
	*DmaCopyLen = f_size * sizeof(TYPE);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Start the accelerated function
	*STENCILFlags = DEV_INIT;
	//Poll function for finish
	while ((*STENCILFlags & DEV_INTR) != DEV_INTR);

	//Transfer M3
	*DmaRdAddr  = SOLADDR;
	*DmaWrAddr  = SOL_addr;
	*DmaCopyLen = row_size * col_size * sizeof(TYPE);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	return;
}