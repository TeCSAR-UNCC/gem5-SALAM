TARGET=bfs.ll top.ll

%.ll : %.c
	clang -O1 -S -fno-slp-vectorize -fno-vectorize \
	-fno-unroll-loops -fno-inline -fno-builtin -emit-llvm -o $@ $< -target armv7-pc-none-eabi
build : $(TARGET)

clean:
	rm -f *.ll
