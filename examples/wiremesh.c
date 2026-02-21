#define SSG_IMPLEMENTATION
#include "ssg.h"

#define SSG_BACKEND_IMPLEMENTATION
#include "ssg_backend.h"

#define PI 3.14159265359

#define FOREGROUND (Color){254, 0, 254, 255}
#define BACKGROUND (Color){20, 20, 20, 255}

#define WIDTH 900
#define HEIGHT 900

#define Z_START 0.5
#define CUBE_WIDTH 0.3
#define CUBE_HEIGHT 0.3
#define CUBE_DEPTH 0.3

int main(void) {
    SSG_Canvas canvas = ssg_create_canvas(WIDTH, HEIGHT);

    if (!ssg_window_init(canvas, "SSG")) {
        fprintf(stderr, "Failed to initialize SDL backend!\n");
        ssg_free_canvas(canvas);
        return -1;
    }

    float angle = 0;
    float dt;

    int edges[12][2] = {{0, 1}, {0, 2}, {0, 4}, {1, 3}, {1, 5}, {2, 3}, {2, 6}, {3, 7}, {4, 5}, {4, 6}, {5, 7}, {6, 7}};
    while (ssg_window_running()) {
        ssg_window_begin_frame();
        
        dt = ssg_window_get_dt();
        angle += 0.5*PI*dt;

        ssg_fill(canvas, BACKGROUND);

        float xs[8];
        float ys[8];
        int index = 0;
        for (int ix = -1; ix <= 1; ix += 2) {
            for (int iy = -1; iy <= 1; iy += 2) {
                for (int iz = -1; iz <= 1; iz += 2) {
                    float x = ix * (CUBE_WIDTH * 0.5f);
                    float y = iy * (CUBE_HEIGHT * 0.5f);
                    float z = iz * (CUBE_DEPTH * 0.5f) + Z_START;

                    float cx = 0.0f;
                    float cz = Z_START;

                    ssg_rotate_pointf(&x, &z, cx, cz, angle);

                    float px = x / z;
                    float py = y / z;

                    px = px * (WIDTH * 0.5f) + (WIDTH * 0.5f);
                    py = py * (HEIGHT * 0.5f) + (HEIGHT * 0.5f);

                    xs[index] = px;
                    ys[index] = py;

                    index++;
                }
            }
        }

        for (int i = 0; i < 12; i++) {
            int a = edges[i][0];
            int b = edges[i][1];

            ssg_line(canvas,
                     xs[a], ys[a],
                     xs[b], ys[b],
                     0, FOREGROUND);
        }

        ssg_window_end_frame(canvas);
    }

    ssg_window_shutdown();
    ssg_free_canvas(canvas);

    return 0;
}