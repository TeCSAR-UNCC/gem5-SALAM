.PHONY: build max_pool clean

build: max_pool

max_pool: $(FILES) 
	clang -emit-llvm -c max_pool.c -o max_pool.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll max_pool.o -o max_pool.bc
	llvm-dis max_pool.bc -o max_pool.ll

clean:
	rm -f *.ll
	rm -f *.o
	rm -f *.bc
