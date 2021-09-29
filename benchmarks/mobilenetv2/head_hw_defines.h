//BEGIN GENERATED CODE
//Cluster: CLASSIFIER_LINEAR
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
//Accelerator: HEAD_TOP
#define HEAD_TOP 0x10020080
//Accelerator: HEAD_NORMALCONV
#define HEAD_NORMALCONV 0x10020100
#define head_NormalConvWindow 0x10020140
#define head_NormalConvWeights 0x10020180
#define head_NormalConvBias 0x10020440
#define head_NormalConvIMultBias 0x10020480
#define head_NormalConvNShiftBias 0x100204c0
#define head_NormalConvIMultOut 0x10020500
#define head_NormalConvNShiftOut 0x10020540
#define head_NormalConvWeightZP 0x10020580
#define head_NormalConvQBuffer 0x100205c0
#define head_NormalConvOut 0x10020840
//Accelerator: HEAD_DWCONV
#define HEAD_DWCONV 0x10020880
#define head_DWConvWindow 0x100208c0
#define head_DWConvWeights 0x100209c0
#define head_DWConvBias 0x10020ac0
#define head_DWConvIMultBias 0x10020b00
#define head_DWConvNShiftBias 0x10020b40
#define head_DWConvIMultOut 0x10020b80
#define head_DWConvNShiftOut 0x10020bc0
#define head_DWConvWeightZP 0x10020c00
#define head_DWConvQBuffer 0x10020c40
#define head_DWConvOutBuffer 0x10021400
#define head_DWConvOut 0x10021440
//Accelerator: HEAD_PWCONV
#define HEAD_PWCONV 0x10021480
#define head_PWConvLocalFeats 0x100214c0
#define head_PWConvWeights 0x10021500
#define head_PWConvBias 0x100216c0
#define head_PWConvIMultBias 0x10021700
#define head_PWConvNShiftBias 0x10021740
#define head_PWConvIMultOut 0x10021780
#define head_PWConvNShiftOut 0x100217c0
#define head_PWConvWeightZP 0x10021800
//END GENERATED CODE