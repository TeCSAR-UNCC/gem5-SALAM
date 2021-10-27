LL = top.ll conv0.ll pool0.ll data_move_0.ll data_move_1.ll conv1.ll

%.ll : %.c
	clang -O1 -S -target armv7-pc-none-eabi -emit-llvm -o $@ $<
build: $(LL)
	mkdir -p ../ir
	mv $^ ../ir/
clean:
	rm -f ../ir/*.ll