KERN=gemm

build: $(FILES)
	clang -emit-llvm -c $(KERN).c -o $(KERN).o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll $(KERN).o -o $(KERN).bc
	llvm-dis $(KERN).bc -o gemm-full-unroll.ll
	#clang -O1 -S -m32 -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -fno-builtin -emit-llvm $(KERN).c

clean:
	rm -f *.ll
