KERN=md_knn.c top.c
build: $(FILES)
	clang -O1 -S -target armv7-pc-none-eabi -fno-slp-vectorize -fno-vectorize -fno-unroll-loops -fno-inline -fno-builtin -emit-llvm $(KERN)

clean:
	rm -f *.ll
