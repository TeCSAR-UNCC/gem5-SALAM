TARGET=top.ll search.ll vector1.ll vector2.ll

%.ll : %.c
	clang -O1 -loop-vectorize -S -target armv7-pc-none-eabi -emit-llvm -o $@ $<
build : $(TARGET)

clean:
	rm -f *.ll
