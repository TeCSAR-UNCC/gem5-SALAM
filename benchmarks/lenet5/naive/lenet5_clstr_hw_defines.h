#include <stdint.h>
#define DEV_INIT	0x01
#define DEV_INTR	0x04
#define TYPE float

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
//Accelerator: TOP
#define TOP 0x10020040
//Accelerator: CONV0
#define CONV0 0x10020080
#define Conv0Input 0x100200c0
#define Conv0Weights 0x10021100
#define Conv0Output 0x10021380
//Accelerator: POOL0
#define POOL0 0x10025d40
#define pool0Input 0x10025d80
#define pool0Output 0x1002a740
//Accelerator: CONV1
#define CONV1 0x1002b9c0
#define Conv1Input 0x1002ba00
#define Conv1Weights 0x1002cc80
#define Conv1Output 0x1002f240
//Accelerator: POOL1
#define POOL1 0x10030b80
#define pool1Input 0x10030bc0
#define pool1Output 0x10032500
//Accelerator: FC0
#define FC0 0x10032b80
#define fc0Input 0x10032bc0
#define fc0Weights 0x10033240
#define fc0Output 0x10062080
//Accelerator: FC1
#define FC1 0x10062280
#define fc1Input 0x100622c0
#define fc1Weights 0x100624c0
#define fc1Output 0x1006c280
//END GENERATED CODE