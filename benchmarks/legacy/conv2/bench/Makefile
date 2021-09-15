.PHONY: build conv relu max_pool combined clean

build: conv relu max_pool combined

conv: $(FILES)
	clang -emit-llvm -c conv.c -o conv.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll conv.o -o conv.bc
	llvm-dis conv.bc -o conv.ll

relu: $(FILES) 
	clang -emit-llvm -c relu.c -o relu.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll relu.o -o relu.bc
	llvm-dis relu.bc -o relu.ll

max_pool: $(FILES) 
	clang -emit-llvm -c max_pool.c -o max_pool.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll max_pool.o -o max_pool.bc
	llvm-dis max_pool.bc -o max_pool.ll

combined: $(FILES) 
	clang -emit-llvm -c combined.c -o combined.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll combined.o -o combined.bc
	llvm-dis combined.bc -o combined.ll

clean:
	rm -f *.ll
	rm -f *.o
	rm -f *.bc
