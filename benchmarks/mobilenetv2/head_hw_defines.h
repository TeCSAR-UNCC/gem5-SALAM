//BEGIN GENERATED CODE
//Cluster: PWCONV1
//NonCoherentDMA
#define DMA_Flags 0x10020000
#define DMA_RdAddr 0x10020001
#define DMA_WrAddr 0x10020009
#define DMA_CopyLen 0x10020011
//StreamDMA
#define STREAM_DMA0_Flags 0x10020015
#define STREAM_DMA0_RdAddr 0x10020019
#define STREAM_DMA0_WrAddr 0x10020021
#define STREAM_DMA0_RdFrameSize 0x10020029
#define STREAM_DMA0_NumRdFrames 0x1002002d
#define STREAM_DMA0_RdFrameBufSize 0x1002002e
#define STREAM_DMA0_WrFrameSize 0x1002002f
#define STREAM_DMA0_NumWrFrames 0x10020033
#define STREAM_DMA0_WrFrameBufSize 0x10020034
#define STREAM_DMA0_Stream 0x10020035
//Accelerator: TOP
#define TOP 0x1002003d
//Accelerator: NORMALCONV
#define NORMALCONV 0x1002007e
#define NormalConvWindow 0x1002007f
#define NormalConvWeights 0x1002009a
#define NormalConvQParams 0x10020322
#define NormalConvQBuffer 0x100203b2
#define NormalConvOut 0x100205f2
//Accelerator: DWCONV
#define DWCONV 0x100205f3
#define DWConvWindow 0x100205f4
#define DWConvWeights 0x100206cc
#define DWConvQParams 0x100207bc
#define DWConvQBuffer 0x1002084c
#define DWConvOutBuffer 0x10020fcc
#define DWConvOut 0x10020fe4
//Accelerator: PWCONV
#define PWCONV 0x10020fe5
#define PWConvLocalFeatSize 0x10020fe6
#define PWConvWeights 0x10020ffe
#define PWConvQParams 0x1002117e
//END GENERATED CODE