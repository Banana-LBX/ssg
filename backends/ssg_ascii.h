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
    uint8_t alpha = pixel.a;

    size_t index = (alpha * table_size) / 256;

    if (index >= table_size)
        index = table_size - 1;

    return table[index];
}

void ssg_render_ascii(SSG_Canvas canvas, const char *table) {
    for(size_t y = 0; y < canvas.height; y++) {
        for(size_t x = 0; x < canvas.width; x++) {
            // draw two times to look normal in terminal
            putc(ssg_pixel_to_ascii(canvas.pixels[y*canvas.width+x], table), stdout);
            putc(ssg_pixel_to_ascii(canvas.pixels[y*canvas.width+x], table), stdout);
        }
        putc('\n', stdout);
    }
    fflush(stdout);
}

#endif // SSG_ASCII_IMPLEMENTATION

#endif // SSG_ASCII_H