ifneq ($(KERNELRELEASE),)

obj-m := memdev.o

else
	
KDIR := /home/craigyang/tiny6410/KernelCompile/linux-2.6.38/
all:
	make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux-
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers  modul*

endif
