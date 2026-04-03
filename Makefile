CC = gcc
TARGET = app

CFLAGS = -Wall -Wextra -std=c11 -I. -Ibackends $(shell pkg-config --cflags sdl3)
LDFLAGS = $(shell pkg-config --libs sdl3) -lm

all:
	$(CC) main.c $(CFLAGS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)
