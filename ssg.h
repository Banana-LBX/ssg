#ifndef SSG_H
#define SSG_H


#include <stdint.h>

typedef struct {
    uint8_t red;
    uint8_t green;
    uint8_t blue;
} Color;

typedef struct {
    Color *pixels;
    size_t height;
    size_t width;
} SSG_Canvas;

#define SSG_PIXEL(canvas, x, y) (canvas).pixels[(y)*(canvas).width + (x)]

static inline void ssg_min_max_int(int *nums, int count, int *min, int *max) {
    *min = nums[0];
    *max = nums[0];

    for(int i = 1; i < count; i++) {
        if(nums[i] < *min) *min = nums[i];
        if(nums[i] > *max) *max = nums[i];
    }
}

static inline void ssg_swap_int(int *a, int *b) {
    int t = *a;
    *a = *b;
    *b = t;
}

static inline float ssg_lerpf(float a, float b, float t) {
    return a + (b - a)*t;
}

// helper for the triangle_fill
static inline int ssg_triangle_edge(int x0, int y0, int x1, int y1, int x, int y) {
    return (x - x0)*(y1 - y0) - (y - y0)*(x1 - x0);
}

static inline void ssg_fill(SSG_Canvas canvas, Color color) {
    for(size_t y = 0; y < canvas.height; y++) {
        for(size_t x = 0; x < canvas.width; x++) {
            SSG_PIXEL(canvas, x, y) = color;
        }
    }
}

static inline void ssg_fill_rect(SSG_Canvas canvas, int x, int y, size_t width, size_t height, Color color) {
    int start_x = x;
    int start_y = y;
    int end_x = x + width;
    int end_y = y + height;

    for(int py = start_y; py < end_y; py++) {
        if(py >= 0 && py < canvas.height) {
            for(int px = start_x; px < end_x; px++) {
                if(px >= 0 && px < canvas.width) {
                    SSG_PIXEL(canvas, px, py) = color;
                }
            }
        }
    }
}

static inline void ssg_fill_circle(SSG_Canvas canvas, int cx, int cy, int r, Color color) {
    int x1 = cx - r;
    int y1 = cy - r;
    int x2 = cx + r;
    int y2 = cy + r;
    for(int y = y1; y <= y2; y++) {
        if(y >= 0 && y < (int)canvas.height) {
            for(int x = x1; x <= x2; x++) {
                if(x >= 0 && x < (int)canvas.width) {
                    // check if point is inside the circle
                    int dx = x - cx;
                    int dy = y - cy;
                    if(dx*dx + dy*dy <= r*r) {
                        SSG_PIXEL(canvas, x, y) = color;
                    }
                }
            }
        }
    }
}

static inline void ssg_draw_line(SSG_Canvas canvas, int x1, int y1, int x2, int y2, Color color) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    // Bresenhams line algorithm
    while (1) {
        if (x1 >= 0 && x1 < (int)canvas.width &&
            y1 >= 0 && y1 < (int)canvas.height) {
            SSG_PIXEL(canvas, x1, y1) = color;
        }

        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

void ssg_fill_triangle(SSG_Canvas canvas, int x1, int y1, int x2, int y2, int x3, int y3, Color color) {
    int coords_x[3] = {x1, x2, x3};
    int coords_y[3] = {y1, y2, y3};

    int start_x;
    int end_x;
    ssg_min_max_int(coords_x, 3, &start_x, &end_x);
    int start_y;
    int end_y;
    ssg_min_max_int(coords_y, 3, &start_y, &end_y);

    // clamp the bounding box to be inside the screen
    if (start_x < 0) start_x = 0;
    if (start_y < 0) start_y = 0;
    if (end_x >= canvas.width) end_x = canvas.width - 1;
    if (end_y >= canvas.height) end_y = canvas.height - 1;

    for(int y = start_y; y <= end_y; y++) {
        if(y >= 0 && y < canvas.height) {
            for(int x = start_x; x <= end_x; x++) {
                if(x >= 0 && x < canvas.width) {
                    // check if point is in triangle
                    int w1 = ssg_triangle_edge(x2, y2, x3, y3, x, y);
                    int w2 = ssg_triangle_edge(x3, y3, x1, y1, x, y);
                    int w3 = ssg_triangle_edge(x1, y1, x2, y2, x, y);

                    if ((w1 >= 0 && w2 >= 0 && w3 >= 0) ||
                        (w1 <= 0 && w2 <= 0 && w3 <= 0)) {
                        SSG_PIXEL(canvas, x, y) = color;
                    }
                }
            }
        }
    }
}

static inline int ssg_save_to_ppm(SSG_Canvas canvas, const char *file_path) {
    FILE *f = fopen(file_path, "wb");
    if(!f) return errno;

    fprintf(f, "P6\n%zu %zu\n255\n", canvas.width, canvas.height);

    for(size_t i = 0; i < canvas.width * canvas.height; i++) {
        Color pixel = canvas.pixels[i];
        uint8_t bytes[3] = {pixel.red, pixel.green, pixel.blue};
        fwrite(bytes, sizeof(bytes), 1, f);
    }

    fclose(f);
    return 0;
}

static inline SSG_Canvas ssg_create_canvas(size_t width, size_t height) {
    SSG_Canvas canvas;
    canvas.width = width;
    canvas.height = height;
    canvas.pixels = malloc(width * height * sizeof(Color));
    return canvas;
}

static inline void ssg_free_canvas(SSG_Canvas canvas) {
    free(canvas.pixels);
}

#endif // SSG_H