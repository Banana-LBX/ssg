#ifndef SSG_H
#define SSG_H

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

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

bool ssg_bounding_box(SSG_Canvas canvas, int x, int y, int w, int h, SSG_Bounding_Box *box);
void ssg_rotate_point(int *x, int *y, int cx, int cy, float angle);
void ssg_rotate_pointf(float *x, float *y, float cx, float cy, float angle);
void ssg_rotate_points(int *xs, int *ys, int count, int cx, int cy, float angle);
void ssg_rotate_pointsf(float *xs, float *ys, float count, float cx, float cy, float angle);

void ssg_draw_pixel(SSG_Canvas canvas, int x, int y, Color color);
void ssg_fill(SSG_Canvas canvas, Color color);

void ssg_line(SSG_Canvas canvas, int x1, int y1, int x2, int y2, size_t thickness, Color color);
void ssg_triangle(SSG_Canvas canvas, int x1, int y1, int x2, int y2, int x3, int y3, Color color);
void ssg_polygon(SSG_Canvas canvas, int *xs, int *ys, int count, Color color);
void ssg_polygon_outline(SSG_Canvas canvas, int *xs, int *ys, int count, size_t thickness, Color color);
void ssg_polygon_get_center(int *xs, int *ys, int count, int *cx, int *cy);
void ssg_rect(SSG_Canvas canvas, int x, int y, int width, int height, Color color);
void ssg_rect_outline(SSG_Canvas canvas, int x, int y, int width, int height, size_t thickness, Color color);
void ssg_circle(SSG_Canvas canvas, int cx, int cy, int r, Color color);
void ssg_circle_outline(SSG_Canvas canvas, int cx, int cy, int r, size_t thickness, Color color);

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

bool ssg_bounding_box(SSG_Canvas canvas, int x, int y, int w, int h, SSG_Bounding_Box *box) {
    box->x1 = x;
    box->y1 = y;

    // Turn the box into 2 points
    box->x2 = box->x1 + SIGN(int, w)*(ABS(int, w));
    if (box->x1 > box->x2) SWAP(int, box->x1, box->x2);
    box->y2 = box->y1 + SIGN(int, h)*(ABS(int, h));
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

void ssg_rotate_point(int *x, int *y, int cx, int cy, float angle) {
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    float dx = *x - cx;
    float dy = *y - cy;

    float rx = dx * cosA - dy * sinA;
    float ry = dx * sinA + dy * cosA;

    *x = (int)(rx + cx);
    *y = (int)(ry + cy);
}

void ssg_rotate_pointf(float *x, float *y, float cx, float cy, float angle) {
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    float dx = *x - cx;
    float dy = *y - cy;

    float rx = dx * cosA - dy * sinA;
    float ry = dx * sinA + dy * cosA;

    *x = rx + cx;
    *y = ry + cy;
}

void ssg_rotate_points(int *xs, int *ys, int count, int cx, int cy, float angle) {
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

void ssg_rotate_pointsf(float *xs, float *ys, float count, float cx, float cy, float angle) {
    float cosA = cosf(angle);
    float sinA = sinf(angle);

    for(int i = 0; i < count; i++) {
        float dx = xs[i] - cx;
        float dy = ys[i] - cy;

        float rx = dx * cosA - dy * sinA;
        float ry = dx * sinA + dy * cosA;

        xs[i] = rx + cx;
        ys[i] = ry + cy;
    }
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

void ssg_line(SSG_Canvas canvas, int x1, int y1, int x2, int y2, size_t thickness, Color color) {
    // Bresenham line drawing algorithm
    int dx = ABS(int, (x2 - x1));
    int dy = ABS(int, (y2 - y1));

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int err = dx - dy;

    int half = thickness / 2;

    while (1) {
        // Draw centered rectangle at the current line pixel
        ssg_rect(canvas, x1 - half, y1 - half, thickness, thickness, color);
        if (x1 == x2 && y1 == y2)
            break;

        int e2 = 2 * err;
        if (e2 > -dy) { err -= dy; x1 += sx; }
        if (e2 <  dx) { err += dx; y1 += sy; }
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
    if (count < 2) return;
    for(int i = 1; i < count - 1; i++) {
        ssg_triangle(canvas,
            xs[0], ys[0],
            xs[i], ys[i],
            xs[i+1], ys[i+1],
            color);
    }
}

void ssg_polygon_outline(SSG_Canvas canvas, int *xs, int *ys, int count, size_t thickness, Color color) {
    if (count < 2) return;
    for (int i = 0; i < count; i++) {
        int j = (i + 1) % count;  // wrap to first vertex
        ssg_line(canvas, xs[i], ys[i], xs[j], ys[j], thickness, color);
    }
}

void ssg_polygon_get_center(int *xs, int *ys, int count, int *cx, int *cy) {
    float area = 0.0f;

    for (int i = 0; i < count; i++) {
        int j = (i + 1) % count;

        float cross = xs[i] * ys[j] - xs[j] * ys[i];

        area += cross;

        *cx += (xs[i] + xs[j]) * cross;
        *cy += (ys[i] + ys[j]) * cross;
    }

    area *= 0.5f;

    *cx /= (6.0f * area);
    *cy /= (6.0f * area);
}

void ssg_rect(SSG_Canvas canvas, int x, int y, int width, int height, Color color) {
    int xs[4] = {x, x+width, x+width, x};
    int ys[4] = {y, y, y+height, y+height};
    ssg_polygon(canvas, xs, ys, 4, color);
}

void ssg_rect_outline(SSG_Canvas canvas, int x, int y, int width, int height, size_t thickness, Color color) {
    int xs[4] = {x, x+width, x+width, x};
    int ys[4] = {y, y, y+height, y+height};
    ssg_polygon_outline(canvas, xs, ys, 4, thickness, color);
}

void ssg_circle(SSG_Canvas canvas, int cx, int cy, int r, Color color) {
    int ax = cx - r;
    int ay = cy - r;
    SSG_Bounding_Box box = {0};
    if(!ssg_bounding_box(canvas, ax, ay, r*2, r*2, &box)) return;
    for(int y = box.y1; y <= box.y2; y++) {
        for(int x = box.x1; x <= box.x2; x++) {
            int dx = x - cx;
            int dy = y - cy;
            int dist2 = dx*dx + dy*dy;
            int r2 = r*r;
            if(dist2 <= r2)
                ssg_draw_pixel(canvas, x, y, color);
        }
    }
}

void ssg_circle_outline(SSG_Canvas canvas, int cx, int cy, int r, size_t thickness, Color color) {
    int half = thickness / 2;
    int r_outer = r + half;
    int r_inner = r - half;
    if (r_inner < 0) r_inner = 0;

    int outer2 = r_outer * r_outer;
    int inner2 = r_inner * r_inner;

    // Compute bounding box for the outer circle + thickness
    SSG_Bounding_Box box = {0};
    if (!ssg_bounding_box(canvas, cx - r_outer, cy - r_outer, r_outer * 2, r_outer * 2, &box))
        return;

    // Draw circle outline inside bounding box
    for (int y = box.y1; y <= box.y2; y++) {
        for (int x = box.x1; x <= box.x2; x++) {
            int dx = x - cx;
            int dy = y - cy;
            int dist2 = dx*dx + dy*dy;

            if (dist2 <= outer2 && dist2 >= inner2)
                ssg_draw_pixel(canvas, x, y, color);
        }
    }
}

#endif // SSG_IMPLEMENTATION