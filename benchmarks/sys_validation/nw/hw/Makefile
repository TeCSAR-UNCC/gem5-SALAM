KERN=nw.c top.c

build : $(FILES)
	clang -O1 -S -target armv7-pc-none-eabi -emit-llvm $(KERN)
clean:
	rm -f *.ll
