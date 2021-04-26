#include "body_defines.h"

void Residual() {
	volatile dType_8u * streamIn0 = (dType_8u *)ResIn0;
	volatile dType_8u * streamIn1 = (dType_8u *)ResIn1;
	volatile dType_8u * streamOut = (dType_8u *)ResOut;
	if (RES_0_ENABLE == 0) {
		for(int i=0; i< PW0_0_I_SIZE * PW0_0_I_SIZE * PW0_0_IC_SIZE; i++) {
			*streamOut = *streamIn0;
		}
	} else {
		for(int i=0; i< PW0_0_I_SIZE * PW0_0_I_SIZE * PW0_0_IC_SIZE; i++) {
			*streamOut = *streamIn0 + *streamIn1;
		}
	}
}