.PHONY: all build clean install
all:build

PREFIX?=.
TOP_DIR?=.

BUILD_DIR?=$(TOP_DIR)/build
SOURCE_DIR?=$(PREFIX)
INC_DIR?=../include
JRELIB?=../src/build/jre.a
DEBUG?=-g

HOST_OS:=$(shell uname -s)
GCC_VERSION:=$(shell gcc --version | awk '/^gcc/ {print $$4}')

CC:=g++
CPPFLAGS:=$(DEBUG) -I$(INC_DIR) -fPIC -std=c++11
CPPFLAGS+=-Wall -Werror
ifeq ($(HOST_OS),Linux)
LDFLAGS:=-rdynamic
endif
LDFLAGS+=-lpthread

SRCS:=$(wildcard $(SOURCE_DIR)/*.cpp)
OBJS:=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TARGETS:=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%,$(SRCS))

TARGETS: $(JRELIB)

$(BUILD_DIR)/%: $(SOURCE_DIR)/%.cpp $(JRELIB)
	@#printf "$(CC) $(CPPFLAGS) $^ -o $@\n"
	$(CC) $(CPPFLAGS) $^ $(LDFLAGS) -o $@

build: create-$(BUILD_DIR) $(TARGETS)

clean:
	rm -rf $(BUILD_DIR)

RUN_TESTS:=$(TARGETS:$(BUILD_DIR)/%=run-%)

.PHONY: create-$(BUILD_DIR)
create-$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

.PHONY: $(RUN_TESTS)
$(RUN_TESTS):
	@printf "$(TRM_WHI)Runnig $(@:run-%=%)...$(TRM_END)\n"
	@$(BUILD_DIR)/$(@:run-%=%)
	@printf "\r$(TRM_WHI)Test $(@:run-%=%) $(TRM_GRE)[OK]$(TRM_END)\n"

.PHONY: run
run: $(RUN_TESTS)
