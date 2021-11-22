//BEGIN GENERATED CODE
//Cluster: BODY_PWCONV1
//NonCoherentDMA
#define TAIL_DMA_Flags 0x10078700
#define TAIL_DMA_RdAddr 0x10078701
#define TAIL_DMA_WrAddr 0x10078709
#define TAIL_DMA_CopyLen 0x10078711
//StreamDMA
#define TAIL_STREAM_DMA0_Flags 0x10078740
#define TAIL_STREAM_DMA0_RdAddr 0x10078744
#define TAIL_STREAM_DMA0_WrAddr 0x1007874c
#define TAIL_STREAM_DMA0_RdFrameSize 0x10078754
#define TAIL_STREAM_DMA0_NumRdFrames 0x10078758
#define TAIL_STREAM_DMA0_RdFrameBufSize 0x10078759
#define TAIL_STREAM_DMA0_WrFrameSize 0x1007875a
#define TAIL_STREAM_DMA0_NumWrFrames 0x1007875e
#define TAIL_STREAM_DMA0_WrFrameBufSize 0x1007875f
#define TAIL_STREAM_DMA0_Stream 0x10078760
//Accelerator: TAIL_TOP
#define TAIL_TOP 0x10078780
//Accelerator: TAIL_PWCONV
#define TAIL_PWCONV 0x10078800
#define tail_PWConvLocalFeats 0x10078840
#define tail_PWConvWeights 0x10078940
#define tail_PWBias 0x100c3980
#define tail_PWIMultBias 0x100c3ec0
#define tail_PWNShiftBias 0x100c4400
#define tail_PWIMultOut 0x100c4940
#define tail_PWNShiftOut 0x100c4e80
#define tail_PWWeightZP 0x100c53c0
#define tail_PWConvOut 0x100c5900
//Accelerator: TAIL_RESHAPE
#define TAIL_RESHAPE 0x100c5940
#define tail_ReshapeOut 0x100c5980
//Accelerator: TAIL_AVGPOOL
#define TAIL_AVGPOOL 0x100cd6c0
//END GENERATED CODE