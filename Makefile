.PHONY: all clean install

export TOP_DIR:=$(CURDIR)
export LIB_DIR:=$(TOP_DIR)/build
export INC_DIR:=$(TOP_DIR)/include
export BUILD_DIR:=$(TOP_DIR)/build

export CC=g++
export CFLAGS+=-g2 -I$(INC_DIR)
export CPPFLAGS+=-g2 -std=c++11 -I$(INC_DIR)

SUBDIRS:=src tests

BUILD_DIRS:=$(SUBDIRS:%=build-%)
CLEAN_DIRS:=$(SUBDIRS:%=clean-%)
.PHONY: $(BUILD_DIRS) $(CLEAN_DIRS)

all: $(BUILD_DIRS)
clean: $(CLEAN_DIRS)
	@rm -rf $(BUILD_DIR)

$(BUILD_DIRS):
	@printf "$(TRM_WHI)Building module $(@:build-%=%)...$(TRM_END)\n"
	@$(MAKE) -s -C $(@:build-%=%) BUILD_DIR=$(BUILD_DIR)/$(@:build-%=%) SOURCE_DIR=$(CURDIR)/$(@:build-%=%) build
	@printf "\r$(TRM_WHI)Building module $(@:build-%=%)...  $(TRM_GRE)[OK]$(TRM_END)\n"

$(CLEAN_DIRS):
	@printf "$(TRM_WHI)Clean $(@:build-%=%)...$(TRM_END)\n"
	@$(MAKE) -s -C $(@:clean-%=%) BUILD_DIR=$(BUILD_DIR)/$(@:clean-%=%) clean

build-tests: build-src
