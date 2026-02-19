#include <time.h>
#include <stdbool.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define SSG_IMPLEMENTATION
#include "ssg.h"

#define BACKGROUND (Color){20, 20, 20, 255}
#define FOREGROUND (Color){254, 0, 254, 255}

const size_t width = 900;
const size_t height = 900;

bool checker_board(size_t cell_size) {
    SSG_Canvas canvas = ssg_create_canvas(width, height);
    if(!canvas.pixels) return false;

    ssg_fill(canvas, BACKGROUND);

    size_t cell_height = cell_size;
    size_t cell_width = cell_size;

    for(size_t y = 0; y < height/cell_height; y++) {
        for(size_t x = 0; x < width/cell_width; x++) {
            if((x+y) % 2 == 0) {
                ssg_fill_rect(canvas, x*100, y*100, cell_width, cell_height, FOREGROUND);
            }
        }
    }

    if (!stbi_write_png("outputs/output.png", canvas.width, canvas.height, 4, canvas.pixels, canvas.width * sizeof(Color))) {
        fprintf(stderr, "ERROR: could not write output.png\n");
        return false;
    }
    ssg_free_canvas(canvas);

    return true;
}

bool random_circles(size_t amount) {
    srand(time(NULL));

    SSG_Canvas canvas = ssg_create_canvas(width, height);
    if(!canvas.pixels) return false;

    ssg_fill(canvas, BACKGROUND);

    for(int i = 0; i < amount; i++) {
        int x = ((rand() % 10) * 100) + 10;
        int y = ((rand() % 10) * 100) + 10;
        int r = ((rand() % 10) * 10) + 10;
        ssg_fill_circle(canvas, x, y, r, FOREGROUND);
    }

    if (!stbi_write_png("outputs/output.png", canvas.width, canvas.height, 4, canvas.pixels, canvas.width * sizeof(Color))) {
        fprintf(stderr, "ERROR: could not write output.png\n");
        return false;
    }
    ssg_free_canvas(canvas);

    return true;
}

bool circles_gradient(size_t rows, size_t cols, size_t cell_size, float start, float end) {
    SSG_Canvas canvas = ssg_create_canvas(width, height);
    if(!canvas.pixels) return false;

    for(int y = 0; y < rows; y ++) {
        for(int x = 0; x < cols; x ++) {
            float u = (float)x/cols;
            float v = (float)y/cols;
            float t = (u + v)/2;

            size_t radius = cell_size;
            if(radius > cell_size) radius = cell_size;

            ssg_fill_circle(canvas, x*cell_size + cell_size/2, y*cell_size + cell_size/2, ssg_lerpf(start, end, t), FOREGROUND);
        }
    }

    if (!stbi_write_png("outputs/output.png", canvas.width, canvas.height, 4, canvas.pixels, canvas.width * sizeof(Color))) {
        fprintf(stderr, "ERROR: could not write output.png\n");
        return false;
    }
    ssg_free_canvas(canvas);

    return true;
}

bool transparency(void) {
    SSG_Canvas canvas = ssg_create_canvas(width, height);
    if(!canvas.pixels) return false;

    ssg_fill(canvas, BACKGROUND);
    ssg_fill_triangle(canvas, 400, 200, 200, 600, 600, 600, (Color){254, 0, 254, 255});
    ssg_fill_circle(canvas, 300, 300, 100, (Color){0, 220, 10, 100});
    ssg_fill_rect(canvas, 500, 500, 150, 150, (Color) {0, 10, 220, 50});

    if (!stbi_write_png("outputs/output.png", canvas.width, canvas.height, 4, canvas.pixels, canvas.width * sizeof(Color))) {
        fprintf(stderr, "ERROR: could not write output.png\n");
        return false;
    }
    ssg_free_canvas(canvas);

    return true;
}

int main(void) {
    if(!checker_board(100)) return -1;
    // if(!random_circles(10)) return -1;
    // if(!circles_gradient(10, 10, 100, 10, 80)) return -1;
    // if(!transparency()) return -1;

    return 0;
}