#ifndef SSG_IMAGE_H
#define SSG_IMAGE_H

#include "ssg.h"

#ifdef __cplusplus
extern "C" {
#endif

int ssg_save_to_ppm(SSG_Canvas canvas, const char *file_path);
int ssg_save_to_png(SSG_Canvas canvas, const char *file_path);
SSG_Canvas ssg_load_texture(const char *file_path);
void ssg_draw_texture(SSG_Canvas canvas, SSG_Canvas texture, int tex_x, int tex_y, size_t width, size_t height);

#ifdef __cplusplus
}
#endif


#ifdef SSG_IMAGE_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <errno.h>

int ssg_save_to_ppm(SSG_Canvas canvas, const char *file_path) {
    FILE *f = fopen(file_path, "wb");
    if(!f) return errno;

    fprintf(f, "P6\n%zu %zu\n255\n", canvas.width, canvas.height);

    for (size_t y = 0; y < canvas.height; y++) {
        for (size_t x = 0; x < canvas.width; x++) {
            Color pixel = canvas.pixels[y * canvas.stride + x];
            uint8_t bytes[3] = {pixel.r, pixel.g, pixel.b};
            fwrite(bytes, sizeof(bytes), 1, f);
        }
    }

    fclose(f);
    return 0;
}

int ssg_save_to_png(SSG_Canvas canvas, const char *file_path) {
    if (!stbi_write_png(file_path, canvas.width, canvas.height, 4, canvas.pixels, canvas.stride * sizeof(Color))) {
        fprintf(stderr, "ERROR: could not write output.png\n");
        return errno;
    }

    return 0;
}

SSG_Canvas ssg_load_texture(const char *file_path) {
    int width, height, channels;

    unsigned char *data = stbi_load(file_path, &width, &height, &channels, 4);
    if (!data) {
        fprintf(stderr, "Failed to load image: %s\n", file_path);
        return (SSG_Canvas){0};
    }

    SSG_Canvas canvas = ssg_create_canvas(width, height);

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {

            int i = (y * width + x) * 4;

            Color c = {
                .r = data[i + 0],
                .g = data[i + 1],
                .b = data[i + 2],
                .a = data[i + 3],
            };

            canvas.pixels[y * canvas.stride + x] = c;
        }
    }

    stbi_image_free(data);

    return canvas;
}

void ssg_draw_texture(SSG_Canvas canvas, SSG_Canvas texture, int tex_x, int tex_y, size_t width, size_t height) {
    SSG_Canvas scaled = ssg_canvas_resize(texture, width, height);
    if (!scaled.pixels) return;

    for (size_t y = 0; y < scaled.height; y++) {
        for (size_t x = 0; x < scaled.width; x++) {
            Color c = scaled.pixels[y * scaled.stride + x];

            ssg_draw_pixel(canvas, tex_x + x, tex_y + y, c);
        }
    }

    ssg_free_canvas(scaled);
}

#endif // SSG_IMAGE_IMPLEMENTATION

#endif // SSG_IMAGE_H