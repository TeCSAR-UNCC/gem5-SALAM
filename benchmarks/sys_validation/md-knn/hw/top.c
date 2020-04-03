#include "hw_defines.h"

void top(uint64_t force_x_addr,
		 uint64_t force_y_addr,
		 uint64_t force_z_addr,
		 uint64_t position_x_addr,
		 uint64_t position_y_addr,
		 uint64_t position_z_addr,
		 uint64_t nl_addr) {

	//Define Device MMRs
	volatile uint8_t  * MDFlags    = (uint8_t *)MD;
	volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA+1);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA+9);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA+17);

	//Transfer Input Matrices
	//Transfer Position X
	*DmaRdAddr  = position_x_addr;
	*DmaWrAddr  = POSITIONXADDR;
	*DmaCopyLen = ARRAY_SIZE;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer Position Y
	*DmaRdAddr  = position_y_addr;
	*DmaWrAddr  = POSITIONYADDR;
	*DmaCopyLen = ARRAY_SIZE;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer Position Z
	*DmaRdAddr  = position_z_addr;
	*DmaWrAddr  = POSITIONZADDR;
	*DmaCopyLen = ARRAY_SIZE;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer NL
	*DmaRdAddr  = nl_addr;
	*DmaWrAddr  = NLADDR;
	*DmaCopyLen = NL_SIZE;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Start the accelerated function
	*MDFlags = DEV_INIT;
	//Poll function for finish
	while ((*MDFlags & DEV_INTR) != DEV_INTR);

	//Transfer Force X
	*DmaRdAddr  = FORCEXADDR;
	*DmaWrAddr  = force_x_addr;
	*DmaCopyLen = ARRAY_SIZE;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer Force Y
	*DmaRdAddr  = FORCEYADDR;
	*DmaWrAddr  = force_y_addr;
	*DmaCopyLen = ARRAY_SIZE;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer Force Z
	*DmaRdAddr  = FORCEZADDR;
	*DmaWrAddr  = force_z_addr;
	*DmaCopyLen = ARRAY_SIZE;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	return;
}