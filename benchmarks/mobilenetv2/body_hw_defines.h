//BEGIN GENERATED CODE
//Cluster: HEAD_PWCONV
//NonCoherentDMA
#define BODY_DMA_Flags 0x10021a00
#define BODY_DMA_RdAddr 0x10021a01
#define BODY_DMA_WrAddr 0x10021a09
#define BODY_DMA_CopyLen 0x10021a11
//StreamDMA
#define BODY_STREAM_DMA0_Flags 0x10021a40
#define BODY_STREAM_DMA0_RdAddr 0x10021a44
#define BODY_STREAM_DMA0_WrAddr 0x10021a4c
#define BODY_STREAM_DMA0_RdFrameSize 0x10021a54
#define BODY_STREAM_DMA0_NumRdFrames 0x10021a58
#define BODY_STREAM_DMA0_RdFrameBufSize 0x10021a59
#define BODY_STREAM_DMA0_WrFrameSize 0x10021a5a
#define BODY_STREAM_DMA0_NumWrFrames 0x10021a5e
#define BODY_STREAM_DMA0_WrFrameBufSize 0x10021a5f
#define BODY_STREAM_DMA0_Stream 0x10021a60
//StreamDMA
#define BODY_STREAM_DMA1_Flags 0x10021a80
#define BODY_STREAM_DMA1_RdAddr 0x10021a84
#define BODY_STREAM_DMA1_WrAddr 0x10021a8c
#define BODY_STREAM_DMA1_RdFrameSize 0x10021a94
#define BODY_STREAM_DMA1_NumRdFrames 0x10021a98
#define BODY_STREAM_DMA1_RdFrameBufSize 0x10021a99
#define BODY_STREAM_DMA1_WrFrameSize 0x10021a9a
#define BODY_STREAM_DMA1_NumWrFrames 0x10021a9e
#define BODY_STREAM_DMA1_WrFrameBufSize 0x10021a9f
#define BODY_STREAM_DMA1_Stream 0x10021aa0
//Accelerator: BODY_TOP
#define BODY_TOP 0x10021ac0
//Accelerator: BODY_RESIDUAL
#define BODY_RESIDUAL 0x10021b40
#define body_ResidualOut 0x10021b80
//Accelerator: BODY_PWCONV0
#define BODY_PWCONV0 0x10021bc0
#define body_PWConv0LocalFeats 0x10021c00
#define body_PWConv0Weights 0x10021c80
#define body_PW0Bias 0x10036e40
#define body_PW0IMultBias 0x10037140
#define body_PW0NShiftBias 0x10037440
#define body_PW0IMultOut 0x10037740
#define body_PW0NShiftOut 0x10037a40
#define body_PW0WeightZP 0x10037d40
#define body_PWConv0Out 0x10038040
//Accelerator: BODY_DWCONV
#define BODY_DWCONV 0x10038080
#define body_DWConvWindow 0x10038100
#define body_DWConvWeights 0x10039a80
#define body_DWBias 0x1003b6c0
#define body_DWIMultBias 0x1003b9c0
#define body_DWNShiftBias 0x1003bcc0
#define body_DWIMultOut 0x1003bfc0
#define body_DWNShiftOut 0x1003c2c0
#define body_DWWeightZP 0x1003c5c0
#define body_DWConvBuffer 0x1003c8c0
#define body_DWConvOutBuffer 0x1004d700
#define body_DWConvOut 0x1004da00
//Accelerator: BODY_PWCONV1
#define BODY_PWCONV1 0x1004da40
#define body_PWConv1LocalFeats 0x1004da80
#define body_PWConv1Weights 0x1004dd80
#define body_PW1Bias 0x100780c0
#define body_PW1IMultBias 0x100781c0
#define body_PW1NShiftBias 0x100782c0
#define body_PW1IMultOut 0x100783c0
#define body_PW1NShiftOut 0x100784c0
#define body_PW1WeightZP 0x100785c0
//END GENERATED CODE