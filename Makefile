#TARGET   := mallocator_module
#WARN     := -W -Wall -Wstrict-prototypes -Wmissing-prototypes
#INCLUDE  := -isystem /lib/modules/`uname -r`/build/include
#CFLAGS   := -O2 -DMODULE -D__KERNEL__ ${WARN} ${INCLUDE}

KERNEL_MODULES_BUILD_DIR ?= /lib/modules/$(shell uname -r)/build

#${TARGET}.o: ${TARGET}.c


#.PHONY: clean

#obj-m += mallocator_module.o 
#mallocator_module-objs := mallocator_utils.o

obj-m += kmallocator.o
kmallocator-objs := mallocator_module.o mallocator_utils.o

#mallocator_module-objs := mallocator_module.o mallocator_utils.o
#KSRCS = mallocator_module.c mallocator_utils.c
#KOBJS = $(KSRCS:.c=.o)
#obj-m += $(KOBJS)

all: kernel_module user_app

user_app:
	$(CC) -o mallocator mallocator.c mallocator_utils.c -D USER_LAND

kernel_module:
	make -C ${KERNEL_MODULES_BUILD_DIR} M=$(PWD) modules


clean:
	#rm -fr ${TARGET}.o
	make -C ${KERNEL_MODULES_BUILD_DIR} M=$(PWD) clean
	rm -fr *.marker modules.order
	rm -fr mallocator
