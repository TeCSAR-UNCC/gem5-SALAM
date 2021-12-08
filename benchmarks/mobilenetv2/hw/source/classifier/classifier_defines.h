#include "../common/support.h"
#include "../../../classifier_hw_defines.h"
#include "../../../network_params.h"
/***********************************************************
 * Computation Defines
 ***********************************************************/

// Linear (Fully Connected)
#define INPUT_SIZE		__LINEAR_DEPTH_SIZE__
#define OUTPUT_SIZE		__LINEAR_ROW_SIZE__
#define WeightSize		INPUT_SIZE*OUTPUT_SIZE
#define BIAS_ZP			0
#define INPUT_ZP		0
#define OUTPUT_ZP		0
#define QParamSize		1000