SHELL=/bin/bash

# When using make alone show the help message
.DEFAULT_GOAL:=help

##################################################
# Source files
##################################################
CWD := $(shell pwd)

.PHONY: all
all: clean MessageDisplay compile run
	@echo "====COMPILE AND RUN DONE===="

.PHONY: compile
compile: MessageDisplay
	mkdir bin
	$(CC) -I./include -lm -O3 -fopenmp --offload-arch=native $(CWD)/$(DIR)/main.c -o bin/$(DIR).o
	@echo "====COMPILE DONE===="

.PHONY: run
run: 
	./bin/$(DIR).o

.PHONY: MessageDisplay
MessageDisplay:
	@echo "    ====    SOLLVE PROJECT MAKEFILE   ====   "
	@echo "Running make with the following compilers"
	@echo CC = $(CC)

.PHONY: clean
clean: 
	- rm -rf bin

.PHONY: help
help:
	@echo "SOLLVE"
	@echo ""
	@echo " === USE ==="
	@echo "  make CC=ccompiler CXX=cppcompiler FC=fortrancompiler [OPTIONS] [RULE]"
	@echo ""
	@echo " === OPTIONS === "
	@echo " === RULES ==="
	@echo "  all"
	@echo "    Build and run SOURCES. If none is specified build and run all the OpenMP test files"
	@echo "  run"
	@echo "    run tests previously build. Filter tests SOURCES list, or all the OpenMP tests that are available within bin/ directory"
	@echo "  compile"
	@echo "    Compile the specific SOURCES files. If none is specified compile all the OpenMP test files"
	@echo "  clean"
	@echo "    Remove all executables from bin/ directory"
