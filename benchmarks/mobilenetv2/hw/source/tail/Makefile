LLVM_PATH = $(WORKING_DIR)/llvm-project/install/bin
CLUSTER = tail
LL = top.ll PWConv.ll Reshape.ll AvgPool.ll

# .PHONY clean build

%.o : %.c
	clang -emit-llvm -c -o $@ $<
%.bc : %.o
	opt -O1 -inline -mem2reg -simplifycfg -loops -loop-unroll -o $@ $<
%.ll : %.bc
	llvm-dis -o $@ $<
build: $(LL)
	mv $^ ../../ir/$(CLUSTER)/
clean:
	rm -f ../../ir/$(CLUSTER)/*.ll