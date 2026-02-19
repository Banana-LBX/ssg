# Current makefile that includes backend implementations
CC = gcc
TARGET = app
SRC = main.c backends/ssg_sdl.c

CFLAGS = $(shell pkg-config --cflags sdl3)
LDFLAGS = $(shell pkg-config --libs sdl3)

all:
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS) $(LDFLAGS)

run: all
	./$(TARGET)

clean:
	rm -f $(TARGET).exe