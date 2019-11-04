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
#include "gemm.h"
#include "../../common/dma.h"
#include "../../common/m5ops.h"

gemm_struct ges;

int main(void) {
	m5_reset_stats();
    uint64_t base = 0x80c00000;
    uint64_t spm_base = 0x2f100000;
	TYPE *m1 = (TYPE *)base;
	TYPE *m2 = (TYPE *)(base+8*ROW*COL);
	TYPE *m3 = (TYPE *)(base+16*ROW*COL);
	TYPE *check = (TYPE *)(base+24*ROW*COL);
	int row_size = ROW;
    int col_size = COL;
#ifdef SPM
    TYPE *spm1 = (TYPE *)spm_base;
    TYPE *spm2 = (TYPE *)(spm_base+8*ROW*COL);
    TYPE *spm3 = (TYPE *)(spm_base+16*ROW*COL);
#endif
	common_val = 0;

    ges.a = m1;
    ges.b = m2;
    ges.c = m3;
    ges.row_size = row_size;
    ges.col_size = col_size;

    printf("Generating data\n");
    genData(&ges);
    printf("Data generated\n");

    val_a = (uint64_t)base;
    val_b = (uint64_t)(base+8*ROW*COL);
    val_c = (uint64_t)(base+16*ROW*COL);

#ifndef SPM
    val_a = (uint64_t)base;
    val_b = (uint64_t)(base+8*ROW*COL);
    val_c = (uint64_t)(base+16*ROW*COL);

    printf("%d\n", acc);
#else
    val_a = (uint64_t)spm_base;
    val_b = (uint64_t)(spm_base+8*ROW*COL);
    val_c = (uint64_t)(spm_base+16*ROW*COL);

    printf("%d\n", acc);
    //acc = 0x01;

    dmacpy(spm1, m1, sizeof(TYPE)*ROW*COL);
    while(!pollDma());
    resetDma();
    dmacpy(spm2, m2, sizeof(TYPE)*ROW*COL);
    while(!pollDma());
    resetDma();
#endif

    acc = 0x01;
    printf("%d\n", acc);
	while(acc != 0x4) {
        printf("%d\n", acc);
	}

#ifdef SPM
    dmacpy(m3, spm3, sizeof(TYPE)*ROW*COL);
    while(!pollDma());
#endif
	acc = 0x00;
#ifdef CHECK
	bool fail = false;
	int i, j, k, k_col, i_col;
	TYPE sum = 0;
	TYPE mult = 0;

	for(i=0;i<ROW;i++) {
        for(j=0;j<COL;j++) {
            i_col = i * COL;
            sum = 0;
            for(k=0;k<ROW;k++) {
                k_col = k * COL;
                mult = m1[i_col + k] * m2[k_col + j];
                sum += mult;
            }
            check[i_col + j] = sum;
        }
    }

    for(i=0; i<ROW*COL; i++) {
        printf("Expected:%f Actual:%f\n", check[i], m3[i]);
        if(m3[i] != check[i])
            fail = true;
    }

    if(fail)
        printf("Check Failed\n");
    else
        printf("Check Passed\n");
#endif
    //*(char *)0x2FFFFFFF = 1;
	m5_dump_stats();	
	m5_exit();
}
