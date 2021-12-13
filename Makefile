.PHONY: all build clean install

TRM_RED:=$(shell printf "\033[0;31m")
TRM_GRE:=$(shell printf "\033[0;32m")
TRM_YEL:=$(shell printf "\033[0;33m")
TRM_BLU:=$(shell printf "\033[0;34m")
TRM_WHI:=$(shell printf "\033[1;37m")
TRM_END:=$(shell printf "\033[0m")

# topdir is top project dir (where top makefile is)
# curdir is build dir (where make is executed from)
TOPDIR:=$(dir $(realpath $(firstword $(MAKEFILE_LIST))))
# or
TOPDIR:=$(dir $(abspath $(lastword $(MAKEFILE_LIST))))

export TOP_DIR:=$(CURDIR)
export BUILD_DIR:=build
export INC_DIR:=include
export JRELIB:=$(BUILD_DIR)/jre.a

export DEBUG=-g3

SUBDIRS:=src tests

BUILD_DIRS:=$(SUBDIRS:%=build-%)
CLEAN_DIRS:=$(SUBDIRS:%=clean-%)
.PHONY: $(BUILD_DIRS) $(CLEAN_DIRS)

all:build

rebuild:
	make clean
	make all

.PHONY: crate-build-dir
crate-build-dir:
	mkdir -p $(BUILD_DIR)

build: crate-build-dir $(BUILD_DIRS)
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

.PHONY: run run-tests
run: run-tests
run-tests: build-tests
	-$(MAKE) PREFIX=$(@:run-%=%) -f $(@:run-%=%)/Makefile run


#instaling binutils on mac:
#brew install binutils
#==> Downloading https://ftp.gnu.org/gnu/binutils/binutils-2.29.1.tar.gz
######################################################################## 100,0%
#==> ./configure --enable-deterministic-archives --program-prefix=g --prefix=/usr/local/Cellar/binutils/2.29.1 --infodir=/usr/local/Cellar/binutils/2.29.1/share/in
#==> make
#==> make install
# 🍺  /usr/local/Cellar/binutils/2.29.1: 113 files, 152.7MB, built in 6 minutes 34 seconds
