.PHONY: all build clean install
all:build

PREFIX?=.
TOP_DIR?=.

BUILD_DIR?=$(TOP_DIR)/build
SOURCE_DIR?=$(PREFIX)
INC_DIR?=../include
JRELIB?=../build/jre.a
DEBUG?=-g2
STATIC=

HOST_OS:=$(shell uname -s)
GCC_VERSION:=$(shell gcc --version | awk '/^gcc/ {print $$4}')

CC:=g++
CPPFLAGS:=$(DEBUG) -I$(INC_DIR) -fPIC -std=c++11
CPPFLAGS+=-Wall -Wconversion -Werror

LDFLAGS:=
ifeq ($(STATIC),1)
LDFLAGS+=-Wl,-Bstatic
endif
LDFLAGS+=-lpthread
ifeq ($(HOST_OS),Linux)
#-rdynamic
# Pass the flag -export-dynamic to the ELF linker, on targets that support it. This instructs the linker to add all symbols,
# not only used ones, to the dynamic symbol table. This option is needed for some uses of dlopen or to allow obtaining
# backtraces from within a program.
LDFLAGS+=-rdynamic -ldl
#LDFLAGS+=-ldl
endif

SRCS:=$(wildcard $(SOURCE_DIR)/*.cpp)
OBJS:=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TARGETS:=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%,$(SRCS))

TARGETS: $(JRELIB)

$(BUILD_DIR)/%: $(SOURCE_DIR)/%.cpp $(JRELIB)
	@printf "$(CC) $(CPPFLAGS) $^ $(LDFLAGS) -o $@\n"
	$(CC) $(CPPFLAGS) $^ $(LDFLAGS) -o $@

build: create-$(BUILD_DIR) $(TARGETS)

clean:
	rm -rf $(BUILD_DIR)

RUN_TESTS:=$(TARGETS:$(BUILD_DIR)/%=run-%)
ADDR_TESTS:=$(TARGETS:$(BUILD_DIR)/%=addr-%)

.PHONY: create-$(BUILD_DIR)
create-$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

.PHONY: $(RUN_TESTS)
$(RUN_TESTS):
	@printf "====================================\n"
	@printf "$(TRM_WHI)Execute binary $(@:run-%=%)...$(TRM_END)\n"
	-@$(BUILD_DIR)/$(@:run-%=%)
	@printf "\r$(TRM_WHI)Test $(@:run-%=%) $(TRM_GRE)[OK]$(TRM_END)\n"

.PHONY: run
run: $(RUN_TESTS)

ADDR_TESTS:
	addr2line -e $(@:addr-%=%) $(ADDR)
