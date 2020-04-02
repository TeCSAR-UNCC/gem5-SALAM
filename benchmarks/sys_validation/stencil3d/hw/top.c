#include "hw_defines.h"

void top(uint64_t C_addr,
		 uint64_t ORIG_addr,
		 uint64_t SOL_addr) {

	//Define Device MMRs
	volatile uint8_t  * STENCILFlags  = (uint8_t *)STENCIL;
	volatile uint8_t  * DmaFlags      = (uint8_t  *)(DMA);
	volatile uint64_t * DmaRdAddr     = (uint64_t *)(DMA+1);
	volatile uint64_t * DmaWrAddr     = (uint64_t *)(DMA+9);
	volatile uint32_t * DmaCopyLen    = (uint32_t *)(DMA+17);

	//Transfer Input Matrices
	//Transfer C
	*DmaRdAddr  = C_addr;
	*DmaWrAddr  = CADDR;
	*DmaCopyLen = C_SIZE*sizeof(TYPE);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer ORIG
	*DmaRdAddr  = ORIG_addr;
	*DmaWrAddr  = ORIGADDR;
	*DmaCopyLen = SIZE*sizeof(TYPE);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Start the accelerated function
	*STENCILFlags = DEV_INIT;
	//Poll function for finish
	while ((*STENCILFlags & DEV_INTR) != DEV_INTR);

	//Transfer SOL
	*DmaRdAddr  = SOLADDR;
	*DmaWrAddr  = SOL_addr;
	*DmaCopyLen = SIZE*sizeof(TYPE);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	return;
}