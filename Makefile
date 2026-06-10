CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c99

SRC = src/main.c src/builtins.c
TARGET = nexus

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -I include -o $(TARGET)

clean:
	rm -f $(TARGET)

.PHONY: all clean
