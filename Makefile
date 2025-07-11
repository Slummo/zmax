# Directories
SRC := src
INC := include
BLD := build
OBJ := $(BLD)/obj
BIN := $(BLD)/bin
MODELS := test_models
MODEL_FILE_EXT := .txt

# Compiler and flags
CC := gcc
CFLAGS := 
COMPILE_FLAGS := -Wall  -Wextra -Wshadow -I$(INC)
LIBS := -lc -lgsl -lgslcblas -lm

DEBUG ?= 0

ifeq ($(DEBUG),1)
  BUILD_FLAGS := $(CFLAGS) $(COMPILE_FLAGS) -g -O0
else
  BUILD_FLAGS := $(CFLAGS) $(COMPILE_FLAGS) -O2
endif

# Target
TARGET := zmax

# Source files
SRCS := $(shell find $(SRC) -type f -name "*.c")

# Object files
OBJS := $(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))

# Default target
all: $(BIN)/$(TARGET)

# Link
$(BIN)/$(TARGET): $(OBJS)
	@mkdir -p $(BIN)
	$(CC) $(BUILD_FLAGS) $^ $(LIBS) -o $@

# Compile all .c files to .o files
$(OBJ)/%.o: $(SRC)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(BUILD_FLAGS) -c $< -o $@

# Run release
run: all
	@if [ -n "$(ARGS)" ]; then \
		./$(BIN)/$(TARGET) $(MODELS)/$(ARGS)$(MODEL_FILE_EXT); \
	else \
		./$(BIN)/$(TARGET); \
	fi

# Call make with debug
debug:
	$(MAKE) DEBUG=1 all

# Run debugger
drun: DEBUG := 1
drun: DEBUG := 1
drun: all
	@if [ -n "$(ARGS)" ]; then \
		gdb --args ./$(BIN)/$(TARGET) $(MODELS)/$(ARGS)$(MODEL_FILE_EXT); \
	else \
		gdb ./$(BIN)/$(TARGET); \
	fi

# Run with Valgrind
valgrind: DEBUG := 1
valgrind: all
	@if [ -n "$(ARGS)" ]; then \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(BIN)/$(TARGET) $(MODELS)/$(ARGS)$(MODEL_FILE_EXT); \
	else \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --verbose ./$(BIN)/$(TARGET); \
	fi

valgrind_summary: DEBUG := 1
valgrind_summary: all
	@if [ -n "$(ARGS)" ]; then \
		valgrind --leak-check=full ./$(BIN)/$(TARGET) $(MODELS)/$(ARGS)$(MODEL_FILE_EXT); \
	else \
		valgrind --leak-check=full ./$(BIN)/$(TARGET); \
	fi

# Clean build artifacts
clean:
	rm -rf $(BLD)

.PHONY: all run debug drun valgrind valgrind_summary clean
