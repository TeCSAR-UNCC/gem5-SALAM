#!/bin/bash
FLAGS=IOAcc,LLVMInterface,CommInterface,ComputeUnit

BENCH=""
DEBUG="false"

while getopts ":b:d" opt
	do
		case $opt in
			b )
				BENCH=${OPTARG}
				;;
			d )
				DEBUG="true"
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

KERNEL=$M5_PATH/benchmarks/sys_validation/$BENCH/sw/main.elf
SYS_OPTS="--mem-size=4GB \
          --kernel=$KERNEL \
          --disk-image=$M5_PATH/baremetal/common/fake.iso \
          --machine-type=VExpress_GEM5_V1 \
          --dtb-file=none --bare-metal \
          --cpu-type=DerivO3CPU"
CACHE_OPTS="--caches"
# Script to start up full system simulation
$BINARY --debug-flags=$FLAGS --outdir=BM_ARM_OUT/sys_validation/$BENCH \
configs/SALAM/sys_validation.py $SYS_OPTS \
--accpath=$M5_PATH/benchmarks/sys_validation \
--accbench=$BENCH $CACHE_OPTS

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


