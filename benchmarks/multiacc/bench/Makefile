build: $(FILES)
	clang -emit-llvm -c gemm.c -o gemm.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll gemm.o -o gemm.bc
	llvm-dis gemm.bc -o gemm.ll
	clang -emit-llvm -c fir.c -o fir.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll fir.o -o fir.bc
	llvm-dis fir.bc -o fir.ll

clean:
	rm -f *.ll
	rm -f *.o
	rm -f *.bc
