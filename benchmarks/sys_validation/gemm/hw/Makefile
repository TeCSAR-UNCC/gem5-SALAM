KERN=gemm.c top.c
build: $(FILES)
	clang -O1 -S -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -fno-builtin -emit-llvm $(KERN) -target armv7-pc-none-eabi

clean:
	rm -f *.ll
