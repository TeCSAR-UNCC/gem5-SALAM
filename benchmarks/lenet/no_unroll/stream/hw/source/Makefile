LL = top.ll conv0.ll pool0.ll data_move_0.ll data_move_1.ll conv1.ll pool1.ll data_move_2.ll conv2.ll pool_move_0.ll pool_move_1.ll fc0.ll data_move_3.ll

%.ll : %.c
	clang -O1 -S -target armv7-pc-none-eabi -emit-llvm -o $@ $<
build: $(LL)
	mkdir -p ../ir
	mv $^ ../ir/
clean:
	rm -f ../ir/*.ll