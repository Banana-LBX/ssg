#define SSG_IMPLEMENTATION
#include "ssg.h"
#include "ssg_backend.h"

int main(void) {
    SSG_Canvas canvas = ssg_create_canvas(800, 600);

    if (!ssg_init(canvas, "SSG SDL Window")) {
        fprintf(stderr, "Failed to initialize SDL backend!\n");
        ssg_free_canvas(canvas);
        return -1;
    }

    while (ssg_running()) {
        ssg_begin_frame();

        ssg_fill(canvas, (Color){30, 30, 30, 255});

        ssg_fill_triangle(canvas, 400, 100, 200, 500, 600, 500, (Color){255, 0, 0, 255});
        ssg_fill_circle(canvas, 400, 300, 50, (Color){0, 255, 0, 105});

        ssg_end_frame(canvas);
    }

    ssg_shutdown();
    ssg_free_canvas(canvas);

    return 0;
}
