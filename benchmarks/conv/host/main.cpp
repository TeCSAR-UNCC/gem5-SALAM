#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "conv.h"
#include "relu.h"
#include "max_pool.h"
#include "combined.h"
#include "../../common/dma.h"
#include "../../common/m5ops.h"

// Memory Base Offsets
#define BASE            	0x80c00000
#define SPM_BASE        	0x2f100000
#define CONV_OFFSET			0x00000000
#define RELU_OFFSET			0x00100000
#define POOL_OFFSET			0x00200000
#define CHECK_OFFSET		0x00000040

// Multi Accelerator Offsets
#define IN_OFFSET     		0x00000000
#define KERN_OFFSET     	sizeof(TYPE)*(FM_HEIGHT*FM_WIDTH)
#define OUT_OFFSET    		sizeof(TYPE)*(KERN_HEIGHT*KERN_WIDTH)

// Single Accelerator Offsets
#define CONV_IN_OFFSET     	0x00000000
#define CONV_KERN_OFFSET    sizeof(TYPE)*(FM_HEIGHT*FM_WIDTH)
#define CONV_OUT_OFFSET    	KERN_OFFSET+(sizeof(TYPE)*KERN_HEIGHT*KERN_WIDTH)
#define RELU_IN_OFFSET    	CONV_OUT_OFFSET+sizeof(TYPE)*(FM_HEIGHT*FM_WIDTH)
#define RELU_OUT_OFFSET		RELU_IN_OFFSET+sizeof(TYPE)*(FM_HEIGHT*FM_WIDTH)
#define POOL_IN_OFFSET		RELU_OUT_OFFSET+sizeof(TYPE)*(FM_HEIGHT*FM_WIDTH)
#define POOL_OUT_OFFSET		POOL_IN_OFFSET+sizeof(TYPE)*(FM_HEIGHT*FM_WIDTH)

conv_struct convs;
relu_struct relus;
max_pool_struct pools;
comb_struct combs;

int main(void) {
	int i;  // General Loop Counter
	bool reluLaunch = true;
	bool poolLaunch = true;
/////////////////////////////////////////////////////////////////////////
	// Check Data
	TYPE *check				= (TYPE *)(CHECK_OFFSET);
/////////////////////////////////////////////////////////////////////////
#ifdef CPU_COPY
	// Conv 
	TYPE *conv_in       	= (TYPE *)(BASE+CONV_OFFSET+IN_OFFSET);
	TYPE *conv_kern     	= (TYPE *)(BASE+CONV_OFFSET+KERN_OFFSET);
	TYPE *conv_out  		= (TYPE *)(BASE+CONV_OFFSET+CONV_OUT_OFFSET);
	// Relu
	TYPE *relu_in      	 	= (TYPE *)(BASE+RELU_OFFSET+IN_OFFSET);
	TYPE *relu_out  		= (TYPE *)(BASE+RELU_OFFSET+OUT_OFFSET);
	// Max Pool
	TYPE *pool_in       	= (TYPE *)(BASE+POOL_OFFSET+IN_OFFSET);
	TYPE *pool_out  		= (TYPE *)(BASE+POOL_OFFSET+OUT_OFFSET);
#endif
#ifdef CPU_DIRECT
	// Conv 
	TYPE *conv_in       	= (TYPE *)(BASE+CONV_OFFSET+IN_OFFSET);
	TYPE *conv_kern     	= (TYPE *)(BASE+CONV_OFFSET+KERN_OFFSET);
	TYPE *conv_out  		= (TYPE *)(BASE+RELU_OFFSET+IN_OFFSET); 
	// Relu
	TYPE *relu_in      	 	= (TYPE *)(BASE+RELU_OFFSET+IN_OFFSET);
	TYPE *relu_out  		= (TYPE *)(BASE+POOL_OFFSET+IN_OFFSET); 
	// Max Pool
	TYPE *pool_in       	= (TYPE *)(BASE+POOL_OFFSET+IN_OFFSET);
	TYPE *pool_out  		= (TYPE *)(BASE+POOL_OFFSET+OUT_OFFSET);
#endif
#ifdef HW_MANAGED
	// Conv 
	TYPE *conv_in       	= (TYPE *)(BASE+CONV_OFFSET+IN_OFFSET);
	TYPE *conv_kern     	= (TYPE *)(BASE+CONV_OFFSET+KERN_OFFSET);
	TYPE *conv_out  		= (TYPE *)(BASE+RELU_OFFSET+IN_OFFSET); 
	// Relu
	TYPE *relu_in      	 	= (TYPE *)(BASE+RELU_OFFSET+IN_OFFSET);
	TYPE *relu_out  		= (TYPE *)(BASE+POOL_OFFSET+IN_OFFSET); 
	// Max Pool
	TYPE *pool_in       	= (TYPE *)(BASE+POOL_OFFSET+IN_OFFSET);
	TYPE *pool_out  		= (TYPE *)(BASE+POOL_OFFSET+OUT_OFFSET);
#endif
#ifdef SINGLE_ACC
	// Conv 
	TYPE *conv_in       	= (TYPE *)(BASE+CONV_IN_OFFSET);
	TYPE *conv_kern     	= (TYPE *)(BASE+CONV_KERN_OFFSET);
	TYPE *conv_out  		= (TYPE *)(BASE+CONV_OUT_OFFSET);
	// Relu
	TYPE *relu_in      	 	= (TYPE *)(BASE+RELU_IN_OFFSET);
	TYPE *relu_out  		= (TYPE *)(BASE+RELU_OUT_OFFSET);
	// Max Pool
	TYPE *pool_in       	= (TYPE *)(BASE+POOL_IN_OFFSET);
	TYPE *pool_out  		= (TYPE *)(BASE+POOL_OUT_OFFSET);
#endif
/////////////////////////////////////////////////////////////////////////
#ifdef SPM
	/////////////////////////////////////////////////////////////////////
	#ifdef CPU_COPY
		// Conv
		TYPE *conv_spm_in 		= (TYPE *)(SPM_BASE+CONV_OFFSET+IN_OFFSET);
		TYPE *conv_spm_kern    	= (TYPE *)(SPM_BASE+CONV_OFFSET+KERN_OFFSET);
		TYPE *conv_spm_out  	= (TYPE *)(SPM_BASE+CONV_OFFSET+CONV_OUT_OFFSET);
		// Relu
		TYPE *relu_spm_in 		= (TYPE *)(SPM_BASE+RELU_OFFSET+IN_OFFSET);
		TYPE *relu_spm_out  	= (TYPE *)(SPM_BASE+RELU_OFFSET+OUT_OFFSET);
		// Max Pool
		TYPE *pool_spm_in 		= (TYPE *)(SPM_BASE+POOL_OFFSET+IN_OFFSET);
		TYPE *pool_spm_out  	= (TYPE *)(SPM_BASE+POOL_OFFSET+OUT_OFFSET);
	#endif
	#ifdef CPU_DIRECT
		// Conv 
		TYPE *conv_spm_in       	= (TYPE *)(SPM_BASE+CONV_OFFSET+IN_OFFSET);
		TYPE *conv_spm_kern     	= (TYPE *)(SPM_BASE+CONV_OFFSET+KERN_OFFSET);
		TYPE *conv_spm_out  		= (TYPE *)(SPM_BASE+RELU_OFFSET+IN_OFFSET); 
		// Relu
		TYPE *relu_spm_in      	 	= (TYPE *)(SPM_BASE+RELU_OFFSET+IN_OFFSET);
		TYPE *relu_spm_out  		= (TYPE *)(SPM_BASE+POOL_OFFSET+IN_OFFSET); 
		// Max Pool
		TYPE *pool_spm_in       	= (TYPE *)(SPM_BASE+POOL_OFFSET+IN_OFFSET);
		TYPE *pool_spm_out  		= (TYPE *)(SPM_BASE+POOL_OFFSET+OUT_OFFSET);
	#endif
	#ifdef HW_MANAGED
		// Conv 
		TYPE *conv_spm_in       	= (TYPE *)(SPM_BASE+CONV_OFFSET+IN_OFFSET);
		TYPE *conv_spm_kern     	= (TYPE *)(SPM_BASE+CONV_OFFSET+KERN_OFFSET);
		TYPE *conv_spm_out  		= (TYPE *)(SPM_BASE+RELU_OFFSET+IN_OFFSET);
		// Relu
		TYPE *relu_spm_in      	 	= (TYPE *)(SPM_BASE+RELU_OFFSET+IN_OFFSET);
		TYPE *relu_spm_out  		= (TYPE *)(SPM_BASE+POOL_OFFSET+IN_OFFSET);
		// Max Pool
		TYPE *pool_spm_in       	= (TYPE *)(SPM_BASE+POOL_OFFSET+IN_OFFSET);
		TYPE *pool_spm_out  		= (TYPE *)(SPM_BASE+POOL_OFFSET+OUT_OFFSET);
	#endif
	#ifdef SINGLE_ACC
		// Conv 
		TYPE *conv_spm_in       	= (TYPE *)(SPM_BASE+CONV_IN_OFFSET);
		TYPE *conv_spm_kern     	= (TYPE *)(SPM_BASE+CONV_KERN_OFFSET);
		TYPE *conv_spm_out  		= (TYPE *)(SPM_BASE+CONV_OUT_OFFSET);
		// Relu
		TYPE *relu_spm_in      	 	= (TYPE *)(SPM_BASE+RELU_IN_OFFSET);
		TYPE *relu_spm_out  		= (TYPE *)(SPM_BASE+RELU_OUT_OFFSET);
		// Max Pool
		TYPE *pool_spm_in       	= (TYPE *)(SPM_BASE+POOL_IN_OFFSET);
		TYPE *pool_spm_out  		= (TYPE *)(SPM_BASE+POOL_OUT_OFFSET);
	#endif
	/////////////////////////////////////////////////////////////////////
#endif
/////////////////////////////////////////////////////////////////////////

	// Conv    
	convs.in       			= conv_in;
    convs.kern        		= conv_kern;
    convs.out  				= conv_out;
	convs.check				= check;
	// Relu    
	relus.in       			= relu_in;
    relus.out  				= relu_out;
	// Pool    
	pools.in       			= pool_in;
    pools.out  				= pool_out;
	// Combined
	combs.conv_in			= conv_in;
	combs.conv_kern			= conv_kern;
	combs.conv_out			= conv_out;
	combs.relu_in			= relu_in;
	combs.relu_out			= relu_out;
	combs.pool_in			= pool_in;
	combs.pool_out			= pool_out;

	// Generate Initial Convolution Data
#ifndef SINGLE_ACC
    printf("Generating Data:\n");
    genData(&convs);
    printf("Data Generated:\n");
#else
    printf("Generating Data:\n");
    genDataComb(&combs);
    printf("Data Generated:\n");
#endif

/////////////////////////////////////////////////////////////////////////
#ifndef SPM
	/////////////////////////////////////////////////////////////////////
	#ifdef CPU_COPY
		// Conv
		loc_conv_in       		= (uint64_t)(BASE+CONV_OFFSET+IN_OFFSET);
		loc_conv_kern        	= (uint64_t)(BASE+CONV_OFFSET+KERN_OFFSET);
		loc_conv_out  			= (uint64_t)(BASE+CONV_OFFSET+CONV_OUT_OFFSET);
		// Relu
		loc_relu_in       		= (uint64_t)(BASE+RELU_OFFSET+IN_OFFSET);
		loc_relu_out  			= (uint64_t)(BASE+RELU_OFFSET+OUT_OFFSET);
		// Pool
		loc_pool_in       		= (uint64_t)(BASE+POOL_OFFSET+IN_OFFSET);
		loc_pool_out  			= (uint64_t)(BASE+POOL_OFFSET+OUT_OFFSET);
	#endif
	#ifdef CPU_DIRECT
		// Conv 
		loc_conv_in       		= (uint64_t)(BASE+CONV_OFFSET+IN_OFFSET);
		loc_conv_kern     		= (uint64_t)(BASE+CONV_OFFSET+KERN_OFFSET);
		loc_conv_out  			= (uint64_t)(BASE+RELU_OFFSET+IN_OFFSET); 
		// Relu
		loc_relu_in      	 	= (uint64_t)(BASE+RELU_OFFSET+IN_OFFSET);
		loc_conv_out  			= (uint64_t)(BASE+POOL_OFFSET+IN_OFFSET); 
		// Max Pool
		loc_pool_in       		= (uint64_t)(BASE+POOL_OFFSET+IN_OFFSET);
		loc_pool_out  			= (uint64_t)(BASE+POOL_OFFSET+OUT_OFFSET);
	#endif
	#ifdef HW_MANAGED
		// Conv 
		loc_conv_in       		= (uint64_t)(BASE+CONV_OFFSET+IN_OFFSET);
		loc_conv_kern     		= (uint64_t)(BASE+CONV_OFFSET+KERN_OFFSET);
		loc_conv_out  			= (uint64_t)(BASE+RELU_OFFSET+IN_OFFSET); 
		// Relu
		loc_relu_in      	 	= (uint64_t)(BASE+RELU_OFFSET+IN_OFFSET);
		loc_conv_out  			= (uint64_t)(BASE+POOL_OFFSET+IN_OFFSET); 
		// Max Pool
		loc_pool_in       		= (uint64_t)(BASE+POOL_OFFSET+IN_OFFSET);
		loc_pool_out  			= (uint64_t)(BASE+POOL_OFFSET+OUT_OFFSET);
	#endif
	#ifdef SINGLE_ACC
		// Conv 
		loc_conv_in       		= (uint64_t)(BASE+CONV_IN_OFFSET);
		loc_conv_kern     		= (uint64_t)(BASE+CONV_KERN_OFFSET);
		loc_conv_out  			= (uint64_t)(BASE+CONV_OUT_OFFSET);
		// Relu
		loc_relu_in      	 	= (uint64_t)(BASE+RELU_IN_OFFSET);
		loc_conv_out  			= (uint64_t)(BASE+RELU_OUT_OFFSET);
		// Max Pool
		loc_pool_in       		= (uint64_t)(BASE+POOL_IN_OFFSET);
		loc_pool_out  			= (uint64_t)(BASE+POOL_OUT_OFFSET);
	#endif
	/////////////////////////////////////////////////////////////////////
#else // SPM Defined
/////////////////////////////////////////////////////////////////////////
	#ifdef CPU_COPY
	/////////////////////////////////////////////////////////////////////
		// Conv
		loc_conv_in       		= (uint64_t)(SPM_BASE+CONV_OFFSET+IN_OFFSET);
		loc_conv_kern        	= (uint64_t)(SPM_BASE+CONV_OFFSET+KERN_OFFSET);
		loc_conv_out  			= (uint64_t)(SPM_BASE+CONV_OFFSET+CONV_OUT_OFFSET);
		// Relu
		loc_relu_in       		= (uint64_t)(SPM_BASE+RELU_OFFSET+IN_OFFSET);
		loc_relu_out  			= (uint64_t)(SPM_BASE+RELU_OFFSET+OUT_OFFSET);
		// Pool
		loc_pool_in       		= (uint64_t)(SPM_BASE+POOL_OFFSET+IN_OFFSET);
		loc_pool_out  			= (uint64_t)(SPM_BASE+POOL_OFFSET+OUT_OFFSET);
	#endif
	#ifdef CPU_DIRECT
		// Conv 
		loc_conv_in       		= (uint64_t)(SPM_BASE+CONV_OFFSET+IN_OFFSET);
		loc_conv_kern     		= (uint64_t)(SPM_BASE+CONV_OFFSET+KERN_OFFSET);
		loc_conv_out  			= (uint64_t)(SPM_BASE+RELU_OFFSET+IN_OFFSET); 
		// Relu
		loc_relu_in      	 	= (uint64_t)(SPM_BASE+RELU_OFFSET+IN_OFFSET);
		loc_conv_out  			= (uint64_t)(SPM_BASE+POOL_OFFSET+IN_OFFSET); 
		// Max Pool
		loc_pool_in       		= (uint64_t)(SPM_BASE+POOL_OFFSET+IN_OFFSET);
		loc_pool_out  			= (uint64_t)(SPM_BASE+POOL_OFFSET+OUT_OFFSET);
	#endif
	#ifdef HW_MANAGED
		// Conv 
		loc_conv_in       		= (uint64_t)(SPM_BASE+CONV_OFFSET+IN_OFFSET);
		loc_conv_kern     		= (uint64_t)(SPM_BASE+CONV_OFFSET+KERN_OFFSET);
		loc_conv_out  			= (uint64_t)(SPM_BASE+RELU_OFFSET+IN_OFFSET); 
		// Relu
		loc_relu_in      	 	= (uint64_t)(SPM_BASE+RELU_OFFSET+IN_OFFSET);
		loc_conv_out  			= (uint64_t)(SPM_BASE+POOL_OFFSET+IN_OFFSET);
		// Max Pool
		loc_pool_in       		= (uint64_t)(SPM_BASE+POOL_OFFSET+IN_OFFSET);
		loc_pool_out  			= (uint64_t)(SPM_BASE+POOL_OFFSET+OUT_OFFSET);
	#endif
	#ifdef SINGLE_ACC
		// Conv 
		loc_conv_in       		= (uint64_t)(SPM_BASE+CONV_IN_OFFSET);
		loc_conv_kern     		= (uint64_t)(SPM_BASE+CONV_KERN_OFFSET);
		loc_conv_out  			= (uint64_t)(SPM_BASE+CONV_OUT_OFFSET);
		// Relu
		loc_relu_in      	 	= (uint64_t)(SPM_BASE+RELU_IN_OFFSET);
		loc_conv_out  			= (uint64_t)(SPM_BASE+RELU_OUT_OFFSET);
		// Max Pool
		loc_pool_in       		= (uint64_t)(SPM_BASE+POOL_IN_OFFSET);
		loc_pool_out  			= (uint64_t)(SPM_BASE+POOL_OUT_OFFSET);
	#endif
	/////////////////////////////////////////////////////////////////////	

	// Conv
	printf("Starting Convolution DMA Xfer:\n");
    dmacpy(	conv_spm_in, 		
			conv_in, 	
			sizeof(TYPE)*(FM_HEIGHT*FM_WIDTH));
	while(!pollDma());
    resetDma();
	printf("Convolution In Transfer Complete:\n");
    dmacpy(	conv_spm_kern, 	
			conv_kern, 	
			sizeof(TYPE)*(KERN_HEIGHT*KERN_WIDTH));
    while(!pollDma());
    resetDma();
	printf("Convolution DMA Xfer Finished:\n");

#endif
/////////////////////////////////////////////////////////////////////////
#ifdef CPU_COPY
	printf("Launching Accelerators, CPU Copy Mode:\n");
	
	// Conv
    printf("Launching Convolution Accelerator:\n");
    printf("Conv Status: %d\n", acc_conv);
	acc_conv = 0x01;
	printf("Conv Status: %d\n", acc_conv);

	while(0x04 != acc_pool) { 
		if((0x04 == acc_conv) && reluLaunch) {
			// Relu
			reluLaunch = false;
			memcpy(relu_in, conv_out, sizeof(TYPE)*(FM_HEIGHT*FM_WIDTH));
			acc_conv = 0x00;
			printf("Launching Rectifier Accelerator:\n");
			printf("Relu Status: %d\n", acc_relu);
			acc_relu = 0x01;
			printf("Relu Status: %d\n", acc_relu);
		}
		else if((0x04 == acc_relu) && poolLaunch) {
			// Relu
			poolLaunch = false;
			memcpy(pool_in, relu_out, sizeof(TYPE)*(FM_HEIGHT*FM_WIDTH));
			acc_relu = 0x00;			
			printf("Launching Pooling Accelerator:\n");
			printf("Relu Status: %d\n", acc_relu);
			acc_relu = 0x01;
			printf("Relu Status: %d\n", acc_relu);
		}
		else {
			printf("Conv Status: %d\n", acc_conv);
			printf("Relu Status: %d\n", acc_relu);
			printf("Pool Status: %d\n", acc_pool);
		}
	}
#endif
#ifdef CPU_DIRECT
	printf("Launching Accelerators, CPU Direct Xfer Mode:\n");
	
	// Conv
    printf("Launching Convolution Accelerator:\n");
    printf("Conv Status: %d\n", acc_conv);
	acc_conv = 0x01;
	printf("Conv Status: %d\n", acc_conv);

	while(0x04 != acc_pool) { 
		if((0x04 == acc_conv) && reluLaunch) {
			// Relu
			reluLaunch = false;
			acc_conv = 0x00;
			printf("Launching Rectifier Accelerator:\n");
			printf("Relu Status: %d\n", acc_relu);
			acc_relu = 0x01;
			printf("Relu Status: %d\n", acc_relu);
		}
		else if((0x04 == acc_relu) && poolLaunch) {
			// Relu
			poolLaunch = false;
			acc_relu = 0x00;
			printf("Launching Pooling Accelerator:\n");
			printf("Relu Status: %d\n", acc_relu);
			acc_relu = 0x01;
			printf("Relu Status: %d\n", acc_relu);
		}
		else {
			printf("Conv Status: %d\n", acc_conv);
			printf("Relu Status: %d\n", acc_relu);
			printf("Pool Status: %d\n", acc_pool);
		}
	}
#endif
#ifdef HW_MANAGED  // Ready Mode
	printf("Launching Accelerators In Ready Mode:\n");
	
	// Conv
    printf("Launching Convolution Accelerator:\n");
    printf("Conv Status: %d\n", acc_conv);
	acc_conv = 0x01;
	printf("Conv Status: %d\n", acc_conv);

	// Relu
    printf("Launching Rectifier Accelerator:\n");
    printf("Relu Status: %d\n", acc_relu);
	acc_relu = 0x01;
    printf("Relu Status: %d\n", acc_relu);

	// Pool
    printf("Launching Max Pooling Accelerator:\n");
    printf("Pool Status: %d\n", acc_pool);
    acc_pool = 0x01;
    printf("Pool Status: %d\n", acc_pool);

	while(0x04 != (acc_conv || acc_relu || acc_pool)) { 
		if (0x04 == acc_conv) acc_conv = 0x00;
		else if(0x04 == acc_relu) acc_relu = 0x00;
        printf("Conv Status: %d\n", acc_conv);
		printf("Relu Status: %d\n", acc_relu);
		printf("Pool Status: %d\n", acc_pool);
	}
#endif
#ifdef SINGLE_ACC
	printf("Launching Combined Accelerator\n");
	printf("Combo Acc Status: %d\n", acc_comb);
    acc_comb = 0x01;
    printf("Combo Acc Status: %d\n", acc_comb);
	while(0x04 != acc_comb) {
		printf("Combo Acc Status: %d\n", acc_comb);
	}

#endif
/////////////////////////////////////////////////////////////////////////
#ifdef SPM

	// Pool
	printf("Reading Max Pooling Data From Device:\n");
    dmacpy(pool_spm_out, 	conv_out, 	sizeof(TYPE)*((FM_HEIGHT/2)*(FM_WIDTH/2)));
    while(!pollDma());
    resetDma();

#endif
/////////////////////////////////////////////////////////////////////////
#ifdef CPU_COPY
    acc_pool = 0x00;
	if(!checkData(&convs)) {
	   
	}
#endif
#ifdef CPU_DIRECT
    acc_pool = 0x00;
	if(!checkData(&convs)) {
	   
	}
#endif
#ifdef HW_MANAGED
    acc_pool = 0x00;
	if(!checkData(&convs)) {
	   
	}
#endif
#ifdef SINGLE_ACC
    acc_comb = 0x00;
	if(!checkDataComb(&combs)) {
	   
	}
#endif
/////////////////////////////////////////////////////////////////////////
	m5_exit();
}

