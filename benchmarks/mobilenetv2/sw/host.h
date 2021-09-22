#include "../hw/source/common/support.h"

#define feats       0x8f000000
#define weights     0x91000000
#define qparams     0x93000000
#define head_top    0x10020080
#define body_top    0x10021681
#define tail_top    0x100780c2
#define class_top   0x100ccf83

volatile uint8_t stage;

void runHead(uint64_t img_rd_addr, uint64_t feat_wr_addr,
             uint64_t conv_weights, uint64_t conv_quant,
             uint64_t dw_weights, uint64_t dw_quant,
             uint64_t pw_weights, uint64_t pw_quant);

void runBody(uint8_t phase, uint64_t feat_rd_addr,
             uint64_t res_rd_addr, uint64_t feat_wr_addr,
             uint64_t pw0_weights, uint64_t pw0_quant,
             uint64_t dw0_weights, uint64_t dw0_quant,
             uint64_t pw1_weights, uint64_t pw1_quant);

void runTail(uint64_t feat_rd_addr, uint64_t feat_wr_addr,
             uint64_t pw_weights, uint64_t pw_quant);

void runClassifier(uint64_t feat_rd_addr, uint64_t feat_wr_addr,
                   uint64_t weight, uint64_t quant);