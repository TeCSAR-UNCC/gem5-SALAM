LLVM_PATH = $(WORKING_DIR)/llvm-project/install/bin
CLUSTER = classifier
LL = top.ll Linear.ll

# .PHONY clean build

%.ll : %.c
	clang -O1 -S -target armv7-pc-none-eabi -emit-llvm -o $@ $<
build: $(LL)
	mkdir -p ../../ir/$(CLUSTER)
	mv $^ ../../ir/$(CLUSTER)/
clean:
	rm -f ../../ir/$(CLUSTER)/*.ll