#define SSG_IMPLEMENTATION
#include "ssg.h"
#include "ssg_backend.h"

#define PI 3.14159265359

#define FOREGROUND (Color){254, 0, 254, 255}
#define BACKGROUND (Color){20, 20, 20, 255}

const int width = 900;
const int height = 900;

const int rows = 9;
const int cols = 9;
const int cell_size = 100;

int main(void) {
    SSG_Canvas canvas = ssg_create_canvas(width, height);

    if (!ssg_window_init(canvas, "SSG")) {
        fprintf(stderr, "Failed to initialize SDL backend!\n");
        ssg_free_canvas(canvas);
        return -1;
    }

    float time = 0.0f;

    while (ssg_window_running())
    {
        ssg_window_begin_frame();

        float dt = ssg_window_get_dt();
        time += dt;

        ssg_fill(canvas, BACKGROUND);

        float min_radius = 10.0f;
        float max_radius = 50.0f;

        for (int y = 0; y < rows; y++) {
            for (int x = 0; x < cols; x++) {
                float u = (float)x / cols;
                float v = (float)y / rows;

                // Diagonal coordinate (0 → ~1)
                float d = (u + v) * 0.5f;
                // Moving diagonal wave
                float wave = sinf(d * 6.0f - time * 2.0f);
                // Normalize from [-1,1] → [0,1]
                float t = (wave + 1.0f) * 0.5f;

                float radius = min_radius + (max_radius - min_radius) * t;

                // ssg_circle_outline(canvas, x * cell_size + cell_size / 2, y * cell_size + cell_size / 2, radius, FOREGROUND);
                int center_x = x*cell_size+cell_size/2;
                int center_y = y*cell_size+cell_size/2;
                int xs[4] = {center_x-radius, center_x+radius, center_x+radius, center_x-radius};
                int ys[4] = {center_y-radius, center_y-radius, center_y+radius, center_y+radius};
                ssg_polygon_outline(canvas, xs, ys, 4, FOREGROUND);
            }
        }

        ssg_window_end_frame(canvas);
    }

    ssg_window_shutdown();
    ssg_free_canvas(canvas);

    return 0;
}
