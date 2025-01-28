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

all: clean compile

compile: dirs binary

dirs:
	-mkdir -v obj bin

binary: $(OBJECTS)
	$(CC) -o $(BIN)/$(TARGET) $^ $(LDFLAGS)
$(OBJ)/%.o: $(SRC)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)
$(OBJ)/%.o: $(LIB)/%.c
	$(CC) -o $@ -c $< $(CFLAGS)

test: compile
	./bin/tma --port 8080 >> test.log
	tail -f test.log
kill:
	killall tma

clean:
	-rm $(BIN)/*
	-rm $(OBJECTS)

install: compile
	install ./bin/tma /home/mzh/.local/tma


