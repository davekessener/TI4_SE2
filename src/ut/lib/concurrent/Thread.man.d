CC=g++
PROJECT_ROOT=$(shell git rev-parse --show-toplevel)/src
CPPFLAGS=-std=c++03 -Wall -I$(PROJECT_ROOT)/lib
LDFLAGS=-L$(PROJECT_ROOT)/test
OBJ=Thread.man.o Thread.o
TARGET=ThreadTest

.PHONY: all

all: $(TARGET)

$(TARGET): $(OBJ)
	$(MAKE) -C $(PROJECT_ROOT)/test
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET) -lut_framework -lpthread

Thread.man.o: Thread.man.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

Thread.o: $(PROJECT_ROOT)/lib/concurrent/Thread.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)

