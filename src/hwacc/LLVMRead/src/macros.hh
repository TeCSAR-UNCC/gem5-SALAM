#ifndef __MACROS_HH__
#define __MACROS_HH__

// Data type sizing based off LLVM references
#define SYSTEMSIZE      64  // Bit size of system 
#define DEFAULTSIZE     64  // Default register size
#define BYTE            8   // Size of a byte in bits
#define VOIDSIZE        0   // Void data type size
#define BYTESIZE(X)     (((X-1)/8)+1)       // Convert bits to bytes
#define POINTERSIZE     (SYSTEMSIZE/BYTE)   // Pointer data type size
#define LABELSIZE       (SYSTEMSIZE/BYTE)   // Label data type size
#define DOUBLESIZE      (SYSTEMSIZE/BYTE)   // Double data type size
#define FLOATSIZE       (SYSTEMSIZE/(2*BYTE)) // Float data type size

// Indexing
#define SKIPFIRST       1   // Start string on second character
#define COMPAREFOUND    0   // Index 0 returned if entire string is found

// Keyword Definition
// Hardware Units
#define    INTADDER 0
#define    INTMULTI 1
#define    BITWISE 2
#define    SHIFTER 3
#define    FPSPADDER 4
#define    FPSPMULTI 5
#define    FPDPADDER 6
#define    FPDPMULTI 7

#define ADDUNIT 0
#define MULUNIT 1
#define BITUNIT 2
#define SHIFTUNIT 3

//Data Types
#define INTEGER 0
#define FLOAT 1
#define DOUBLE 2
#define CUSTOM 3

//Debug Flags
#define SystemSize 64
#define DEBUG 1
#define DEBUGPARAMS 1
#define DEPENDENCYLIST 1
#define PRINTREGISTERS 1


//Cycle Counts
#define CYCLECOUNTRET				1
#define CYCLECOUNTBR				1
#define CYCLECOUNTSWITCH			1
#define CYCLECOUNTINDIRECTBR		1
#define CYCLECOUNTINVOKE			1
#define CYCLECOUNTRESUME			1
#define CYCLECOUNTUNREACHABLE		1

#define CYCLECOUNTADD				1
#define CYCLECOUNTSUB				1
#define CYCLECOUNTMUL				1
#define CYCLECOUNTUDIV				1
#define CYCLECOUNTSDIV				1
#define CYCLECOUNTUREM				1
#define CYCLECOUNTSREM				1

#define CYCLECOUNTFADD				5
#define CYCLECOUNTFSUB				5
#define CYCLECOUNTFMUL				4
#define CYCLECOUNTFDIV				16
#define CYCLECOUNTFREM				5

#define CYCLECOUNTSHL				1
#define CYCLECOUNTLSHR				1
#define CYCLECOUNTASHR				1
#define CYCLECOUNTAND				1
#define CYCLECOUNTOR				1
#define CYCLECOUNTXOR				1
	
#define CYCLECOUNTEXTRACTELEMENT	1
#define CYCLECOUNTINSERTELEMENT		1
#define CYCLECOUNTSHUFFLEVECTOR		1

#define CYCLECOUNTEXTRACTVALUE		1
#define CYCLECOUNTINSERTVALUE		1

#define CYCLECOUNTALLOCA			1
#define CYCLECOUNTLOAD				1
#define CYCLECOUNTSTORE				1
#define CYCLECOUNTFENCE				1
#define CYCLECOUNTCMPXCHG			1
#define CYCLECOUNTATOMICRMW			1
#define CYCLECOUNTGETELEMENTPTR		1

#define CYCLECOUNTTRUNC				1
#define CYCLECOUNTZEXT				1
#define CYCLECOUNTSEXT				1
#define CYCLECOUNTFPTRUNC			1
#define CYCLECOUNTFPEXT				1
#define CYCLECOUNTFPTOUI			1
#define CYCLECOUNTFPTOSI			1
#define CYCLECOUNTUITOFP			1
#define CYCLECOUNTSITOFP			1
#define CYCLECOUNTPTRTOINT			1
#define CYCLECOUNTINTTOPTR			1
#define CYCLECOUNTBITCAST			1
#define CYCLECOUNTADDRSPACECAST		1

#define CYCLECOUNTICMP				1
#define CYCLECOUNTFCMP				1
#define CYCLECOUNTPHI				1
#define CYCLECOUNTSELECT			1
#define CYCLECOUNTCALL				1
#define CYCLECOUNTVAARG				1
#define CYCLECOUNTLANDINGPAD		1
#define CYCLECOUNTCATCHPAD			1

#endif //__MACROS_HH__
