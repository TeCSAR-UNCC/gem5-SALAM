#include <stdint.h>
#define DEV_INIT	0x01
#define DEV_INTR	0x04
#define TYPE uint32_t

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

#define conv2InDim 5
#define conv2InChan 16
#define conv2KSize 5
#define conv2KernChan 120
#define conv2OutDim 1
#define conv2OutChan 120
#define conv2InSize conv2InDim*conv2InDim*conv2InChan*sizeof(TYPE)
#define conv2WeightSize conv2KSize*conv2KSize*conv2InChan*conv2KernChan*sizeof(TYPE)
#define conv2OutputSize conv2OutDim*conv2OutDim*conv2KernChan*sizeof(TYPE)

#define fc0InDim 1
#define fc0InChan 120
#define fc0KSize 1
#define fc0KernChan 84
#define fc0OutDim 1
#define fc0OutChan 84
#define fc0InSize fc0InDim*fc0InDim*fc0InChan*sizeof(TYPE)
#define fc0WeightSize fc0KSize*fc0KSize*conv2InChan*fc0KernChan*sizeof(TYPE)
#define fc0OutputSize fc0OutDim*fc0OutDim*fc0KernChan*sizeof(TYPE)

//BEGIN GENERATED CODE
//Cluster: LENET5_CLSTR
//NonCoherentDMA
#define DMA_Flags 0x10020000
#define DMA_RdAddr 0x10020001
#define DMA_WrAddr 0x10020009
#define DMA_CopyLen 0x10020011
//StreamDMA
#define STREAMDMA_Flags 0x10020040
#define STREAMDMA_RdAddr 0x10020044
#define STREAMDMA_WrAddr 0x1002004c
#define STREAMDMA_RdFrameSize 0x10020054
#define STREAMDMA_NumRdFrames 0x10020058
#define STREAMDMA_RdFrameBufSize 0x10020059
#define STREAMDMA_WrFrameSize 0x1002005a
#define STREAMDMA_NumWrFrames 0x1002005e
#define STREAMDMA_WrFrameBufSize 0x1002005f
#define STREAMDMA_Stream 0x10020060
#define STREAMDMA_Status 0x10020080
//Accelerator: TOP
#define TOP 0x100200c0
//Accelerator: DATA_MOVE_0
#define DATA_MOVE_0 0x10020100
#define Conv0LineBuff 0x10020140
//Accelerator: CONV0
#define CONV0 0x10020400
#define Conv0Weights 0x10020440
#define Conv0Window 0x100206c0
#define Conv0Out 0x10020740
#define Conv0Out_Status 0x10020780
//Accelerator: POOL_MOVE_0
#define POOL_MOVE_0 0x100207c0
//Accelerator: POOL0
#define POOL0 0x10020800
#define Pool0Window 0x10020840
#define Pool0Out 0x10020dc0
#define Pool0Out_Status 0x10020e00
//Accelerator: DATA_MOVE_1
#define DATA_MOVE_1 0x10020e40
#define Conv1LineBuff 0x10020e80
//Accelerator: CONV1
#define CONV1 0x10022100
#define Conv1Weights 0x10022140
#define Conv1Window 0x10024700
#define Conv1Out 0x10024980
#define Conv1Out_Status 0x100249c0
//Accelerator: POOL_MOVE_1
#define POOL_MOVE_1 0x10024a00
//Accelerator: POOL1
#define POOL1 0x10024a40
#define Pool1Window 0x10024a80
#define Pool1Out 0x10024fc0
#define Pool1Out_Status 0x10025000
//Accelerator: DATA_MOVE_2
#define DATA_MOVE_2 0x10025040
#define Conv2LineBuff 0x10025080
//Accelerator: CONV2
#define CONV2 0x10025700
#define Conv2Weights 0x10025740
#define Conv2Window 0x10054580
#define Conv2Out 0x10054c00
#define Conv2Out_Status 0x10054c40
//Accelerator: DATA_MOVE_3
#define DATA_MOVE_3 0x10054c80
#define FC0LineBuff 0x10054cc0
//Accelerator: FC0
#define FC0 0x10054ec0
#define FC0Weights 0x10054f00
#define FC0Window 0x1005ecc0
//END GENERATED CODE