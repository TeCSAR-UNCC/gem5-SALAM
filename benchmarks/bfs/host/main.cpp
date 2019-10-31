#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "bfs.h"
#include "../../common/dma.h"
#include "../../common/m5ops.h"

bfs_struct bfs;

#define BASE            0x80c00000
#define SPM_BASE        0x2f100000

#define NODES_OFFSET 0
#define EDGES_OFFSET NODES_OFFSET + sizeof(node_index_t) * N_NODES * 2
#define START_OFFSET EDGES_OFFSET + sizeof(edge_index_t) * N_EDGES
#define LEVEL_OFFSET START_OFFSET + sizeof(node_index_t)
#define COUNT_OFFSET LEVEL_OFFSET + sizeof(level_t)      * N_NODES
#define CHECK_OFFSET COUNT_OFFSET + sizeof(edge_index_t) * N_LEVELS

int main(void) {
    node_index_t * nodes 			= (node_index_t *)(BASE + NODES_OFFSET);
    edge_index_t * edges 			= (edge_index_t *)(BASE + EDGES_OFFSET);
    node_index_t * starting_node 	= (node_index_t *)(BASE + START_OFFSET);
    level_t      * level 			= (level_t      *)(BASE + LEVEL_OFFSET);
    edge_index_t * level_counts 	= (edge_index_t *)(BASE + COUNT_OFFSET);
    edge_index_t * check 			= (edge_index_t *)(BASE + CHECK_OFFSET);
#ifdef SPM
    node_index_t * spmn 			= (node_index_t *)(SPM_BASE + NODES_OFFSET);
    edge_index_t * spme 			= (edge_index_t *)(SPM_BASE + EDGES_OFFSET);
    level_t      * spml 			= (level_t      *)(SPM_BASE + LEVEL_OFFSET);
    edge_index_t * spmlc		 	= (edge_index_t *)(SPM_BASE + COUNT_OFFSET);
#endif

	common_val = 0;
    bfs.nodes = nodes;
    bfs.edges = edges;
    bfs.starting_node = starting_node;
    bfs.level = level;
    bfs.level_counts = level_counts;
    bfs.check = check;

    printf("Generating data\n");
    genData(&bfs);
    printf("Data generated\n");
#ifndef SPM
    NODES_ADDR = (uint64_t)(BASE + NODES_OFFSET);
    EDGES_ADDR = (uint64_t)(BASE + EDGES_OFFSET);
    START_ADDR = *(uint64_t *)(starting_node);
    LEVEL_ADDR = (uint64_t)(BASE + LEVEL_OFFSET);
    COUNT_ADDR = (uint64_t)(BASE + COUNT_OFFSET);
#else
    NODES_ADDR = (uint64_t)(SPM_BASE + NODES_OFFSET);
    EDGES_ADDR = (uint64_t)(SPM_BASE + EDGES_OFFSET);
    START_ADDR = *(uint64_t *)(starting_node);
    LEVEL_ADDR = (uint64_t)(SPM_BASE + LEVEL_OFFSET);
    COUNT_ADDR = (uint64_t)(SPM_BASE + COUNT_OFFSET);

    dmacpy(spmn,		nodes,	sizeof(node_index_t) * N_NODES * 2);
    while(!pollDma());
    resetDma();
    dmacpy(spme, 		edges, 	sizeof(edge_index_t) * N_EDGES);
    while(!pollDma());
    resetDma();
    dmacpy(spml,		level,	sizeof(level_t) * N_NODES);
    while(!pollDma());
    resetDma();
#endif
    int i;
    printf("%d\n", ACC);
    ACC = 0x01;
    printf("%d\n", ACC);
	while(ACC != 0x4) {
        printf("%d\n", ACC);
	}
#ifdef SPM
    dmacpy(level_counts,	spmlc,	sizeof(edge_index_t) * N_LEVELS);
    while(!pollDma());
#endif
	ACC = 0x00;
#ifdef CHECK
	if(!checkData(&bfs)) {
	    for (i = 0; i < N_LEVELS; i++) {
	        if (check[i] != level_counts[i]) {
	            printf("idx:%d exp:%d act:%d\n", i, check[i], level_counts[i]);
	        }
	    }
	}
#endif
	*(char *)(0x7fffffff) = 0;
  	//m5_exit();
}
