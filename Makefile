# ==============================================================================
# 
#	 ███╗   ███╗ ██████╗ ███╗   ██╗ ██████╗ ██╗     ██╗████████╗██╗  ██╗
#	 ████╗ ████║██╔═══██╗████╗  ██║██╔═══██╗██║     ██║╚══██╔══╝██║  ██║
#	 ██╔████╔██║██║   ██║██╔██╗ ██║██║   ██║██║     ██║   ██║   ███████║
#	 ██║╚██╔╝██║██║   ██║██║╚██╗██║██║   ██║██║     ██║   ██║   ██╔══██║
#	 ██║ ╚═╝ ██║╚██████╔╝██║ ╚████║╚██████╔╝███████╗██║   ██║   ██║  ██║
#	 ╚═╝     ╚═╝ ╚═════╝ ╚═╝  ╚═══╝ ╚═════╝ ╚══════╝╚═╝   ╚═╝   ╚═╝  ╚═╝
#
#                           ░▒▓█ _TMA_ █▓▒░
#
#   Makefile
#   Author     : MrZloHex
#   Date       : 2025-02-01
#   Version    : 1.0
#
#   Description:
#       This Makefile compiles and links the tma project sources.
#       It searches recursively under the "src" directory for source files,
#       compiles them into "obj", and links the final executable in "bin".
#
#   Warning    : This Makefile is so cool it might make your terminal shine!
# ==============================================================================
#
# Verbosity: Set V=1 for verbose output (full commands) or leave it unset for cool, quiet messages.
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

SOURCES = $(shell find $(SRC) -type f -name '*.c')
OBJECTS = $(patsubst $(SRC)/%.c, $(OBJ)/%.o, $(SOURCES))

ifneq ($(strip $(LIB)),)
LIBRARY = $(wildcard $(LIB)/*.c)
OBJECTS += $(patsubst $(LIB)/%.c, $(OBJ)/%.o, $(LIBRARY))
endif

all: $(BIN)/$(TARGET)

$(BIN)/$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN)
	@echo "  CCLD       $(patsubst $(BIN)/%,%,$@)"
	$(Q) $(CC) -o $(BIN)/$(TARGET) $^ $(LDFLAGS)

$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(@D)
	@echo "  CC       $(patsubst $(OBJ)/%,%,$@)"
	$(Q) $(CC) -o $@ -c $< $(CFLAGS)


ifneq ($(strip $(LIB)),)
$(OBJ)/%.o: $(LIB)/%.c
    @mkdir -p $(@D)
    @echo "  CC       $(patsubst $(OBJ)/%,%,$@)"
    $(Q) $(CC) -o $@ -c $< $(CFLAGS)
endif

clean:
	$(Q) rm -rf $(OBJ) $(BIN)

PORT ?= 8080
dry-run: $(BIN)/$(TARGET) 
	@echo "  Launching on port $(PORT)"
	$(Q) ./bin/tma --port $(PORT) -f

INSTALL_PATH ?= /usr/local/bin
install: $(BIN)/$(TARGET)
	@echo "  Installing in $(INSTALL_PATH)"
	$(Q) install $(BIN)/$(TARGET) $(INSTALL_PATH)/tma

.PHONY: all clean dry-run install
