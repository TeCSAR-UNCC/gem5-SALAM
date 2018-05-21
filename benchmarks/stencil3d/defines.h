//#define SPM
//#define TEST

#define C_SIZE  2

#ifndef TEST
//Define input sizes
#define HYT     32
#define COL     32
#define ROW     16
//Data Bounds
#define TYPE int32_t
#define MAX 1000
#define MIN 1
//Convenience Macros
#define SIZE (ROW * COL * HYT)
#define INDX(_row_size,_col_size,_i,_j,_k) ((_i)+_row_size*((_j)+_col_size*(_k)))

#else
//Define input sizes
#define HYT     32
#define COL     32
#define ROW     16
//Data Bounds
#define TYPE int32_t
#define MAX 1000
#define MIN 1
//Convenience Macros
#define SIZE (ROW * COL * HYT)
#define INDX(_row_size,_col_size,_i,_j,_k) ((_i)+_row_size*((_j)+_col_size*(_k)))
#endif
