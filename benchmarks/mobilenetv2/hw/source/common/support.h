#ifndef __MNV2_SUPPORT_H__
#define __MNV2_SUPPORT_H__
#include "inttypes.h"

#define dType_Reg		int32_t
#define dType_8u		uint8_t
#define dType_16u		uint16_t
#define dType_32u		uint32_t
#define dType_8t		int8_t
#define dType_16t		int16_t
#define dType_32t		int32_t

#define MAX(a,b)		((a > b) ? a : b)
#define MIN(a,b)		((a < b) ? a : b)

#define STR_DMA_INIT_RD	0x01
#define STR_DMA_INIT_WR 0x02
#define STR_DMA_RD_INTR	0x10
#define STR_DMA_WR_INTR	0x20
#define STR_DMA_RD_RUNNING 0x04
#define STR_DMA_WR_RUNNING 0x08


#define MEM_DMA_INIT	0x01
#define MEM_DMA_INTR	0x04

#endif //__MNV2_SUPPORT_H__