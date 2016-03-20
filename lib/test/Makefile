CC=g++
CPPFLAGS=-Wall -std=c++03
SRC=$(wildcard *.cc)
OBJ=$(SRC:.cc=.o)
DEP=$(SRC:.cc=.d)
TARGET=libut_framework.a

.PHONY: all clean tidy

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

