#include "hw_defines.h"

void top(uint64_t seqa_addr,
		 uint64_t seqb_addr,
		 uint64_t aligneda_addr,
		 uint64_t alignedb_addr) {

	//Define Device MMRs
	volatile uint8_t  * NWFlags    = (uint8_t *)NW;
	volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA+1);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA+9);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA+17);

	//Transfer Input Matrices
	//Transfer SEQA
	*DmaRdAddr  = seqa_addr;
	*DmaWrAddr  = SEQAADDR;
	*DmaCopyLen = ALEN;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer SEQB
	*DmaRdAddr  = seqb_addr;
	*DmaWrAddr  = SEQBADDR;
	*DmaCopyLen = BLEN;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Start the accelerated function
	*NWFlags = DEV_INIT;
	//Poll function for finish
	while ((*NWFlags & DEV_INTR) != DEV_INTR);

	//Transfer AlignedA
	*DmaRdAddr  = ALIGNEDAADDR;
	*DmaWrAddr  = aligneda_addr;
	*DmaCopyLen = (ALEN+BLEN);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Transfer AlignedB
	*DmaRdAddr  = ALIGNEDBADDR;
	*DmaWrAddr  = alignedb_addr;
	*DmaCopyLen = (ALEN+BLEN);
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	return;
}