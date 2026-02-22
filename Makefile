CC = gcc
TARGET = app

BACKEND ?= image

CFLAGS = -Wall -Wextra -std=c11 -I.
LDFLAGS =

# Backend selection
ifeq ($(BACKEND),image)
    CFLAGS += -DSSG_BACKEND_IMAGE
    BACKEND_HEADER = backends/ssg_image.h

else ifeq ($(BACKEND),ascii)
    CFLAGS += -DSSG_BACKEND_ASCII
    BACKEND_HEADER = backends/ssg_ascii.h

else ifeq ($(BACKEND),sdl)
    CFLAGS += -DSSG_BACKEND_SDL
    BACKEND_HEADER = backends/ssg_sdl.h

    # MSYS2 UCRT64 SDL3
    CFLAGS += $(shell pkg-config --cflags sdl3)
    LDFLAGS += $(shell pkg-config --libs sdl3)

else
    $(error Invalid BACKEND. Use image, ascii, or sdl)
endif

all:
	$(CC) main.c $(CFLAGS) -o $(TARGET) $(LDFLAGS)

clean:
	rm -f $(TARGET)