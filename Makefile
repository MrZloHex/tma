
V ?= 0
ifeq ($(V),0)
	Q = @
else
	Q =
endif

CC      = gcc
CFLAGS  = -O0 -Wall -Wextra -Wpedantic -std=c2x -Wstrict-aliasing
CFLAGS += -Wno-old-style-declaration -Wno-implicit-fallthroug -Wno-unused-result
CFLAGS += -Wno-pointer-sign
CFLAGS += -Iinc -Ilib -Iinc/ws

LDFLAGS = -lcurl

TARGET  = tma

SRC 	= src
OBJ 	= obj
BIN 	= bin
LIB 	=
TST 	= test

SOURCES = $(shell find $(SRC) -type f -name '*.c')
LIBRARY = $(wildcard $(LIB)/*.c)
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))
OBJECTS += $(patsubst $(LIB)/%.c, $(OBJ)/%.o, $(LIBRARY))

all: clean compile

compile: $(BIN)/$(TARGET) 

$(BIN)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN)
	@echo "  CCLD       $(patsubst $(BIN)/%,%,$@)"
	$(Q) $(CC) -o $(BIN)/$(TARGET) $^ $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)
	@echo "  CC       $(patsubst $(OBJ)/%,%,$@)"
	$(Q) $(CC) -o $@ -c $< $(CFLAGS)

$(OBJ)/%.o: $(LIB)/%.c
	@mkdir -p $(@D)
	@echo "  CC       $(patsubst $(OBJ)/%,%,$@)"
	$(Q) $(CC) -o $@ -c $< $(CFLAGS)

test: compile
	./bin/tma --port 8080 -f
kill:
	killall tma

clean:
	$(Q) rm -rf $(OBJ) $(BIN)

install: compile
	install ./bin/tma /home/mzh/.local/tma


