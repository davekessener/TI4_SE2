CC=g++
GIT_ROOT=$(shell git rev-parse --show-toplevel)
PROJECT_ROOT=$(GIT_ROOT)/TI4_SE2_1_1/src
CPPFLAGS=-std=c++03 -Wall -I$(PROJECT_ROOT)/lib
LDFLAGS=-L$(GIT_ROOT)/test
OBJ=Lock.man.o Thread.o Mutex.o
TARGET=LockTest

.PHONY: all

all: $(TARGET)

$(TARGET): $(OBJ)
	$(MAKE) -C $(GIT_ROOT)/test
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET) -lut_framework -lpthread

Lock.man.o: Lock.man.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

Thread.o: $(PROJECT_ROOT)/lib/concurrent/Thread.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

Mutex.o: $(PROJECT_ROOT)/lib/concurrent/Mutex.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)


