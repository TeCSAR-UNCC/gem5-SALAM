KERN=spmv

build: $(FILES)
	clang -O1 -S -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -fno-builtin -emit-llvm $(KERN).c

clean:
	rm -f *.ll
