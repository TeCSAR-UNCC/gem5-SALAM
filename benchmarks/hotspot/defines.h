#ifndef __DEFINES_H__
#define __DEFINES_H__

#include "stdint.h"

#define SPM
#define CHECK
#define TEST

#ifdef TEST
#define STR_SIZE	256
/* maximum power density possible (say 300W for a 10mm x 10mm chip)	*/
#define MAX_PD	        (3.0e6)
/* required precision in degrees	*/
#define PRECISION       0.001
#define SPEC_HEAT_SI    1.75e6
#define K_SI            100
/* capacitance fitting factor	*/
#define FACTOR_CHIP	    0.5
//#define NUM_THREAD 4
#define GRID_ROWS       64
#define GRID_COLS       64
#define SIM_TIME        2
#define NUM_THREADS     4

#else
#define STR_SIZE	256
/* maximum power density possible (say 300W for a 10mm x 10mm chip)	*/
#define MAX_PD	        (3.0e6)
/* required precision in degrees	*/
#define PRECISION       0.001
#define SPEC_HEAT_SI    1.75e6
#define K_SI            100
/* capacitance fitting factor	*/
#define FACTOR_CHIP	    0.5
//#define NUM_THREAD 4
#define GRID_ROWS       64
#define GRID_COLS       64
#define SIM_TIME        2
#define NUM_THREADS     4

#endif
#endif
