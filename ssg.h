#ifndef SSG_H
#define SSG_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <errno.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    uint8_t r, g, b, a;
} Color;

typedef struct {
    Color *pixels;
    size_t width;
    size_t height;
} SSG_Canvas;

typedef struct {
    int x1, y1;
    int x2, y2;
} SSG_Bounding_Box;

SSG_Canvas ssg_create_canvas(size_t width, size_t height);
void ssg_free_canvas(SSG_Canvas canvas);

void ssg_draw_pixel(SSG_Canvas canvas, int x, int y, Color color);
void ssg_fill(SSG_Canvas canvas, Color color);
bool ssg_bounding_box(SSG_Canvas canvas, int x, int y, int w, int h, SSG_Bounding_Box *box);
void ssg_rect(SSG_Canvas canvas, int x, int y, int width, int height, Color color);
void ssg_circle(SSG_Canvas canvas, int cx, int cy, int r, Color color);
void ssg_line(SSG_Canvas canvas, int x1, int y1, int x2, int y2, Color color);
void ssg_triangle(SSG_Canvas canvas, int x1, int y1, int x2, int y2, int x3, int y3, Color color);

int ssg_save_to_ppm(SSG_Canvas canvas, const char *file_path);

#ifdef __cplusplus
}
#endif

#endif // SSG_H


#ifdef SSG_IMPLEMENTATION

/* ===== Helper functions ===== */
#define SWAP(T, a, b) do { T t = a; a = b; b = t; } while (0)
#define SIGN(T, x) ((T)((x) > 0) - (T)((x) < 0))
#define ABS(T, x) (SIGN(T, x)*(x))

static inline void ssg_min_max_int(int *nums, int count, int *min, int *max) {
    *min = nums[0];
    *max = nums[0];

    for(int i = 1; i < count; i++) {
        if(nums[i] < *min) *min = nums[i];
        if(nums[i] > *max) *max = nums[i];
    }
}

static inline int ssg_triangle_edge(int x0, int y0, int x1, int y1, int x,  int y) {
    return (x - x0)*(y1 - y0) - (y - y0)*(x1 - x0);
}

static inline float ssg_lerpf(float a, float b, float t) {
    return a + (b - a) * t;
}

/* ===== Implementation ===== */

SSG_Canvas ssg_create_canvas(size_t width, size_t height) {
    SSG_Canvas canvas;
    canvas.width  = width;
    canvas.height = height;
    canvas.pixels = malloc(width * height * sizeof(Color));
    if(canvas.pixels)
        memset(canvas.pixels, 0, width * height * sizeof(Color));
    return canvas;
}

void ssg_free_canvas(SSG_Canvas canvas) {
    free(canvas.pixels);
}

void ssg_draw_pixel(SSG_Canvas canvas, int x, int y, Color src) {
    if(x < 0 || y < 0 || x >= (int)canvas.width || y >= (int)canvas.height)
        return;

    Color *dst = &canvas.pixels[y * canvas.width + x];

    if(src.a == 255) {
        *dst = src;
        return;
    }

    if(src.a == 0)
        return;

    uint8_t inv = 255 - src.a;

    // blend colors according to alpha
    dst->r = (uint8_t)((src.r * src.a + dst->r * inv) / 255);
    dst->g = (uint8_t)((src.g * src.a + dst->g * inv) / 255);
    dst->b = (uint8_t)((src.b * src.a + dst->b * inv) / 255);
    // set back to opaque
    dst->a = 255;
}

void ssg_fill(SSG_Canvas canvas, Color color) {
    for(size_t y = 0; y < canvas.height; y++) {
        for(size_t x = 0; x < canvas.width; x++) {
            ssg_draw_pixel(canvas, x, y, color);
        }
    }
}

bool ssg_bounding_box(SSG_Canvas canvas, int x, int y, int w, int h, SSG_Bounding_Box *box) {
    box->x1 = x;
    box->y1 = y;

    // Turn the box into 2 points
    box->x2 = box->x1 + SIGN(int, w)*(ABS(int, w) - 1);
    if (box->x1 > box->x2) SWAP(int, box->x1, box->x2);
    box->y2 = box->y1 + SIGN(int, h)*(ABS(int, h) - 1);
    if (box->y1 > box->y2) SWAP(int, box->y1, box->y2);

    // Remove out of bounds
    if (box->x1 >= (int)canvas.width) return false;
    if (box->x2 < 0) return false;
    if (box->y1 >= (int)canvas.height) return false;
    if (box->y2 < 0) return false;

    // Clamp box inside boundaries
    if (box->x1 < 0) box->x1 = 0;
    if (box->x2 >= (int)canvas.width) box->x2 = (int)canvas.width - 1;
    if (box->y1 < 0) box->y1 = 0;
    if (box->y2 >= (int)canvas.height) box->y2 = (int)canvas.height - 1;

    return true;
}

void ssg_rotate_points(int *xs, int *ys, int count, float cx, float cy, float angle)
{
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    for(int i = 0; i < count; i++) {
        float dx = xs[i] - cx;
        float dy = ys[i] - cy;

        float rx = dx * cosA - dy * sinA;
        float ry = dx * sinA + dy * cosA;

        xs[i] = (int)(rx + cx);
        ys[i] = (int)(ry + cy);
    }
}

void ssg_triangle(SSG_Canvas canvas, int x1, int y1, int x2, int y2, int x3, int y3, Color color) {
    int coords_x[3] = {x1, x2, x3};
    int coords_y[3] = {y1, y2, y3};

    int start_x, end_x;
    int start_y, end_y;

    ssg_min_max_int(coords_x, 3, &start_x, &end_x);
    ssg_min_max_int(coords_y, 3, &start_y, &end_y);

    if(start_x < 0) start_x = 0;
    if(start_y < 0) start_y = 0;
    if(end_x >= (int)canvas.width) end_x = canvas.width - 1;
    if(end_y >= (int)canvas.height) end_y = canvas.height - 1;

    for(int y = start_y; y <= end_y; y++) {
        for(int x = start_x; x <= end_x; x++) {
            int w1 = ssg_triangle_edge(x2, y2, x3, y3, x, y);
            int w2 = ssg_triangle_edge(x3, y3, x1, y1, x, y);
            int w3 = ssg_triangle_edge(x1, y1, x2, y2, x, y);

            if((w1 >= 0 && w2 >= 0 && w3 >= 0) ||
               (w1 <= 0 && w2 <= 0 && w3 <= 0))
                ssg_draw_pixel(canvas, x, y, color);
        }
    }
}

void ssg_polygon(SSG_Canvas canvas, int *xs, int *ys, int count, Color color) {
    for(int i = 1; i < count - 1; i++) {
        ssg_triangle(canvas,
            xs[0], ys[0],
            xs[i], ys[i],
            xs[i+1], ys[i+1],
            color);
    }
}

void ssg_circle(SSG_Canvas canvas, int cx, int cy, int r, Color color) {
    int ax = cx - r;
    int ay = cy - r;
    SSG_Bounding_Box box = {0};
    if(!ssg_bounding_box(canvas, ax, ay, r*2, r*2, &box)) return;
    for(int x = box.x1; x <= box.x2; x++) {
        for(int y = box.y1; y <= box.y2; y++) {
            int dx = x - cx;
            int dy = y - cy;
            if(dx*dx + dy*dy <= r*r)
                ssg_draw_pixel(canvas, x, y, color);
        }
    }
}

void ssg_line(SSG_Canvas canvas, int x1, int y1, int x2, int y2, Color color) {
    // Bresenham line drawing algorithm
    int dx = ABS(int, (x2 - x1));
    int dy = ABS(int, (y2 - y1));

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    while(1) {
        if(x1 >= 0 && x1 < (int)canvas.width && y1 >= 0 && y1 < (int)canvas.height)
            ssg_draw_pixel(canvas, x1, y1, color);

        if(x1 == x2 && y1 == y2) break;

        int e2 = 2 * err;
        if(e2 > -dy) { err -= dy; x1 += sx; }
        if(e2 < dx)  { err += dx; y1 += sy; }
    }
}

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

#endif // SSG_IMPLEMENTATION