.PHONY: build relu clean

build: relu

relu: $(FILES) 
	clang -emit-llvm -c relu.c -o relu.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll relu.o -o relu.bc
	llvm-dis relu.bc -o relu.ll

clean:
	rm -f *.ll
	rm -f *.o
	rm -f *.bc
