.PHONY: all build clean install

TOP_DIR?=.
PREFIX?=.

BUILD_DIR?=$(TOP_DIR)/build
SOURCE_DIR?=$(PREFIX)
INC_DIR?=../include
DEBUG?=-g2
#SOFLAGS:=-Wl,-soname,jre.so.1

ifeq ($(OS),Windows_NT)
    HOST_OS:="WinNT"
else
    HOST_OS:=$(shell uname -s)
endif

CC:=g++
CPPFLAGS:=$(DEBUG) -I$(INC_DIR) -fPIC -std=c++11
CPPFLAGS+=-Wall -Wconversion -Werror
#CPPFLAGS+=-O2
#CPPFLAGS+=-fstack-protector

SRC:=
SRCS+=$(wildcard $(SOURCE_DIR)/*.cpp)
SRCS+=$(wildcard $(SOURCE_DIR)/lang/*.cpp)
SRCS+=$(wildcard $(SOURCE_DIR)/io/*.cpp)
SRCS+=$(wildcard $(SOURCE_DIR)/net/*.cpp)
SRCS+=$(wildcard $(SOURCE_DIR)/nio/*.cpp)
SRCS+=$(wildcard $(SOURCE_DIR)/util/*.cpp)
SRCS+=$(wildcard $(SOURCE_DIR)/awt/*.cpp)
ifeq ($(HOST_OS),WinNT)
SRCS+=$(wildcard $(SOURCE_DIR)/awt/win/*.cpp)
endif
ifeq ($(HOST_OS),Linux)
SRCS+=$(wildcard $(SOURCE_DIR)/awt/x11/*.cpp)
HAS_GRAPHICS=1
endif
ifeq ($(HOST_OS),Darwin)
SRCS+=$(wildcard $(SOURCE_DIR)/awt/x11/*.cpp)
HAS_GRAPHICS=1
endif
ifndef HAS_GRAPHICS
$(warning no graphics supported for your os)
endif
SRCS+=$(wildcard $(SOURCE_DIR)/swing/*.cpp)

OBJS:=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TARGETS:=$(BUILD_DIR)/jre.a #$(BUILD_DIR)/jre.so
SRC_BUILD_DIRS:=$(dir $(OBJS))

build: create-build_dir $(BUILD_DIR)/srcdep.mk $(TARGETS)

.PHONY: create-build_dir
.PHONY: $(SRC_BUILD_DIRS)
create-build_dir: $(SRC_BUILD_DIRS)
	mkdir -p $^

$(TARGETS): $(OBJS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@#printf "cpp $(CC) -c $(CPPFLAGS) $< -o $@\n"
	$(CC) -c $(CPPFLAGS) $< -o $@

$(BUILD_DIR)/%.a:
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.so:
	$(CC) -shared $(SOFLAGS) -o $@ $^

clean: $(SRC_BUILD_DIRS)
	if [ "$(PREFIX)" = "." ]; then rm -rf $(BUILD_DIR); else rm -rf $^; fi

$(BUILD_DIR)/srcdep.mk: $(SRCS)
	rm -f $(BUILD_DIR)/srcdep.mk
	$(CC) $(CPPFLAGS) -MM $^ > $(BUILD_DIR)/srcdep.mk


include $(BUILD_DIR)/srcdep.mk
