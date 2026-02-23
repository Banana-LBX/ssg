#define SSG_IMPLEMENTATION
#include "ssg.h"

#define SSG_ASCII_IMPLEMENTATION
#include "backends/ssg_ascii.h"

#define SSG_IMAGE_IMPLEMENTATION
#include "backends/ssg_image.h"

#define WIDTH 800
#define HEIGHT 800

#define BACKGROUND (Color){0, 0, 0, 255}
#define FOREGROUND (Color){254, 0, 254, 255}

int main(void) {
    SSG_Canvas canvas = ssg_create_canvas(WIDTH, HEIGHT);
    if(!canvas.pixels) {
        printf("Canvas creation failed\n");
        return 1;
    }

    ssg_fill(canvas, BACKGROUND);
    ssg_triangle(canvas, 400, 200, 200, 600, 600, 600, (Color){254, 0, 254, 255});
    ssg_circle(canvas, 300, 300, 100, (Color){0, 220, 10, 100});
    ssg_circle(canvas, 500, 500, 150, (Color) {0, 10, 220, 50});

    ssg_canvas_downscale(&canvas, 5, 5);
    ssg_render_ascii(canvas, " .'`^\",:;Il!i~+_-?][}{1)(|\\/*tfjrxnuvczXYUJCLQ0OZmwqpdbkhao*#MW&8%B@$");
    ssg_save_to_png(canvas, "outputs/output.png");

    ssg_free_canvas(canvas);

    return 0;
}