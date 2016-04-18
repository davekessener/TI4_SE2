CC=g++
GIT_ROOT=$(shell git rev-parse --show-toplevel)
PROJECT_ROOT=$(GIT_ROOT)/TI4_SE2_1_1/src
CPPFLAGS=-std=c++03 -Wall -I$(PROJECT_ROOT)/lib
LDFLAGS=-L$(GIT_ROOT)/test
OBJ=Thread.man.o Thread.o
TARGET=ThreadTest

.PHONY: all

all: $(TARGET)

$(TARGET): $(OBJ)
	$(MAKE) -C $(GIT_ROOT)/test
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET) -lut_framework -lpthread

Thread.man.o: Thread.man.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

Thread.o: $(PROJECT_ROOT)/lib/concurrent/Thread.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)

