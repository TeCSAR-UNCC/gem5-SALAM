#!/bin/bash
FLAGS=""
BENCH=""
UNROLL_TYPE=""
DEBUG="false"
PRINT_TO_FILE="false"
VALGRIND="false"

while getopts ":u:b:f:vdp" opt; do
	case $opt in
		b )
			BENCH=${OPTARG}
			;;
		u )
			UNROLL_TYPE=${OPTARG}
			;;
		d )
			DEBUG="true"
			;;
		p )
			PRINT_TO_FILE="true"
			;;
		f )
			if [ -z "${FLAGS}" ]
			then
				FLAGS+="${OPTARG}"
			else
				FLAGS+=",${OPTARG}"
			fi
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
	echo "Usage: $0 -u Unroll Type -b BENCHMARK (-f DEBUGFLAG) (-p) (-d)"
	exit 2
fi

if [ "${UNROLL_TYPE}" == "" ]; then
	echo "No benchmark specified."
	echo "Usage: $0 -u Unroll Type -b BENCHMARK (-f DEBUGFLAG) (-p) (-d)"
	exit 2
fi

if [ "${DEBUG}" == "true" ]; then
	BINARY="gdb --args ${M5_PATH}/build/ARM/gem5.debug"
elif [ "${VALGRIND}" == "true" ]; then
	BINARY="valgrind --leak-check=yes --suppressions=util/valgrind-suppressions --track-origins=yes --error-limit=no --leak-check=full --show-leak-kinds=all --show-reachable=no ${M5_PATH}/build/ARM/gem5.debug"
else
	BINARY="${M5_PATH}/build/ARM/gem5.opt"
fi

BENCH_DIR="/benchmarks/lenet/${UNROLL_TYPE}/${BENCH}/"

KERNEL=$M5_PATH/$BENCH_DIR/sw/main.elf

SYS_OPTS="--mem-size=4GB \
		  --mem-type=DDR4_2400_8x8 \
          --kernel=$KERNEL \
          --disk-image=$M5_PATH/baremetal/common/fake.iso \
          --machine-type=VExpress_GEM5_V1 \
          --dtb-file=none --bare-metal \
          --cpu-type=DerivO3CPU"
CACHE_OPTS="--caches --l2cache"

OUTDIR=BM_ARM_OUT/lenet/$UNROLL_TYPE/$BENCH

DEBUG_FLAGS=""

if [ ${FLAGS}  != "" ]; then
	DEBUG_FLAGS+="--debug-flags="
	DEBUG_FLAGS+=$FLAGS
fi

SYS_NAME="lenet_${UNROLL_TYPE}_${BENCH}"

RUN_SCRIPT="$BINARY $DEBUG_FLAGS --outdir=$OUTDIR \
			configs/SALAM/generated/fs_$SYS_NAME.py $SYS_OPTS \
			--accpath=$M5_PATH/$BENCH_DIR \
			--accbench=$BENCH $CACHE_OPTS"

${M5_PATH}/SALAM-Configurator/systembuilder.py --sysName "$SYS_NAME" --benchDir "$BENCH_DIR"

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


