#ifndef SSG_ASCII_H
#define SSG_ASCII_H

#include "ssg.h"

#ifdef __cplusplus
extern "C" {
#endif

char ssg_pixel_to_ascii(Color pixel, const char *table);
void ssg_render_ascii(SSG_Canvas canvas, const char *table);

#ifdef __cplusplus
}
#endif


#ifdef SSG_ASCII_IMPLEMENTATION

#include <stdint.h>
#include <stdio.h>

char ssg_pixel_to_ascii(Color pixel, const char *table) {
    size_t table_size = strlen(table);

    size_t r = pixel.r;
    size_t g = pixel.g;
    size_t b = pixel.b;

    size_t bright = r;

    if (bright < g) bright = g;
    if (bright < b) bright = b;

    return table[bright*table_size/256];
}

void ssg_render_ascii(SSG_Canvas canvas, const char *table) {
    printf("\033[H");

    size_t width = canvas.width;
    size_t height = canvas.height;

    size_t buffer_size = (width + 1) * height + 1;
    char *buffer = malloc(buffer_size);
    if (!buffer) return;

    char *ptr = buffer;

    for (size_t y = 0; y < height; y++) {
        for (size_t x = 0; x < width; x++) {
            *ptr++ = ssg_pixel_to_ascii(
                canvas.pixels[y * width + x],
                table
            );
        }
        *ptr++ = '\n';
    }

    *ptr = '\0';

    printf("%s", buffer);
    fflush(stdout);

    free(buffer);
}

#endif // SSG_ASCII_IMPLEMENTATION

#endif // SSG_ASCII_H