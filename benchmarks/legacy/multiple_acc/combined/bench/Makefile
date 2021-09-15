.PHONY: build combined clean

build: combined

combined: $(FILES) 
	clang -emit-llvm -c combined.c -o combined.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll combined.o -o combined.bc
	llvm-dis combined.bc -o combined.ll

clean:
	rm -f *.ll
	rm -f *.o
	rm -f *.bc
