KERN=spmv.c top.c

build: $(FILES)
	clang -O1 -S -m32 -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -fno-builtin -emit-llvm $(KERN)

clean:
	rm -f *.ll
