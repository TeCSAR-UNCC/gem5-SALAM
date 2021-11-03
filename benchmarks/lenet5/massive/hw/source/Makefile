LL = top.ll conv0.ll conv1.ll pool0.ll pool1.ll fc0.ll fc1.ll data_mover_0.ll data_mover_1.ll data_mover_2.ll data_mover_3.ll

%.ll : %.c
	rm -f ../ir/*.ll
	clang -O1 -S -target armv7-pc-none-eabi -emit-llvm -o $@ $<
build: $(LL)
	mkdir -p ../ir
	mv $^ ../ir/
clean:
	rm -f ../ir/*.ll