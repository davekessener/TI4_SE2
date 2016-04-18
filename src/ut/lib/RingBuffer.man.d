CC=g++
PROJECT_ROOT=$(shell git rev-parse --show-toplevel)/src
CPPFLAGS=-std=c++03 -ggdb -Wall -I$(PROJECT_ROOT)/lib
LDFLAGS=-L$(PROJECT_ROOT)/test
OBJ=RingBuffer.man.o Thread.o Mutex.o Semaphore.o
TARGET=RingBufferTest

.PHONY: all

all: $(TARGET)

$(TARGET): $(OBJ)
	$(MAKE) -C $(PROJECT_ROOT)/test
	$(CC) $(LDFLAGS) $(OBJ) -o $(TARGET) -lut_framework -lpthread

RingBuffer.man.o: RingBuffer.man.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

Thread.o: $(PROJECT_ROOT)/lib/concurrent/Thread.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

Mutex.o: $(PROJECT_ROOT)/lib/concurrent/Mutex.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

Semaphore.o: $(PROJECT_ROOT)/lib/concurrent/Semaphore.cc
	$(CC) $(CPPFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ) $(TARGET)


