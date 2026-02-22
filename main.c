#define SSG_IMPLEMENTATION
#include "ssg.h"

#define SSG_ASCII_IMPLEMENTATION
#include "backends/ssg_ascii.h"

#define WIDTH 400
#define HEIGHT 400

#define BACKGROUND (Color){0, 0, 0, 0}
#define FOREGROUND (Color){254, 0, 254, 255}

int main(void) {
    SSG_Canvas canvas = ssg_create_canvas(WIDTH, HEIGHT);
    if(!canvas.pixels) {
        printf("Canvas creation failed\n");
        return 1;
    }

    ssg_fill(canvas, BACKGROUND);
    ssg_circle(canvas, 200, 200, 100, FOREGROUND);

    ssg_render_ascii(canvas, " .:a@#");

    ssg_free_canvas(canvas);

    return 0;
}