#include <stdint.h>
#define DEV_INIT	0x01
#define DEV_INTR	0x04
#define TYPE uint8_t

#define INPUTDIMENSION 32

#define conv0InputSize INPUTDIMENSION*INPUTDIMENSION*sizeof(TYPE)
#define CONV0KSIZE 5
#define CONV0Channels 6
#define conv0WeightSize CONV0KSIZE*CONV0KSIZE*CONV0Channels*sizeof(TYPE)
#define conv0OutputSize 37632

#define POOL0InputDim 28
#define POOL0KSIZE 2
#define POOL0Channels 6
#define pool0InputSize 37632
#define pool0WeightSize 192
#define pool0OutputSize 9408

#define conv1InputDim 14
#define conv1InputChannels 6
#define conv1KSIZE 5
#define conv1OutputChannels 16
#define conv1InputSize conv1InputDim*conv1InputDim*conv1InputChannels*sizeof(TYPE)
#define conv1WeightSize conv1KSIZE*conv1KSIZE*conv1OutputChannels*sizeof(TYPE)
#define conv1OutputSize 12800

#define POOL1InputDim 10
#define POOL1KSIZE 2
#define POOL1Channels 16
#define pool1InputSize 12800
#define pool1WeightSize 512
#define pool1OutputSize 3200

#define conv2InputDim 5
#define conv2InputChannels 16
#define conv2KSIZE 5
#define conv2OutputChannels 1
#define conv2InputSize conv2InputDim*conv2InputDim*conv2InputChannels*sizeof(TYPE)
#define conv2WeightSize conv2KSIZE*conv2KSIZE*conv2OutputChannels*sizeof(TYPE)
#define conv2OutputSize 960

//BEGIN GENERATED CODE
//Cluster: CONV2
//NonCoherentDMA
#define DMA_Flags 0x10020000
#define DMA_RdAddr 0x10020001
#define DMA_WrAddr 0x10020009
#define DMA_CopyLen 0x10020011
//Accelerator: TOP
#define TOP 0x10020015
//Accelerator: CONV0
#define CONV0 0x1002001e
#define Conv0Window 0x1002001f
#define Conv0Weights 0x1002201f
#define Conv0Output 0x100224cf
//Accelerator: POOL0
#define POOL0 0x1002b7cf
#define pool0Window 0x1002b7d0
#define pool0Weights 0x10034ad0
#define pool0Output 0x10034b90
//Accelerator: CONV1
#define CONV1 0x10037050
#define Conv1Window 0x10037051
#define Conv1Weights 0x10039511
#define Conv1Output 0x1003a191
//Accelerator: POOL1
#define POOL1 0x1003d391
#define pool1Window 0x1003d392
#define pool1Weights 0x10040592
#define pool1Output 0x10040792
//Accelerator: CONV2
#define CONV2 0x10041412
#define Conv2Window 0x10041413
#define Conv2Weights 0x10042093
#define Conv2Output 0x10047e53
//END GENERATED CODE