#include "hw_defines.h"

void top(uint64_t unsorted,
		 uint64_t sorted) {

	//Define Device MMRs
	volatile uint8_t  * SORTFlags  = (uint8_t *)MERGESORT;
	volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA_Flags);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA_RdAddr);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA_WrAddr);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA_CopyLen);

	//Transfer Result
	*DmaRdAddr  = unsorted;
	*DmaWrAddr  = MAIN;
	*DmaCopyLen = arr_size;
	*DmaFlags   = DEV_INIT;
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Start the accelerated function
	*SORTFlags = DEV_INIT;
	//Poll function for finish
	while ((*SORTFlags & DEV_INTR) != DEV_INTR);

	//Transfer Result
	*DmaRdAddr  = MAIN;
	*DmaWrAddr  = sorted;
	*DmaCopyLen = arr_size;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	return;
}