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
        volatile uint8_t  * head_MMR  = (uint8_t  *)(head_top);
        volatile uint8_t  * body_MMR  = (uint8_t  *)(body_top);
        volatile uint8_t  * tail_MMR  = (uint8_t  *)(tail_top);
        volatile uint8_t  * class_MMR  = (uint8_t  *)(class_top);

	m5_reset_stats();
        unsigned phase, rs_offset, rd_offset, wr_offset;
        
        phase = 0;
        printf("Current Stage: %d\n", phase);
        rs_offset = (phase + 0) * 0x00100000;
        rd_offset = (phase + 1) * 0x00100000;
        wr_offset = (phase + 2) * 0x00100000;
        runHead(phase,
                feats+rd_offset,
                feats+rs_offset,
                feats+wr_offset,
                weights, qparams,
                weights, qparams);

        while(*head_MMR != 0x04);
        
        phase = 1;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);

        phase = 2;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);

        phase = 3;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);

        phase = 4;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);

        phase = 5;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);

        phase = 6;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);

        phase = 7;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);

        phase = 8;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);

        phase = 9;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);

        phase = 10;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);

        phase = 11;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);
        
        phase = 12;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);
        
        phase = 13;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);
        
        phase = 14;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);
        
        phase = 15;
        printf("Current Stage: %d\n", phase);
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
        while(*body_MMR != 0x04);

        phase = 16;
        printf("Current Stage: %d\n", phase);
        runTail(feats+0x01100000,
                feats+0x01200000,
                weights, qparams);
        while(*tail_MMR != 0x04);

        phase = 17;
        printf("Current Stage: %d\n", phase);
        runClassifier(feats+0x01100000,
                feats+0x01200000,
                weights, qparams);
        while(*class_MMR != 0x04);


        m5_dump_stats();
	m5_exit();
        return 0;
}

void runHead(uint64_t img_rd_addr, uint64_t feat_wr_addr,
             uint64_t conv_weights, uint64_t conv_quant,
             uint64_t dw_weights, uint64_t dw_quant,
             uint64_t pw_weights, uint64_t pw_quant) {
    uint8_t  * MMR  = (uint8_t  *)(head_top);
    uint64_t * ARGS = (uint64_t *)(head_top+1);
    printf("Setting args for HEAD\n");
    ARGS[0] = img_rd_addr;
    ARGS[1] = feat_wr_addr;
    ARGS[2] = conv_weights;
    ARGS[3] = conv_quant;
    ARGS[4] = dw_weights;
    ARGS[5] = dw_quant;
    ARGS[6] = pw_weights;
    ARGS[7] = pw_quant;
    printf("Running HEAD\n");                                                                                                                 
    MMR[0]  = 0x01;
}

void runBody(uint8_t phase, uint64_t feat_rd_addr,
             uint64_t res_rd_addr, uint64_t feat_wr_addr,
             uint64_t pw0_weights, uint64_t pw0_quant,
             uint64_t dw0_weights, uint64_t dw0_quant,
             uint64_t pw1_weights, uint64_t pw1_quant) {
    
    volatile uint8_t  * MMR  = (uint8_t  *)(body_top);
    volatile uint64_t * ARGS = (uint64_t *)(body_top+1);
    volatile uint64_t count;

    printf("Setting args for BODY\n");
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

    printf("Running BODY\n");
    MMR[0]  = 0x01;
}

void runTail(uint64_t feat_rd_addr, uint64_t feat_wr_addr,
             uint64_t pw_weights, uint64_t pw_quant) {
    uint8_t  * MMR  = (uint8_t  *)(tail_top);
    uint64_t * ARGS = (uint64_t *)(tail_top+1);
    
    printf("Setting args for TAIL\n");
    ARGS[0] = feat_rd_addr;
    ARGS[1] = feat_wr_addr;
    ARGS[2] = pw_weights;
    ARGS[3] = pw_quant;

    printf("Running TAIL\n");
    MMR[0]  = 0x01;
}

void runClassifier(uint64_t feat_rd_addr, uint64_t feat_wr_addr,
                   uint64_t weight, uint64_t quant) {
    uint8_t  * MMR  = (uint8_t  *)(class_top);
    uint64_t * ARGS = (uint64_t *)(class_top+1);

    printf("Setting args for CLASSIFIER\n");
    ARGS[0] = feat_rd_addr;
    ARGS[1] = feat_wr_addr;
    ARGS[2] = weight;
    ARGS[3] = quant;
    
    printf("Running CLASSIFIER\n");
    MMR[0]  = 0x01;
}