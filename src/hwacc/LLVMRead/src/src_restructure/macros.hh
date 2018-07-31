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

#endif //__MACROS_HH__