#!/bin/bash
FLAGS="HWACC"
BENCH=""
DEBUG="false"
PRINT_TO_FILE="false"
VALGRIND="false"

while getopts ":b:f:vdp" opt; do
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
		f )
			FLAGS+=",${OPTARG}"
			;;
		v ) 
			VALGRIND="true"
			;;
		* )
			echo "Invalid argument: ${OPTARG}"
			echo "Usage: $0 -b BENCHMARK (-f DEBUGFLAG) (-p) (-d)"
			exit 1
			;;
	esac
done

if [ "${BENCH}" == "" ]; then
	echo "No benchmark specified."
	echo "Usage: $0 -b BENCHMARK (-f DEBUGFLAG) (-p) (-d)"
	exit 2
fi

if [ "${DEBUG}" == "true" ]; then
	BINARY="ddd --gdb --args ${M5_PATH}/build/ARM/gem5.debug"
elif [ "${VALGRIND}" == "true" ]; then
	BINARY="valgrind --leak-check=yes --suppressions=util/valgrind-suppressions --track-origins=yes ${M5_PATH}/build/ARM/gem5.debug"
else
	BINARY="${M5_PATH}/build/ARM/gem5.opt"
fi

KERNEL=$M5_PATH/benchmarks/sys_validation/$BENCH/sw/main.elf

SYS_OPTS="--mem-size=4GB \
		  --mem-type=DDR4_2400_8x8 \
          --kernel=$KERNEL \
          --disk-image=$M5_PATH/baremetal/common/fake.iso \
          --machine-type=VExpress_GEM5_V1 \
          --dtb-file=none --bare-metal \
          --cpu-type=DerivO3CPU"
CACHE_OPTS="--caches --l2cache"

OUTDIR=BM_ARM_OUT/sys_validation/$BENCH

RUN_SCRIPT="$BINARY --debug-flags=$FLAGS --outdir=$OUTDIR \
			configs/SALAM/sys_validation.py $SYS_OPTS \
			--accpath=$M5_PATH/benchmarks/sys_validation \
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


