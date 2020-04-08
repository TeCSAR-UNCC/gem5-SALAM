#!/bin/bash
FLAGS=IOAcc,LLVMInterface,CommInterface,ComputeUnit

BENCH=""
DEBUG="false"
PRINT_TO_FILE="false"

while getopts ":b:dp" opt
	do
		case $opt in
			b )
				BENCH=${OPTARG}
				;;
			d )
				DEBUG="true"
				;;
			p )
				PRINT_TO_FILE="true"
				;;
			* )
				echo "Invalid argument: ${OPTARG}"
				exit 1
				;;
		esac
done

if [ "${BENCH}" == "" ]; then
	echo "No benchmark specified."
	echo "Usage: $0 -b BENCHMARK (-d)"
	exit 2
fi

if [ "${DEBUG}" == "true" ]; then
	BINARY="ddd --gdb --args ${M5_PATH}/build/ARM/gem5.debug"
else
	BINARY="${M5_PATH}/build/ARM/gem5.opt"
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

OUTDIR=BM_ARM_OUT/$BENCH

RUN_SCRIPT="$BINARY --debug-flags=$FLAGS --outdir=$OUTDIR \
			configs/SALAM/fs_hwacc.py $SYS_OPTS \
			--accpath=$M5_PATH/benchmarks \
			--accbench=$BENCH $CACHE_OPTS"

if [ "${PRINT_TO_FILE}" == "true" ]; then
	mkdir -p $OUTDIR
	$RUN_SCRIPT > ${OUTDIR}/debug-trace.txt
else
	$RUN_SCRIPT
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


