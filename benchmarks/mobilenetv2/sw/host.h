#include "../hw/source/common/support.h"
#include "../head_hw_defines.h"
#include "../body_hw_defines.h"
#include "../tail_hw_defines.h"
#include "../classifier_hw_defines.h"

#define feats       0x8f000000
#define weights     0x91000000
#define qparams     0x93000000
#define head_top    HEAD_TOP
#define body_top    BODY_TOP
#define tail_top    TAIL_TOP
#define class_top   CLASSIFIER_TOP

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