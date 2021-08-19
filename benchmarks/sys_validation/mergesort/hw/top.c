#include "hw_defines.h"

void top(uint64_t m1_addr,
		 uint64_t m2_addr,
		 uint64_t m3_addr) {

	//Define Device MMRs
	volatile uint8_t  * SORTFlags  = (uint8_t *)SORT;
	volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA+1);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA+9);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA+17);

	//Start the accelerated function
	*SORTFlags = DEV_INIT;
	//Poll function for finish
	while ((*SORTFlags & DEV_INTR) != DEV_INTR);

	// //Transfer M3
	// *DmaRdAddr  = M3ADDR;
	// *DmaWrAddr  = m3_addr;
	// *DmaCopyLen = mat_size;
	// *DmaFlags   = DEV_INIT;
	// //Poll DMA for finish
	// while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	return;
}