#!/bin/bash
FLAGS=""
DEBUG="false"
PRINT_TO_FILE="false"
VALGRIND="false"

while getopts ":b:f:vdp" opt; do
	case $opt in
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

if [ "${DEBUG}" == "true" ]; then
	BINARY="gdb --args ${M5_PATH}/build/ARM/gem5.debug"
elif [ "${VALGRIND}" == "true" ]; then
	BINARY="valgrind --leak-check=yes --suppressions=util/valgrind-suppressions --suppressions=util/salam.supp --track-origins=yes --error-limit=no --leak-check=full --show-leak-kinds=definite,possible --show-reachable=no --log-file=mobilenetv2.log  ${M5_PATH}/build/ARM/gem5.debug" #--gen-suppressions=all
else
	BINARY="${M5_PATH}/build/ARM/gem5.opt"
fi

KERNEL=$M5_PATH/benchmarks/mobilenetv2/sw/main.elf

SYS_OPTS="--mem-size=4GB \
		  --mem-type=DDR4_2400_8x8 \
          --kernel=$KERNEL \
          --disk-image=$M5_PATH/baremetal/common/fake.iso \
          --machine-type=VExpress_GEM5_V1 \
          --dtb-file=none --bare-metal \
          --cpu-type=DerivO3CPU"
CACHE_OPTS="--caches --l2cache"

OUTDIR=BM_ARM_OUT/mobilenetv2/

DEBUG_FLAGS=""

if [ ${FLAGS}  != "" ]; then
	DEBUG_FLAGS+="--debug-flags="
	DEBUG_FLAGS+=$FLAGS
fi

RUN_SCRIPT="$BINARY $DEBUG_FLAGS --outdir=$OUTDIR \
			configs/SALAM/generated/fs_mobilenetv2.py $SYS_OPTS \
			--accpath=$M5_PATH/benchmarks/mobilenetv2 \
			--accbench=mobilenetv2 $CACHE_OPTS"

${M5_PATH}/SALAM-Configurator/systembuilder.py --sysName mobilenetv2 --benchDir "benchmarks/mobilenetv2/${BENCH}"

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
