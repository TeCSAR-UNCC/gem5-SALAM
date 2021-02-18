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
#define NormalConvWeights 0x100212df
#define NormalConvQParams 0x1003cbdf
#define NormalConvQBuffer 0x1003ccb7
#define NormalConvOut 0x1003dc77
//Accelerator: DWCONV
#define DWCONV 0x1003dc78
#define DWConvWindow 0x1003dc79
#define DWConvWeights 0x1003e111
#define DWConvQParams 0x1003e5c1
#define DWConvQBuffer 0x1003e651
#define DWConvOutBuffer 0x10041351
#define DWConvOut 0x10041369
//Accelerator: PWCONV
#define PWCONV 0x1004136a
#define PWConvLocalFeatSize 0x1004136b
#define PWConvWeights 0x10041383
#define PWConvQParams 0x10041503
//END GENERATED CODE