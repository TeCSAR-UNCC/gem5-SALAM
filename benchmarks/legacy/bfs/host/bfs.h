#ifndef DEFINES
#include "../defines.h"
#endif
#include "data.h"

#define common_val      *(unsigned *)0x80b00000
#define ACC             *(char *)0x2f000000
#define NODES_ADDR      *(int *)0x2f000001
#define EDGES_ADDR      *(int *)0x2f000009
#define START_ADDR      *(int *)0x2f000011
#define LEVEL_ADDR      *(int *)0x2f000019
#define COUNT_ADDR      *(int *)0x2f000021

typedef struct {
    node_index_t * nodes;
    edge_index_t * edges;
    node_index_t * starting_node;
    level_t      * level;
    edge_index_t * level_counts;
    edge_index_t * check;
} bfs_struct;

int checkData(bfs_struct * bfs) {
    int i;
    for (i = 0; i < N_LEVELS; i++) {
        if (bfs->level_counts[i] != bfs->check[i]) {
            printf("Check Failed!\n");
            return 0;
        }
    }
    printf("Check Passed!\n");
    return 1;
}

void genData(bfs_struct * bfs) {
    int i;

    *(bfs->starting_node) = start;
    for (i = 0; i < 2*N_NODES; i++) {
        bfs->nodes[i] = nod[i];
    }
    for (i = 0; i < N_EDGES; i++) {
        bfs->edges[i] = edg[i];
    }
    for (i = 0; i < N_LEVELS; i++) {
        bfs->check[i] = chk[i];
    }
    for (i = 0; i < N_NODES; i++) {
        bfs->level[i] = MAX_LEVEL;
    }
}
