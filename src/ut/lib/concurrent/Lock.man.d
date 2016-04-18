CC=g++
PROJECT_ROOT=$(shell git rev-parse --show-toplevel)/src
CPPFLAGS=-std=c++03 -Wall -I$(PROJECT_ROOT)/lib
LDFLAGS=-L$(PROJECT_ROOT)/test
OBJ=Lock.man.o Thread.o Mutex.o
TARGET=LockTest

.PHONY: all

all: $(TARGET)

$(TARGET): $(OBJ)
	$(MAKE) -C $(PROJECT_ROOT)/test
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET) -lut_framework -lpthread

Lock.man.o: Lock.man.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

Thread.o: $(PROJECT_ROOT)/lib/concurrent/Thread.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

Mutex.o: $(PROJECT_ROOT)/lib/concurrent/Mutex.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)


