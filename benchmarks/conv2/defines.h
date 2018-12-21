#include <stdlib.h>

#define TYPE			double

#define FM_WIDTH 		256
#define FM_HEIGHT 		256
#define KERN_WIDTH 		3
#define KERN_HEIGHT 	3

#define SPM

//#define CPU_COPY
//#define CPU_DIRECT
//#define HW_MANAGED
#define SINGLE_ACC

#define RAND_SEED 		5

#define MAX(a, b) ((a)>(b)? (a) : (b))
#define UNIFORM() (((double) rand())/RAND_MAX)
