//BEGIN GENERATED CODE
//Cluster: HEAD_PWCONV
//NonCoherentDMA
#define BODY_DMA_Flags 0x10021f00
#define BODY_DMA_RdAddr 0x10021f01
#define BODY_DMA_WrAddr 0x10021f09
#define BODY_DMA_CopyLen 0x10021f11
//StreamDMA
#define BODY_STREAM_DMA0_Flags 0x10021f40
#define BODY_STREAM_DMA0_RdAddr 0x10021f44
#define BODY_STREAM_DMA0_WrAddr 0x10021f4c
#define BODY_STREAM_DMA0_RdFrameSize 0x10021f54
#define BODY_STREAM_DMA0_NumRdFrames 0x10021f58
#define BODY_STREAM_DMA0_RdFrameBufSize 0x10021f59
#define BODY_STREAM_DMA0_WrFrameSize 0x10021f5a
#define BODY_STREAM_DMA0_NumWrFrames 0x10021f5e
#define BODY_STREAM_DMA0_WrFrameBufSize 0x10021f5f
#define BODY_STREAM_DMA0_Stream 0x10021f60
//StreamDMA
#define BODY_STREAM_DMA1_Flags 0x10021f80
#define BODY_STREAM_DMA1_RdAddr 0x10021f84
#define BODY_STREAM_DMA1_WrAddr 0x10021f8c
#define BODY_STREAM_DMA1_RdFrameSize 0x10021f94
#define BODY_STREAM_DMA1_NumRdFrames 0x10021f98
#define BODY_STREAM_DMA1_RdFrameBufSize 0x10021f99
#define BODY_STREAM_DMA1_WrFrameSize 0x10021f9a
#define BODY_STREAM_DMA1_NumWrFrames 0x10021f9e
#define BODY_STREAM_DMA1_WrFrameBufSize 0x10021f9f
#define BODY_STREAM_DMA1_Stream 0x10021fa0
//Accelerator: BODY_TOP
#define BODY_TOP 0x10021fc0
//Accelerator: BODY_RESIDUAL
#define BODY_RESIDUAL 0x10022040
#define body_ResidualOut 0x10022080
//Accelerator: BODY_PWCONV0
#define BODY_PWCONV0 0x100220c0
#define body_PWConv0LocalFeats 0x10022100
#define body_PWConv0Weights 0x100221c0
#define body_PW0Bias 0x10047a00
#define body_PW0IMultBias 0x10047e00
#define body_PW0NShiftBias 0x10048200
#define body_PW0IMultOut 0x10048600
#define body_PW0NShiftOut 0x10048a00
#define body_PW0WeightZP 0x10048e00
#define body_PWConv0Out 0x10049200
//Accelerator: BODY_DWCONV
#define BODY_DWCONV 0x10049240
#define body_DWConvWindow 0x100492c0
#define body_DWConvWeights 0x1004b4c0
#define body_DWBias 0x1004da80
#define body_DWIMultBias 0x1004de80
#define body_DWNShiftBias 0x1004e280
#define body_DWIMultOut 0x1004e680
#define body_DWNShiftOut 0x1004ea80
#define body_DWWeightZP 0x1004ee80
#define body_DWConvBuffer 0x1004f280
#define body_DWConvOutBuffer 0x10065ac0
#define body_DWConvOut 0x10065ec0
//Accelerator: BODY_PWCONV1
#define BODY_PWCONV1 0x10065f00
#define body_PWConv1LocalFeats 0x10065f40
#define body_PWConv1Weights 0x10066340
#define body_PW1Bias 0x100b1380
#define body_PW1IMultBias 0x100b1500
#define body_PW1NShiftBias 0x100b1680
#define body_PW1IMultOut 0x100b1800
#define body_PW1NShiftOut 0x100b1980
#define body_PW1WeightZP 0x100b1b00
//END GENERATED CODE