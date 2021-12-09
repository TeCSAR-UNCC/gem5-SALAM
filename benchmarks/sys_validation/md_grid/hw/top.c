#include "md.h"

void top(uint64_t npoints,
		 uint64_t force,
		 uint64_t position) {

	//Define Device MMRs
	volatile uint8_t  * MD_GRIDFlags  = (uint8_t *)MD_GRID;
	volatile uint8_t  * DmaFlags   = (uint8_t  *)(DMA_Flags);
	volatile uint64_t * DmaRdAddr  = (uint64_t *)(DMA_RdAddr);
	volatile uint64_t * DmaWrAddr  = (uint64_t *)(DMA_WrAddr);
	volatile uint32_t * DmaCopyLen = (uint32_t *)(DMA_CopyLen);

	//Transfer Input Matrices
	//Transfer M1
	*DmaRdAddr  = npoints;
	*DmaWrAddr  = NPOINTS;
	*DmaCopyLen = points_size;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	//Transfer M2
	*DmaRdAddr  = position;
	*DmaWrAddr  = POSITION;
	*DmaCopyLen = grid_size;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);

	//Start the accelerated function
	*MD_GRIDFlags = DEV_INIT;
	//Poll function for finish
	while ((*MD_GRIDFlags & DEV_INTR) != DEV_INTR);

	//Transfer M3
	*DmaRdAddr  = FORCE;
	*DmaWrAddr  = force;
	*DmaCopyLen = grid_size;
	*DmaFlags   = DEV_INIT;
	//Poll DMA for finish
	while ((*DmaFlags & DEV_INTR) != DEV_INTR);
	return;
}