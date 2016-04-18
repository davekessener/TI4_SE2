CC=g++
GIT_ROOT=$(shell git rev-parse --show-toplevel)
PROJECT_ROOT=$(GIT_ROOT)/TI4_SE2_1_1/src
CPPFLAGS=-Wall -std=c++03 -I$(PROJECT_ROOT)/lib
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
DEP=$(SRC:.cc=.d)
TARGET=libut_framework.a

.PHONY: all lib clean tidy

%.d: %.cc
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< -o $@; \
	sed -i 's,\($*\)\.o[ :]*,\1.o $@ : ,g' $@;

all: $(TARGET)

clean: tidy
	rm -f $(TARGET)

tidy:
	rm -f $(OBJ) $(DEP) 

$(TARGET): $(OBJ)
	ar rcs $(TARGET) $(OBJ)
	ranlib $(TARGET)

-include $(DEP)

