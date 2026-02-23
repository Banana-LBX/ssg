#define SSG_IMPLEMENTATION
#include "ssg.h"

#define SSG_SDL_IMPLEMENTATION
#include "backends/ssg_sdl.h"

#define SSG_ASCII_IMPLEMENTATION
#include "backends/ssg_ascii.h"

#define PI 3.14159265359

#define FOREGROUND (Color){254, 0, 254, 255}
#define BACKGROUND (Color){20, 20, 20, 255}

#define WIDTH 900
#define HEIGHT 900

const int rows = 9;
const int cols = 9;
const int cell_size = 100;

int main(void) {
    SSG_Canvas canvas = ssg_create_canvas(WIDTH, HEIGHT);

    float time = 0.0f;

    for (;;) {
        time += 0.01;

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
                ssg_polygon_outline(canvas, xs, ys, 4, 0, FOREGROUND);
            }
        }

        SSG_Canvas render_canvas = ssg_canvas_downscale(canvas, 5, 5);
        ssg_render_ascii(render_canvas, " .:a@#");

        system("cls");
    }

    ssg_free_canvas(canvas);

    return 0;
}
