ARCH=arm64
COMPILER=aarch64-linux-gnu-
obj-m:= sam_dev.o
KERNELDIR := /home/samerogers/gem5/linux-arm-gem5
PWD:= $(shell pwd)
default:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(COMPILER) modules

clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) clean
