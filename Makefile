CC = gcc
TARGET = app

CFLAGS = -Wall -Wextra -std=c11 -I.
LDFLAGS = $(shell pkg-config --libs sdl3) -mconsole

# MSYS2 UCRT64 SDL3
CFLAGS += $(shell pkg-config --cflags sdl3)

all:
	$(CC) main.c $(CFLAGS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)