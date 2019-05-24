ifeq ($(KERNELRELEASE),)  

KERNELDIR ?= /lib/modules/$(shell uname -r)/build 
PWD := $(shell pwd)  

.PHONY: build clean  

build: sneaky_process 
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules  

sneaky_process: 
	gcc -Werror -Wall -pedantic -o sneaky_process sneaky_process.c

clean: 
	rm -rf *.o *~ core .depend .*.cmd *.order *.symvers *.ko *.mod.c sneaky_process
else  

$(info Building with KERNELRELEASE = ${KERNELRELEASE}) 
obj-m :=    sneaky_mod.o  

endif
