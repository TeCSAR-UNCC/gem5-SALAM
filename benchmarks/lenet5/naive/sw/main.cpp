#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "host.h"
#include "../../../common/m5ops.h"

int main(void) {
	m5_reset_stats();
        uint8_t  * MMR  = (uint8_t  *)(TOP + 0x00);
        uint64_t * ARGS = (uint64_t *)(TOP + 0x01);
        // Run Head
        printf("Setting args\n");
        ARGS[0] = feats;
        ARGS[1] = weights;
        printf("Running acc\n");
        MMR[0]  = 0x01;
        while(stage == 0);

        m5_dump_stats();
	m5_exit();
        return 0;
}