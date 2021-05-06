/*
 * Copyright (c) 2015, University of Kaiserslautern
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived from
 *    this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
 * TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER
 * OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * Authors: Matthias Jung
            Frederik Lauer
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "host.h"
#include "../../common/m5ops.h"

int main(void) {
	m5_reset_stats();
    int exitCond = 0;
    unsigned phase, rs_offset, rd_offset, wr_offset;
    // Start Head
    // runHead(feats+0x00000000,feats+0x00100000,
    //         weights, qparams,
    //         weights, qparams,
    //         weights, qparams);
    // Start Body_0
    phase = 0;
    rs_offset = (phase + 0) * 0x00100000;
    rd_offset = (phase + 1) * 0x00100000;
    wr_offset = (phase + 2) * 0x00100000;
    runBody(phase,
            feats+rd_offset,
            feats+rs_offset,
            feats+wr_offset,
            weights, qparams,
            weights, qparams,
            weights, qparams);
    phase = 0;
    runBody(phase,
            feats+rd_offset,
            feats+rs_offset,
            feats+wr_offset,
            weights, qparams,
            weights, qparams,
            weights, qparams);
    m5_dump_stats();
	m5_exit();
    return 0;
}

void runBody(uint8_t phase, uint64_t feat_rd_addr,
             uint64_t res_rd_addr, uint64_t feat_wr_addr,
             uint64_t pw0_weights, uint64_t pw0_quant,
             uint64_t dw0_weights, uint64_t dw0_quant,
             uint64_t pw1_weights, uint64_t pw1_quant) {
    
    volatile uint8_t  * MMR  = (uint8_t  *)(body_top);
    volatile uint64_t * ARGS = (uint64_t *)(body_top+1);
    volatile uint64_t count;

    ARGS[0] = feat_rd_addr;
    ARGS[1] = res_rd_addr;
    ARGS[2] = feat_wr_addr;
    ARGS[3] = pw0_weights;
    ARGS[4] = pw0_quant;
    ARGS[5] = dw0_weights;
    ARGS[6] = dw0_quant;
    ARGS[7] = pw1_weights;
    ARGS[8] = pw1_quant;
    ARGS[9]  = phase;

    MMR[0]  = 0x01;
    while(MMR[0] & 0x01) count++;
    while(MMR[0] & 0x02) count++;
    count = 0;
}