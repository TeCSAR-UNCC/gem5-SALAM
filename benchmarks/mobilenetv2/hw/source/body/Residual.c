#include "body_defines.h"

void Residual(uint8_t enable, uint32_t length) {
	volatile dType_8u * streamIn0 = (dType_8u *)ResIn0;
	volatile dType_8u * streamIn1 = (dType_8u *)ResIn1;
	volatile dType_8u * streamOut = (dType_8u *)ResOut;
		for(int i=0; i<length; i++) {
			*streamOut = *streamIn0;
		}
}