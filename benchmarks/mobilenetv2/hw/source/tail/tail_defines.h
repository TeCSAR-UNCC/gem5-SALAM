#include "../common/support.h"
#include "../../../tail_hw_defines.h"
#include "../../../network_params.h"

/***********************************************************
 * Computation Defines
 ***********************************************************/

// PW Conv
#define PW_IN_SIZE		__TAIL_PW_CONV_MAX_INPUT_SIZE__
#define PW_OUT_SIZE		3
#define PW_IN_CH		__TAIL_PW_CONV_MAX_INPUT_CHAN__
#define PW_OUT_CH		__TAIL_PW_CONV_MAX_OUTPUT_CHANNEL__
#define PW_CORE_SIZE	120
#define PW_BIAS_ZP		0
#define PW_INPUT_ZP		0
#define PW_OUTPUT_ZP	0
#define PWWeightSize	PW_OUT_CH*PW_IN_CH
#define PWQParamSize	PW_OUT_CH
#define PWOutputSize	PW_OUT_SIZE*PW_OUT_SIZE*PW_OUT_CH

// Reshape (Matrix Transpose)
#define RS_OUT_SIZE		3
#define RS_OUT_CH		__TAIL_PW_CONV_MAX_OUTPUT_CHANNEL__

// Avg Pooling
#define POOL_CH			__TAIL_PW_CONV_MAX_OUTPUT_CHANNEL__
#define POOL_SIZE		3
#define POOL_IMULT		__AVG_POOLING_iMult_KERNEL__
#define POOL_NSHIFT		__AVG_POOLING__nShift_KERNEL__

// StreamDMA
#define INPUT_SIZE		PW_IN_SIZE*PW_IN_SIZE*PW_IN_CH
#define OUTPUT_SIZE		POOL_CH

#define PW_OUT_CH_MAX __TAIL_PW_CONV_MAX_OUTPUT_CHANNEL__