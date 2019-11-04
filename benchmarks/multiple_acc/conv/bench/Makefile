.PHONY: build conv clean

build: conv

conv: $(FILES)
	clang -emit-llvm -c conv.c -o conv.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll conv.o -o conv.bc
	llvm-dis conv.bc -o conv.ll

clean:
	rm -f *.ll
	rm -f *.o
	rm -f *.bc
