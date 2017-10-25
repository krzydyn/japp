SOURCE_DIR?=.
LIB_DIR?=.
BUILD_DIR?=./build

all:build

include ../common.mk

TEST_TARGETS:=$(BUILD_DIR)/lists $(BUILD_DIR)/threads $(BUILD_DIR)/strings 

LIB_OBJS:=$(LIB_DIR)/src/*.o

#LDFLAGS+=$(LIB_OBJS)

#TARGETS+=$(TEST_TARGETS)

$(BUILD_DIR)/lists $(BUILD_DIR)/threads $(BUILD_DIR)/strings: $(LIB_OBJS)

build: $(TEST_TARGETS)

RUN_TESTS:=$(TEST_TARGETS:$(BUILD_DIR)/%=run-%)

$(RUN_TESTS):
	@printf "$(TRM_WHI)Runnig $(@:run-%=%)...$(TRM_END)\n"
	@$(BUILD_DIR)/$(@:run-%=%)
	@printf "\r$(TRM_WHI)Test $(@:run-%=%) $(TRM_GRE)[OK]$(TRM_END)\n"

run: build $(RUN_TESTS)
