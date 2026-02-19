#define SSG_IMPLEMENTATION
#include "ssg.h"
#include "ssg_backend.h"

#define PI 3.14159265359

#define FOREGROUND (Color){254, 0, 254, 255}
#define BACKGROUND (Color){20, 20, 20, 255}

const int width = 960;
const int height = 720;

int main(void) {
    SSG_Canvas canvas = ssg_create_canvas(width, height);

    if (!ssg_window_init(canvas, "SSG")) {
        fprintf(stderr, "Failed to initialize SDL backend!\n");
        ssg_free_canvas(canvas);
        return -1;
    }

    float dt;
    float angle = 0.0f;
    while (ssg_window_running())
    {
        ssg_window_begin_frame();

        dt = ssg_window_get_dt();

        angle += 0.3f*PI*dt;

        ssg_fill(canvas, BACKGROUND);

        int xs[4] = {500, 600, 600, 500};
        int ys[4] = {500, 500, 600, 600};
        ssg_rotate_points(xs, ys, 4, 550, 550, angle);
        ssg_polygon(canvas, xs, ys, 4, FOREGROUND);

        ssg_window_end_frame(canvas);
    }

    ssg_window_shutdown();
    ssg_free_canvas(canvas);

    return 0;
}
