#include <stdint.h>
#define DEV_INIT	0x01
#define DEV_INTR	0x04
#define TYPE float

#define STR_DMA_INIT_RD	0x01
#define STR_DMA_INIT_WR 0x02
#define STR_DMA_RD_INTR	0x10
#define STR_DMA_WR_INTR	0x20
#define STR_DMA_RD_RUNNING 0x04
#define STR_DMA_WR_RUNNING 0x08

#define conv0UnrollFactor 1 // Should be from 1->Input Channels
#define conv0InDim 32
#define conv0InChan 1
#define conv0OutDim 28
#define conv0OutChan 6
#define conv0KSize 5
#define conv0KernChan 6
#define conv0InSize conv0InDim*conv0InDim*conv0InChan*sizeof(TYPE)
#define conv0WeightSize conv0KSize*conv0KSize*conv0InChan*conv0KernChan*sizeof(TYPE)
#define conv0OutputSize conv0OutDim*conv0OutDim*conv0KernChan*sizeof(TYPE)

#define pool0UnrollFactor 1
#define pool0InDim 28
#define pool0InChan 6
#define pool0KSize 2
#define pool0OutDim 14
#define pool0InSize pool0InDim*pool0InDim*pool0InChan*sizeof(TYPE)
#define pool0OutputSize pool0OutDim*pool0OutDim*pool0InChan*sizeof(TYPE)

#define conv1UnrollFactor 1
#define conv1InDim 14
#define conv1InChan 6
#define conv1KSize 5
#define conv1KernChan 16
#define conv1OutDim 10
#define conv1OutChan 16
#define conv1InSize conv1InDim*conv1InDim*conv1InChan*sizeof(TYPE)
#define conv1WeightSize conv1KSize*conv1KSize*conv1InChan*conv1KernChan*sizeof(TYPE)
#define conv1OutputSize conv1OutDim*conv1OutDim*conv1KernChan*sizeof(TYPE)

#define pool1UnrollFactor 1
#define pool1InDim 10
#define pool1InChan 16
#define pool1KSize 2
#define pool1OutDim 5
#define pool1InSize pool1InDim*pool1InDim*pool1InChan*sizeof(TYPE)
#define pool1OutputSize pool1OutDim*pool1OutDim*pool1InChan*sizeof(TYPE)

#define fc0InDim 5
#define fc0InChan 16
#define fc0KSize 5
#define fc0KernChan 120
#define fc0OutDim 1
#define fc0OutChan 120
#define fc0InSize fc0InDim*fc0InDim*fc0InChan*sizeof(TYPE)
#define fc0WeightSize fc0KSize*fc0KSize*fc0InChan*fc0KernChan*sizeof(TYPE)
#define fc0OutputSize fc0OutDim*fc0OutDim*fc0KernChan*sizeof(TYPE)

#define fc1InDim 1
#define fc1InChan 120
#define fc1KSize 1
#define fc1KernChan 84
#define fc1OutDim 1
#define fc1OutChan 84
#define fc1InSize fc1InDim*fc1InDim*fc1InChan*sizeof(TYPE)
#define fc1WeightSize fc1KSize*fc1KSize*fc0InChan*fc1KernChan*sizeof(TYPE)
#define fc1OutputSize fc1OutDim*fc1OutDim*fc1KernChan*sizeof(TYPE)

//BEGIN GENERATED CODE
//Cluster: FC1
//NonCoherentDMA
#define DMA_Flags 0x10020000
#define DMA_RdAddr 0x10020001
#define DMA_WrAddr 0x10020009
#define DMA_CopyLen 0x10020011
//StreamDMA
#define STREAMDMA_Flags 0x10020015
#define STREAMDMA_RdAddr 0x10020019
#define STREAMDMA_WrAddr 0x10020021
#define STREAMDMA_RdFrameSize 0x10020029
#define STREAMDMA_NumRdFrames 0x1002002d
#define STREAMDMA_RdFrameBufSize 0x1002002e
#define STREAMDMA_WrFrameSize 0x1002002f
#define STREAMDMA_NumWrFrames 0x10020033
#define STREAMDMA_WrFrameBufSize 0x10020034
#define STREAMDMA_Stream 0x10020035
//Accelerator: TOP
#define TOP 0x1002003d
//Accelerator: CONV0
#define CONV0 0x10020046
#define Conv0Window 0x10020047
#define Conv0LUT 0x100202c7
#define Conv0Out 0x100202cb
//Accelerator: POOL0
#define POOL0 0x100202cf
#define Pool0Window 0x100202d0
#define Pool0Out 0x10020810
//Accelerator: CONV1
#define CONV1 0x10020814
#define Conv1Window 0x10020815
#define Conv1LUT 0x10020ea5
#define Conv1Out 0x10020ea9
//Accelerator: POOL1
#define POOL1 0x10020ead
#define Pool1Window 0x10020eae
#define Pool1LUT 0x10021b2e
#define Pool1Out 0x10021b32
//Accelerator: FC0
#define FC0 0x10021b36
#define fc0Window 0x10021b37
#define fc0LUT 0x10022177
#define fc0Out 0x1002217b
//Accelerator: FC1
#define FC1 0x1002217f
#define fc1Window 0x10022180
#define fc1LUT 0x10022360
//END GENERATED CODE