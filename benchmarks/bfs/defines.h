#ifndef __DEFINES_H__
#define __DEFINES_H__

#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>

//#define SPM
//#define CHECK

#define MAX_LEVEL INT8_MAX

#ifndef TEST
// Terminology (but not values) from graph500 spec
//   graph density = 2^-(2*SCALE - EDGE_FACTOR)
#define SCALE 8
#define EDGE_FACTOR 16

#define N_NODES (1LL<<SCALE)
#define N_EDGES (N_NODES*EDGE_FACTOR)

// upper limit
#define N_LEVELS 10
#endif

// Larger than necessary for small graphs, but appropriate for large ones
typedef uint64_t edge_index_t;
typedef uint64_t node_index_t;
typedef int8_t level_t;

#endif
