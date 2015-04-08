DEBUG = y
ifeq ($(DEBUG),y)
	DEBFLAGS = -O2 -g -DDEBUGLOG
else
	DEBFLAGS = -O2
endif

EXTRA_CFLAGS+=$(DEBFLAGS)


ifneq ($(KERNELRELEASE),)

obj-m := mymemdev.o

else
	KDIR := /home/craigyang/tiny6410/KernelCompile/linux-2.6.38/
all:
	make -C $(KDIR) M=$(PWD) modules ARCH=arm CROSS_COMPILE=arm-linux- 
clean:
	rm -f *.ko *.o *.mod.o *.mod.c *.symvers  modul*

endif
