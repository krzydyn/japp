.PHONY: all build clean install

PREFIX?=.
TOP_DIR?=.

BUILD_DIR?=$(TOP_DIR)/build
SOURCE_DIR?=$(PREFIX)
INC_DIR?=../include

CC:=g++
CPPFLAGS:=-I$(INC_DIR) -fPIC -std=c++11

SRCS:=$(wildcard $(SOURCE_DIR)/lang/*.cpp)
OBJS:=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

TARGET:=$(BUILD_DIR)/jre.a

build: create-$(BUILD_DIR) $(BUILD_DIR)/jre.a

.PHONY: create-$(BUILD_DIR)
create-$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)/lang

$(BUILD_DIR)/jre.a: $(OBJS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@printf "cpp $(CC) -c $(CPPFLAGS) $< -o $@"
	$(CC) -c $(CPPFLAGS) $< -o $@

$(BUILD_DIR)/%.a:
	$(AR) rcs $@ $^

clean:
	rm -rf $(BUILD_DIR)/lang
