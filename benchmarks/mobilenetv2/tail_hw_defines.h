//BEGIN GENERATED CODE
//Cluster: BODY_PWCONV1
//NonCoherentDMA
#define TAIL_DMA_Flags 0x100b1cc0
#define TAIL_DMA_RdAddr 0x100b1cc1
#define TAIL_DMA_WrAddr 0x100b1cc9
#define TAIL_DMA_CopyLen 0x100b1cd1
//StreamDMA
#define TAIL_STREAM_DMA0_Flags 0x100b1d00
#define TAIL_STREAM_DMA0_RdAddr 0x100b1d04
#define TAIL_STREAM_DMA0_WrAddr 0x100b1d0c
#define TAIL_STREAM_DMA0_RdFrameSize 0x100b1d14
#define TAIL_STREAM_DMA0_NumRdFrames 0x100b1d18
#define TAIL_STREAM_DMA0_RdFrameBufSize 0x100b1d19
#define TAIL_STREAM_DMA0_WrFrameSize 0x100b1d1a
#define TAIL_STREAM_DMA0_NumWrFrames 0x100b1d1e
#define TAIL_STREAM_DMA0_WrFrameBufSize 0x100b1d1f
#define TAIL_STREAM_DMA0_Stream 0x100b1d20
//Accelerator: TAIL_TOP
#define TAIL_TOP 0x100b1d40
//Accelerator: TAIL_PWCONV
#define TAIL_PWCONV 0x100b1dc0
#define tail_PWConvLocalFeats 0x100b1e00
#define tail_PWConvWeights 0x100b1f80
#define tail_PWBias 0x10115fc0
#define tail_PWIMultBias 0x10116500
#define tail_PWNShiftBias 0x10116a40
#define tail_PWIMultOut 0x10116f80
#define tail_PWNShiftOut 0x101174c0
#define tail_PWWeightZP 0x10117a00
#define tail_PWConvOut 0x10117f40
//Accelerator: TAIL_RESHAPE
#define TAIL_RESHAPE 0x10117f80
#define tail_ReshapeOut 0x10117fc0
//Accelerator: TAIL_AVGPOOL
#define TAIL_AVGPOOL 0x1011ad00
//END GENERATED CODE