KERNELDIR = /home/odroid/kernel_cycle_timer

obj-m = cpucycle.o getpid.o monitor.o

KDIR := /home/odroid/kernel_cycle_timer
PWD := $(shell pwd)

default:
	$(MAKE) -C $(KDIR) SUBDIRS=$(PWD) modules

clean:
	rm -rf *.ko
	rm -rf *.mod.*
	rm -rf .*.cmd
	rm -rf *.o
