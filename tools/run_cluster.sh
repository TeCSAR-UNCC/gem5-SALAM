#!/bin/bash
BENCH=""
BENCH_PATH=""
FLAGS=""
# FLAGS="SALAM_Debug,CommInterface,NoncoherentDma,LLVMParse"
DEBUG=False
PRINT_TO_FILE=False
VALGRIND=False

while [[ $# -gt 0 ]]; do
  case $1 in
    -b|--bench)
      BENCH="$2"
      shift # past argument
      shift # past value
      ;;
    -bp|--bench-path)
      BENCH_PATH="$2"
      shift # past argument
      shift # past value
      ;;
    -f|--flags)
      FLAGS="$2"
      shift # past argument
      shift # past value
      ;;
    -d|--debug)
      DEBUG=True
      shift # past argument
      ;;
    -p|--print)
      PRINT_TO_FILE=True
      shift # past argument
      shift # past value
      ;;
    -v|--valgrind)
      VALGRIND=True
      shift # past argument
      shift # past value
      ;;
    -*)
      echo "Unknown option $1"
      exit 1
      ;;
    *)
      shift # past argument
      ;;
  esac
done

if [ "$BENCH" == "" ]; then
	echo "BENCH env var is not set, exiting"
	exit 1
fi

if [ "$M5_PATH" == "" ]; then
	echo "M5_PATH env var is not set, exiting"
	exit 1
fi

if [ "$BENCH_PATH" == "" ]; then
	BENCH_PATH=$BENCH
fi

if [ ${DEBUG} == True ]; then
	BINARY="gdb --args ${M5_PATH}/build/ARM/gem5.debug"
elif [ ${VALGRIND} == True ]; then
	BINARY="valgrind --leak-check=yes --suppressions=util/valgrind-suppressions --suppressions=util/salam.supp --track-origins=yes --error-limit=no --leak-check=full --show-leak-kinds=definite,possible --show-reachable=no --log-file=$BENCH.log  ${M5_PATH}/build/ARM/gem5.debug" #--gen-suppressions=all
else
	BINARY="${M5_PATH}/build/ARM/gem5.opt"
fi

KERNEL=$M5_PATH/benchmarks/"$BENCH_PATH"/sw/main.elf

SYS_OPTS="--mem-size=4GB \
		  --mem-type=DDR4_2400_8x8 \
          --kernel=$KERNEL \
          --disk-image=$M5_PATH/benchmarks/common/fake.iso \
          --machine-type=VExpress_GEM5_V1 \
          --dtb-file=none --bare-metal \
          --cpu-type=DerivO3CPU"

CACHE_OPTS="--caches --l2cache"

OUTDIR=BM_ARM_OUT/$BENCH_PATH/

DEBUG_FLAGS=""

if [ "${FLAGS}"  != "" ]; then
	DEBUG_FLAGS+="--debug-flags="
	DEBUG_FLAGS+=$FLAGS
fi

RUN_SCRIPT="$BINARY $DEBUG_FLAGS --outdir=$OUTDIR \
			$M5_PATH/configs/SALAM/fs_$BENCH.py $SYS_OPTS \
			--accpath=$M5_PATH/benchmarks/$BENCH_PATH \
			--accbench=$BENCH $CACHE_OPTS"

if (! "$M5_PATH"/tools/SALAM-Configurator/systembuilder.py --sys-name "$BENCH" --sys-path "/benchmarks/$BENCH_PATH") then
	echo "Configurator failed"
	exit 1
fi

if [ ${PRINT_TO_FILE} == True ]; then
	mkdir -p "$OUTDIR"
	$RUN_SCRIPT > "${OUTDIR}"/debug-trace.txt
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
