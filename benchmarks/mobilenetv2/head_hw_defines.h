//BEGIN GENERATED CODE
//Cluster: LINEAR
//NonCoherentDMA
#define DMA_Flags 0x10020000
#define DMA_RdAddr 0x10020001
#define DMA_WrAddr 0x10020009
#define DMA_CopyLen 0x10020011
//StreamDMA
#define STREAM_DMA0_Flags 0x10020040
#define STREAM_DMA0_RdAddr 0x10020044
#define STREAM_DMA0_WrAddr 0x1002004c
#define STREAM_DMA0_RdFrameSize 0x10020054
#define STREAM_DMA0_NumRdFrames 0x10020058
#define STREAM_DMA0_RdFrameBufSize 0x10020059
#define STREAM_DMA0_WrFrameSize 0x1002005a
#define STREAM_DMA0_NumWrFrames 0x1002005e
#define STREAM_DMA0_WrFrameBufSize 0x1002005f
#define STREAM_DMA0_Stream 0x10020060
//Accelerator: TOP
#define TOP 0x10020080
//Accelerator: NORMALCONV
#define NORMALCONV 0x10020100
#define NormalConvWindow 0x10020140
#define NormalConvWeights 0x10020180
#define NormalConvQParams 0x10020440
#define NormalConvQBuffer 0x10020500
#define NormalConvOut 0x10020780
//Accelerator: DWCONV
#define DWCONV 0x100207c0
#define DWConvWindow 0x10020800
#define DWConvWeights 0x10020900
#define DWConvQParams 0x10020a00
#define DWConvQBuffer 0x10020ac0
#define DWConvOutBuffer 0x10021280
#define DWConvOut 0x100212c0
//Accelerator: PWCONV
#define PWCONV 0x10021300
#define PWConvLocalFeatSize 0x10021340
#define PWConvWeights 0x10021380
#define PWConvQParams 0x10021540
//END GENERATED CODE