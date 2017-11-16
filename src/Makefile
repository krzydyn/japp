.PHONY: all build clean install

PREFIX?=.
TOP_DIR?=.

BUILD_DIR?=$(TOP_DIR)/build
SOURCE_DIR?=$(PREFIX)
INC_DIR?=../include
DEBUG?=-O2
#SOFLAGS:=-Wl,-soname,jre.so.1

CC:=g++
CPPFLAGS:=$(DEBUG) -I$(INC_DIR) -fPIC -std=c++11
CPPFLAGS+=-Wall -Werror

SRCS:=$(wildcard $(SOURCE_DIR)/lang/*.cpp)
SRCS+=$(wildcard $(SOURCE_DIR)/io/*.cpp)
OBJS:=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TARGETS:=$(BUILD_DIR)/jre.a $(BUILD_DIR)/jre.so

build: create-$(BUILD_DIR) $(TARGETS)

.PHONY: create-$(BUILD_DIR)
create-$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)/lang $(BUILD_DIR)/io

$(TARGETS): $(OBJS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@#printf "cpp $(CC) -c $(CPPFLAGS) $< -o $@\n"
	$(CC) -c $(CPPFLAGS) $< -o $@

$(BUILD_DIR)/%.a:
	$(AR) rcs $@ $^

$(BUILD_DIR)/%.so:
	$(CC) -shared $(SOFLAGS) -o $@ $^

clean:
	rm -rf $(BUILD_DIR)/lang
