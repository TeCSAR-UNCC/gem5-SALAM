#!/bin/bash
FLAGS=IOAcc,LLVMInterface,CommInterface,NoncoherentDma,ComputeUnit
if [ "$1" != "" ]; then
    BENCH=$1
    MULTI="$1"
else
    echo "No benchmark specified. Using vadd."
    BENCH="vadd"
fi
KERNEL=$M5_PATH/benchmarks/$BENCH/host/main.elf
SYS_OPTS="--mem-size=4GB \
          --kernel=$KERNEL \
          --disk-image=$M5_PATH/baremetal/common/fake.iso \
          --machine-type=VExpress_GEM5_V1 \
          --dtb-file=none --bare-metal \
          --cpu-type=DerivO3CPU"
CACHE_OPTS="--caches --l2cache --acc_cache"
# Script to start up full system simulation
# --debug-flags=$FLAGS
if [ $BENCH == "multiple_acc" ]; then
	echo "Running Mutiple Accelerator Configuration"
	build/ARM/gem5.opt --outdir=BM_ARM_OUT/$BENCH \
	configs/example/fs_multi.py \
	$SYS_OPTS --accpath=$M5_PATH/benchmarks --accbench=$BENCH \
	$CACHE_OPTS #> BM_ARM_OUT/$BENCH/debug_trace.txt
	# --debug-flags=$FLAGS
else
	echo "Running Single Accelerator Configuration"
	build/ARM/gem5.opt --outdir=BM_ARM_OUT/$BENCH configs/SALAM/fs_hwacc.py \
	$SYS_OPTS --accpath=$M5_PATH/benchmarks --accbench=$BENCH \
	$CACHE_OPTS #> BM_ARM_OUT/$BENCH/debug_trace.txt
fi

# Debug Flags List
#
# IOAcc
# ClassDetail
# CommInterface
# ComputeUnit
# LLVMInterface
# ComputeNode
# LLVMRegister
# LLVMOp
# LLVMParse
# LLVMGEP
# LLVMRuntime == ComputeNode + LLVMRegister + LLVMOp + LLVMParse
# NoncoherentDma - bfs, fft, gemm, md-knn, nw, spmv


