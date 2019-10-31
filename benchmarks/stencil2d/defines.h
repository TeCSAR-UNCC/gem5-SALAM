#ifndef __DEFINES_H__
#define __DEFINES_H__

//#define SPM
//#define CHECK
#define TEST

#ifndef TEST
//Define input sizes
#define COL 64
#define ROW 128
#define FIL 9

//Data Bounds
#define TYPE int32_t
#define MAX 1000
#define MIN 1

//Set number of iterations to execute
#define MAX_ITERATION 1

#else
//Define input sizes
#define COL 64
#define ROW 128
#define FIL 9

//Data Bounds
#define TYPE int32_t
#define MAX 1000
#define MIN 1


//Set number of iterations to execute
#define MAX_ITERATION 1
#endif

#endif
