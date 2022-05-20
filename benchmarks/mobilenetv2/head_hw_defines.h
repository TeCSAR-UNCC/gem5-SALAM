//BEGIN GENERATED CODE
//Cluster: HEAD
//NonCoherentDMA
#define HEAD_DMA_Flags 0x10020000
#define HEAD_DMA_RdAddr 0x10020001
#define HEAD_DMA_WrAddr 0x10020009
#define HEAD_DMA_CopyLen 0x10020011
//StreamDMA
#define HEAD_STREAM_DMA0_Flags 0x10020040
#define HEAD_STREAM_DMA0_RdAddr 0x10020044
#define HEAD_STREAM_DMA0_WrAddr 0x1002004c
#define HEAD_STREAM_DMA0_RdFrameSize 0x10020054
#define HEAD_STREAM_DMA0_NumRdFrames 0x10020058
#define HEAD_STREAM_DMA0_RdFrameBufSize 0x10020059
#define HEAD_STREAM_DMA0_WrFrameSize 0x1002005a
#define HEAD_STREAM_DMA0_NumWrFrames 0x1002005e
#define HEAD_STREAM_DMA0_WrFrameBufSize 0x1002005f
#define HEAD_STREAM_DMA0_Stream 0x10020060
#define HEAD_STREAM_DMA0_Status 0x10020080
//Accelerator: HEAD_TOP
#define HEAD_TOP 0x100200c0
//Accelerator: HEAD_NORMALCONV
#define HEAD_NORMALCONV 0x10020140
#define head_NormalConvWindow 0x10020180
#define head_NormalConvWeights 0x100201c0
#define head_NormalConvBias 0x10020380
#define head_NormalConvIMultBias 0x100203c0
#define head_NormalConvNShiftBias 0x10020400
#define head_NormalConvIMultOut 0x10020440
#define head_NormalConvNShiftOut 0x10020480
#define head_NormalConvWeightZP 0x100204c0
#define head_NormalConvQBuffer 0x10020500
#define head_NormalConvOut 0x10020780
#define head_NormalConvOut_Status 0x100207c0
//Accelerator: HEAD_DWCONV
#define HEAD_DWCONV 0x10020800
#define head_DWConvWindow 0x10020840
#define head_DWConvWeights 0x10020900
#define head_DWConvBias 0x100209c0
#define head_DWConvIMultBias 0x10020a00
#define head_DWConvNShiftBias 0x10020a40
#define head_DWConvIMultOut 0x10020a80
#define head_DWConvNShiftOut 0x10020ac0
#define head_DWConvWeightZP 0x10020b00
#define head_DWConvQBuffer 0x10020b40
#define head_DWConvOutBuffer 0x10021180
#define head_DWConvOut 0x100211c0
#define head_DWConvOut_Status 0x10021200
//Accelerator: HEAD_PWCONV
#define HEAD_PWCONV 0x10021240
#define head_PWConvLocalFeats 0x10021280
#define head_PWConvWeights 0x100212c0
#define head_PWConvBias 0x10021380
#define head_PWConvIMultBias 0x100213c0
#define head_PWConvNShiftBias 0x10021400
#define head_PWConvIMultOut 0x10021440
#define head_PWConvNShiftOut 0x10021480
#define head_PWConvWeightZP 0x100214c0
//END GENERATED CODE