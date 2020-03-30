#include "tail_defines.h"

#define t_OUTPUT_CHANNEL	RS_OUT_CH
#define t_OUTPUT_SIZE		RS_OUT_SIZE

void reshape() {
	volatile dType_8u * cin_0 		= (dType_8u *)(RSIn);
	volatile dType_8u * channelwise = (dType_8u *)(RSOut);

	for (dType_Reg i = 0; i < t_OUTPUT_SIZE; i++)
    {
        for (dType_Reg j = 0; j < t_OUTPUT_SIZE; j++)
        {
            for (dType_Reg c = 0; c < t_OUTPUT_CHANNEL; c++)
            {
                channelwise[(c * (t_OUTPUT_SIZE * t_OUTPUT_SIZE)) + (i * t_OUTPUT_SIZE + j)] = *cin_0;
            }
        }
    }
}