.PHONY: all clean install

TRM_RED:=$(shell printf "\033[0;31m")
TRM_GRE:=$(shell printf "\033[0;32m")
TRM_YEL:=$(shell printf "\033[0;33m")
TRM_BLU:=$(shell printf "\033[0;34m")
TRM_WHI:=$(shell printf "\033[1;37m")
TRM_END:=$(shell printf "\033[0m")

export TOP_DIR:=$(CURDIR)
export BUILD_DIR:=$(TOP_DIR)/build
export INC_DIR:=include
export JRELIB:=build/jre.a

#export DEBUG=-O3

SUBDIRS:=src tests

BUILD_DIRS:=$(SUBDIRS:%=build-%)
CLEAN_DIRS:=$(SUBDIRS:%=clean-%)
.PHONY: $(BUILD_DIRS) $(CLEAN_DIRS)

#all:
rebuild:
	make clean
	make all

all: $(BUILD_DIRS)
clean: $(CLEAN_DIRS)
	@rm -rf $(BUILD_DIR)

$(BUILD_DIRS):
	@printf "$(TRM_WHI)Building module $(@:build-%=%)...$(TRM_END)\n"
	@$(MAKE) PREFIX=$(@:build-%=%) -f $(@:build-%=%)/Makefile build
	@printf "\r$(TRM_WHI)Building module $(@:build-%=%)  $(TRM_GRE)[OK]$(TRM_END)\n"

$(CLEAN_DIRS):
	@printf "$(TRM_WHI)Clean $(@:clean-%=%)...$(TRM_END)\n"
	@$(MAKE) -s PREFIX=$(@:clean-%=%) -f $(@:clean-%=%)/Makefile clean

# special cases
build-tests: build-src

run-tests:
	@$(MAKE) -s -C $(@:run-%=%) BUILD_DIR=$(BUILD_DIR)/$(@:run-%=%) SOURCE_DIR=$(CURDIR)/$(@:run-%=%) run
