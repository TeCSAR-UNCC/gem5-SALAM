TARGET=top.ll conv0.ll conv1.ll conv2.ll pool0.ll pool1.ll
%.o : %.c
	clang -emit-llvm -c -o $@ $<
%.bc : %.o
	opt -O1 -mem2reg -simplifycfg -loops -loop-unroll -o $@ $<
%.ll : %.bc
	llvm-dis -o $@ $<
build : $(TARGET)

clean:
	rm -f *.ll
