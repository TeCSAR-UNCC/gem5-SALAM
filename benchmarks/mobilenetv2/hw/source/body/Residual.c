#include "body_defines.h"

void Residual(uint64_t enable, uint64_t length) {
	volatile dType_8u * streamIn0 = (dType_8u *)ResIn0;
	volatile dType_8u * streamIn1 = (dType_8u *)ResIn1;
	volatile dType_8u * streamOut = (dType_8u *)ResOut;
	
	// Checks that Sam hates™
	// if(enable != RES_0_ENABLE) return;
	// if(length != PW0_0_I_SIZE * PW0_0_I_SIZE * PW0_0_IC_SIZE) return;
	
	if (enable == 0) {
		for(int i=0; i < length; i++) {
			*streamOut = *streamIn0;
		}
	} else {
		for(int i=0; i < length; i++) {
			*streamOut = *streamIn0 + *streamIn1;
		}
	}
}