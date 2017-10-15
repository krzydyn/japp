BUILD_DIR?=./build

include ../common.mk

OBJS:=$(BUILD_DIR)/System.o $(BUILD_DIR)/Object.o $(BUILD_DIR)/String.o

build: $(BUILD_DIR) $(OBJS)

clean-src:
	rm -f $(OBJS)
