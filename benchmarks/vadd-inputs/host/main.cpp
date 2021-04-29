#include <cstdio>
#include <cstdlib>
#include <cstring>
#include "../../common/dma.h"
#include "../../common/m5ops.h"

#include "gemm.h"

gemm_struct ges;

int main(void) {
  m5_reset_stats();
  uint64_t base = 0x80c00000;
  uint64_t spm_base = 0x2f100000;
  TYPE *m1 = (TYPE *)base;
  TYPE *m2 = (TYPE *)(base + sizeof(TYPE) * N);
  TYPE *m3 = (TYPE *)(base + 2 * sizeof(TYPE) * N);
  TYPE *check = (TYPE *)(base+3*sizeof(TYPE)*N);
   common_val = 0;
  // register int sp asm("sp");
  // uint64_t base_input = 0x90c00000;
  // uint32_t* input = (uint32_t*)0x90c00000;
  for (int i = 0; i < N; i++) {
            printf("%p %ld\n",m1+i,m1[i]);
  }

  for (int i = 0; i < N; i++) {
            printf("%p %ld\n",m2+i,m2[i]);
  }
#ifdef SPM
  TYPE *spm1 = (TYPE *)spm_base;
  TYPE *spm2 = (TYPE *)(spm_base + sizeof(TYPE) * N);
  TYPE *spm3 = (TYPE *)(spm_base + 2 * sizeof(TYPE) * N);
#endif

  val_a = (uint64_t)spm_base;
  val_b = (uint64_t)(spm_base + sizeof(TYPE) * N);
  val_c = (uint64_t)(spm_base + 2 * sizeof(TYPE) * N);

  printf("%d\n", acc);
  // acc = 0x01;

  dmacpy(spm1, m1, sizeof(TYPE) * N);
  while (!pollDma());
  resetDma();
  dmacpy(spm2, m2, sizeof(TYPE) * N);
  while (!pollDma());
  resetDma();

  acc = 0x01;
  printf("%d\n", acc);
  while (acc != 0x4) {
    printf("%d\n", acc);
  }

#ifdef SPM
  dmacpy(m3, spm3, sizeof(TYPE) * N);
  while (!pollDma());
  resetDma();
#endif
  acc = 0x00;
  for(int j = 0; j < 5; j++) {
  for (int i = 0; i < N; i++) {
   printf("M3 %p %ld\n",m3+i,m3[i]);
  }
  }

#ifdef CHECK
   bool fail = false;
   for (int i = 0; i < N; i++) {
     printf("M1: %p %ld\n",m1+i,m1[i]);
     printf("M2: %p %ld\n",m2+i,m2[i]);
     if ((m1[i] + m2[i]) != m3[i])
       {

 	printf("Expected : %ld  Actual : %ld\n",m1[i]+m2[i],m3[i]);
 	fail = true;
       }
     }

   if (fail)
     printf("Check Failed\n");
   else
    printf("Check Passed\n");
#endif
  //*(char *)0x2FFFFFFF = 1;
  m5_dump_stats();
  m5_exit();
}
