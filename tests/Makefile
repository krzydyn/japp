SOURCE_DIR?=.
LIB_DIR?=.
BUILD_DIR?=./build

all:build

include ../common.mk

#TARGETS+=$(BUILD_DIR)/uxtcp

build: $(BUILD_DIR)/uxtcp

$(BUILD_DIR)/uxtcp: $(BUILD_DIR)/unixtcp.o
	$(CC) -o $@ $<
