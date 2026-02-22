#ifndef SSG_IMAGE_H
#define SSG_IMAGE_H

#include "ssg.h"

#ifdef __cplusplus
extern "C" {
#endif

int ssg_save_to_ppm(SSG_Canvas canvas, const char *file_path);
int ssg_save_to_png(SSG_Canvas canvas, const char *file_path);

#ifdef __cplusplus
}
#endif


#ifdef SSG_IMAGE_IMPLEMENTATION
#ifndef SSG_IMAGE_STB_IMPLEMENTATION
#define SSG_IMAGE_STB_IMPLEMENTATION

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#endif // SSG_IMAGE_STB_IMPLEMENTATION

#include <errno.h>

int ssg_save_to_ppm(SSG_Canvas canvas, const char *file_path) {
    FILE *f = fopen(file_path, "wb");
    if(!f) return errno;

    fprintf(f, "P6\n%zu %zu\n255\n", canvas.width, canvas.height);

    for(size_t i = 0; i < canvas.width * canvas.height; i++) {
        Color pixel = canvas.pixels[i];
        uint8_t bytes[3] = {pixel.r, pixel.g, pixel.b};
        fwrite(bytes, sizeof(bytes), 1, f);
    }

    fclose(f);
    return 0;
}

int ssg_save_to_png(SSG_Canvas canvas, const char *file_path) {
    if (!stbi_write_png(file_path, canvas.width, canvas.height, 4, canvas.pixels, canvas.width * sizeof(Color))) {
        fprintf(stderr, "ERROR: could not write output.png\n");
        return errno;
    }

    return 0;
}

#endif // SSG_IMAGE_IMPLEMENTATION

#endif // SSG_IMAGE_H