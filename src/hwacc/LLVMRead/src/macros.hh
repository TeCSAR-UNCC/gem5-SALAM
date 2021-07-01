#ifndef MACROS_HH
#define MACROS_HH
//____________________________________________________________________________



/**
 * Finds the minimal value of arguments
 */ 
#define MIN(a,b) (((a)<(b))?(a):(b))


/**
 * @namespace SALAM
 */

namespace SALAM {
    // LLVM comparison codes
    enum Predicate : unsigned {
        FCMP_FALSE = 0, FCMP_OEQ = 1, FCMP_OGT = 2, FCMP_OGE = 3,
        FCMP_OLT = 4, FCMP_OLE = 5, FCMP_ONE = 6, FCMP_ORD = 7,
        FCMP_UNO = 8, FCMP_UEQ = 9, FCMP_UGT = 10, FCMP_UGE = 11,
        FCMP_ULT = 12, FCMP_ULE = 13, FCMP_UNE = 14, FCMP_TRUE = 15,
        FIRST_FCMP_PREDICATE = FCMP_FALSE, LAST_FCMP_PREDICATE = FCMP_TRUE, BAD_FCMP_PREDICATE = FCMP_TRUE + 1, ICMP_EQ = 32,
        ICMP_NE = 33, ICMP_UGT = 34, ICMP_UGE = 35, ICMP_ULT = 36,
        ICMP_ULE = 37, ICMP_SGT = 38, ICMP_SGE = 39, ICMP_SLT = 40,
        ICMP_SLE = 41, FIRST_ICMP_PREDICATE = ICMP_EQ, LAST_ICMP_PREDICATE = ICMP_SLE, BAD_ICMP_PREDICATE = ICMP_SLE + 1
    };
}


/*
Useful snippets

// Line break
//____________________________________________________________________________





*/

#endif //__MACROS_HH__
