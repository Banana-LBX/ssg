#define SSG_IMPLEMENTATION
#include "ssg.h"

#define SSG_SDL_IMPLEMENTATION
#include "backends/ssg_sdl.h"

#define PI 3.14159265359

#define FOREGROUND (Color){254, 0, 254, 255}
#define BACKGROUND (Color){20, 20, 20, 255}

#define WIDTH 900
#define HEIGHT 900

#define RADIUS 10
#define GRID_COUNT 25
#define GRID_GAP 0.3/GRID_COUNT
#define GRID_SIZE (GRID_COUNT-1)*GRID_GAP
#define Z_START 0.5

int main(void) {
    SSG_Canvas canvas = ssg_create_canvas(WIDTH, HEIGHT);

    if (!ssg_window_init(canvas, "SSG")) {
        fprintf(stderr, "Failed to initialize SDL backend!\n");
        ssg_free_canvas(canvas);
        return -1;
    }

    float angle = 0;
    float dt;
    while (ssg_window_running()) {
        ssg_window_begin_frame();
        
        dt = ssg_window_get_dt();
        angle += 0.5*PI*dt;

        ssg_fill(canvas, BACKGROUND);

        for(int iy = 0; iy < GRID_COUNT; iy++) {
            for(int ix = 0; ix < GRID_COUNT; ix++) {
                for(int iz = 0; iz < GRID_COUNT; iz++) {
                    float x = ix*GRID_GAP-GRID_SIZE/2;
                    float y = iy*GRID_GAP-GRID_SIZE/2;
                    float z = Z_START+iz*GRID_GAP;

                    // Grid center, rotate shape around y axis
                    float cx = 0.0f;
                    float cz = Z_START+GRID_SIZE/2;

                    ssg_rotate_pointf(&x, &z, cx, cz, angle);

                    x /= z;
                    y /= z;

                    uint32_t r = ix*255/GRID_COUNT;
                    uint32_t g = iy*255/GRID_COUNT;
                    uint32_t b = iz*255/GRID_COUNT;

                    Color color = {
                        r, g, b, 255
                    };
                    ssg_circle(canvas, (x+1)/2*WIDTH, (y+1)/2*HEIGHT, RADIUS, color);
                }
            }
        }

        ssg_text(canvas, "fax\nspinning cube", 0, 770, ssg_default_font, 10, 0.5, 0.5, FOREGROUND);

        ssg_window_end_frame(canvas);
    }

    ssg_window_shutdown();
    ssg_free_canvas(canvas);

    return 0;
}