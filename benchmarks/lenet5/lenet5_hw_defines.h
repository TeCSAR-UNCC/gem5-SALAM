#include <stdint.h>
#define DEV_INIT	0x01
#define DEV_INTR	0x04
#define TYPE uint8_t

#define conv0InDim 32
#define conv0InChan 1
#define conv0InSize 1024
#define conv0OutDim 28
#define conv0OutChan 6
#define conv0KSize 5
#define conv0KernChan 6
#define conv0WeightSize 150
#define conv0OutputSize 4704

#define pool0InDim 28
#define pool0InChan 6
#define pool0KSize 2
#define pool0OutDim 14
#define pool0InSize 4704
#define pool0OutputSize 1176

#define conv1InDim 14
#define conv1InChan 6
#define conv1KSize 5
#define conv1KernChan 16
#define conv1OutDim 10
#define conv1OutChan 16
#define conv1InSize 1176
#define conv1WeightSize 400
#define conv1OutputSize 1600

#define pool1InDim 10
#define pool1InChan 16
#define pool1KSize 2
#define pool1OutDim 5
#define pool1InSize 1600
#define pool1OutputSize 400

//BEGIN GENERATED CODE
//Cluster: POOL1
//NonCoherentDMA
#define DMA_Flags 0x10020000
#define DMA_RdAddr 0x10020001
#define DMA_WrAddr 0x10020009
#define DMA_CopyLen 0x10020011
//Accelerator: TOP
#define TOP 0x10020015
//Accelerator: CONV0
#define CONV0 0x1002001e
#define Conv0Input 0x1002001f
#define Conv0Weights 0x1002041f
#define Conv0Output 0x100204b5
//Accelerator: POOL0
#define POOL0 0x10021715
#define pool0Input 0x10021716
#define pool0Output 0x10022976
//Accelerator: CONV1
#define CONV1 0x10022e0e
#define Conv1Input 0x10022e0f
#define Conv1Weights 0x100232a7
#define Conv1Output 0x10023437
//Accelerator: POOL1
#define POOL1 0x10023a77
#define pool1Input 0x10023a78
#define pool1Output 0x100240b8
//END GENERATED CODE