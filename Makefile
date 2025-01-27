CC = gcc
CFLAGS =  -O0 -Wall -Wextra -Wpedantic -std=c2x -Wstrict-aliasing
CFLAGS += -Wno-old-style-declaration -Wno-implicit-fallthroug -Wno-unused-result
CFLAGS += -Iinc -Ilib

LDFLAGS = -lcurl

TARGET = tma

SRC = src
OBJ = obj
BIN = bin
LIB =
TST = test

SOURCES = $(wildcard $(SRC)/*.c)
LIBRARY = $(wildcard $(LIB)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
OBJECTS += $(patsubst $(LIB)/%.c, $(OBJ)/%.o, $(LIBRARY))

TESTS = $(wildcard $(TST)/*.s)
TEST_RES = $(patsubst $(TST)/%.s, $(BIN)/%.bin, $(TESTS))
TEST_CHK = $(wildcard $(TST)/*.bin)

all: clean compile

compile: binary
test: compile tests

binary: $(OBJECTS)
	$(CC) -o $(BIN)/$(TARGET) $^ $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

$(OBJ)/%.o: $(LIB)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

tests: $(TEST_RES) $(TEST_CHK)

$(TST)/%.bin: $(BIN)/%.bin
	diff $< $@ > /dev/null || ( echo -e "\033[0;31mFailed test: $<\033[0m" >&2 && exit 1)

$(BIN)/%.bin: $(TST)/%.s
	$(BIN)/$(TARGET) -i $< -o $@


clean:
	-rm $(BIN)/*
	-rm $(OBJECTS)

install: all
	install ./bin/tma /home/mzh/.local/tma


