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

BUILD_DIR:=$(BUILD_DIR)/bin
ifeq ($(OS),Windows_NT)
    HOST_OS:="WinNT"
    $(error unsuported host system)
else
    HOST_OS:=$(shell uname -s)
endif

GCC_VERSION:=$(shell gcc --version | awk '/^gcc/ {print $$4}')

CC:=g++
CPPFLAGS:=$(DEBUG) -I$(INC_DIR) -fPIC -std=c++11
CPPFLAGS+=-Wall -Wconversion -Werror #-pedantic
CPPFLAGS+=-fsanitize=address
#CPPFLAGS+=-fstack-protector
#CPPFLAGS+=-O2

ifeq ($(HOST_OS),Darwin)
LDFLAGS+=-L/usr/X11/lib
endif


ifeq ($(HOST_OS),Linux)
LDFLAGS:=-rdynamic
endif

ifeq ($(STATIC),1)
LDFLAGS+=-Wl,-Bstatic
endif

#LDFLAGS+=-ljre
LDFLAGS+=-lpthread
LDFLAGS+=-lX11
LDFLAGS+=-fsanitize=address

ifeq ($(STATIC),1)
LDFLAGS+=-Wl,dynamic
endif
ifeq ($(HOST_OS),Linux)
# Pass the flag -export-dynamic to the ELF linker, on targets that support it. This instructs the linker to add all symbols,
# not only used ones, to the dynamic symbol table. This option is needed for some uses of dlopen or to allow obtaining
# backtraces from within a program.
#LDFLAGS+=-ldl
LDFLAGS+=-ldl
endif

SRCS:=$(wildcard $(SOURCE_DIR)/*.cpp)
OBJS:=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TARGETS:=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%,$(SRCS))
#TARGETS:=$(BUILD_DIR)/canary $(BUILD_DIR)/x

TARGETS: $(JRELIB)

$(BUILD_DIR)/%: $(SOURCE_DIR)/%.cpp $(JRELIB)
	$(CC) $(CPPFLAGS) $^ $(LDFLAGS) -o $@

build-awt: $(BUILD_DIR)/awt

build: create-$(BUILD_DIR) $(TARGETS)
	@echo "Build done: $^."

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
