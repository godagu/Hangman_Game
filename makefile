# Compiler
CC = gcc

# Paths
SDL2_PATH = /usr/lib/x86_64-linux-gnu
INCLUDE_PATH = -Iinclude -Isdl2_ttf/font/include
LIB_PATH = -Llib -Lsdl2_ttf/font/lib -L$(SDL2_PATH)

# Compiler Flags
CFLAGS = -Wall -Wextra $(INCLUDE_PATH)

# Libraries
LDFLAGS = $(LIB_PATH) -lSDL2main -lSDL2 -lSDL2_ttf

# Source Files
SRC = $(wildcard *.c)

# Output Executable
OUTPUT = hangman

# Build Target
all: $(OUTPUT)

$(OUTPUT): $(SRC)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(SRC) $(LDFLAGS)

# Clean up compiled files
clean:
	rm -f $(OUTPUT)
