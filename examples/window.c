// Example that creates a window and draws a circle
#define SSG_IMPLEMENTATION
#include "ssg.h"
#include "ssg_backend.h"

#define FOREGROUND (Color){254, 0, 254, 255}
#define BACKGROUND (Color){20, 20, 20, 255}

#define WIDTH 900
#define HEIGHT 900

int main(void) {
    SSG_Canvas canvas = ssg_create_canvas(WIDTH, HEIGHT);

    if (!ssg_window_init(canvas, "SSG")) {
        fprintf(stderr, "Failed to initialize SDL backend!\n");
        ssg_free_canvas(canvas);
        return -1;
    }

    while (ssg_window_running()) {
        ssg_window_begin_frame();

        ssg_fill(canvas, BACKGROUND);
        ssg_circle(canvas, WIDTH/2, HEIGHT/2, 200, FOREGROUND);

        ssg_window_end_frame(canvas);
    }

    ssg_window_shutdown();
    ssg_free_canvas(canvas);

    return 0;
}