PWD := $(shell pwd)
KERNEL_DIR ?= /lib/modules/$(shell uname -r)/build

DRV_NAME :=my_module_param
obj-m :=$(DRV_NAME).o

SRC_DIR := $(PWD)/src
BUILD_DIR := $(PWD)/build
BUILD_FILES += $(SRC_DIR)/*.ko

CLANG_FORMAT_VERS ?= 19
CLANG_FORMAT := clang-format-$(CLANG_FORMAT_VERS)
CLANG_FORMAT_FLAGS += -i
FORMAT_FILES := $(SRC_DIR)/*.c

.PHONY: build run remove install uninstall clean format check

$(shell mkdir -p $(BUILD_DIR))

build:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) modules
	mv $(BUILD_FILES) $(BUILD_DIR)/.

run:
	insmod $(BUILD_DIR)/$(DRV_NAME).ko

remove:
	rmmod $(BUILD_DIR)/$(DRV_NAME).ko

install:
	cp $(BUILD_DIR)/$(DRV_NAME).ko /lib/modules/$(shell uname -r)
	/sbin/depmod -a
	/sbin/modprobe $(DRV_NAME)

uninstall:
	/sbin/modprobe -r $(DRV_NAME)
	rm -f /lib/modules/$(shell uname -r)/$(DRV_NAME).ko
	/sbin/depmod -a
format:
	$(CLANG_FORMAT) $(CLANG_FORMAT_FLAGS) $(FORMAT_FILES)

clean:
	$(MAKE) -C $(KERNEL_DIR) M=$(PWD) clean

check:
	sudo python3 checker/main.py $(DRV_NAME)


