// Conway's Game of Life
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SSG_IMPLEMENTATION
#include "ssg.h"

#define SSG_SDL_IMPLEMENTATION
#include "backends/ssg_sdl.h"

#define WIDTH 900
#define HEIGHT 900

#define CELL_SIZE 5
#define CELL_COLOR (Color){255,255,255,255}
#define EMPTY_CELL_COLOR (Color){0,0,0,255}

#define WORLD_WIDTH WIDTH/CELL_SIZE
#define WORLD_HEIGHT HEIGHT/CELL_SIZE

void DrawWorld(SSG_Canvas canvas, bool world[WORLD_HEIGHT][WORLD_WIDTH]) {
    for (int y = 0; y < WORLD_HEIGHT; ++y) {
        for (int x = 0; x < WORLD_WIDTH; ++x) {
            if (world[y][x])
                ssg_rect(canvas, x*CELL_SIZE, y*CELL_SIZE, CELL_SIZE, CELL_SIZE, 0, CELL_COLOR);
        }
    }
}

void UpdateWorld(bool world[WORLD_HEIGHT][WORLD_WIDTH]) {
    bool next[WORLD_HEIGHT][WORLD_WIDTH];

    for (int y = 0; y < WORLD_HEIGHT; ++y) {
        for (int x = 0; x < WORLD_WIDTH; ++x) {
            int neighbors = 0;

            for (int dy = -1; dy <= 1; ++dy) {
                for (int dx = -1; dx <= 1; ++dx) {

                    if (dx == 0 && dy == 0) continue;

                    int nx = x + dx;
                    int ny = y + dy;

                    // Bounds check
                    if (nx >= 0 && nx < WORLD_WIDTH &&
                        ny >= 0 && ny < WORLD_HEIGHT) {

                        if (world[ny][nx])
                            neighbors++;
                    }
                }
            }

            // Apply rules
            if (world[y][x]) {
                // Alive
                next[y][x] = (neighbors == 2 || neighbors == 3);
            } else {
                // Dead
                next[y][x] = (neighbors == 3);
            }
        }
    }

    for (int y = 0; y < WORLD_HEIGHT; ++y) {
        for (int x = 0; x < WORLD_WIDTH; ++x) {
            world[y][x] = next[y][x];
        }
    }
}

int main(void) {
    srand(time(NULL));

    SSG_Canvas canvas = ssg_create_canvas(WIDTH, HEIGHT);
    if (!ssg_window_init(canvas, "SSG")) {
        fprintf(stderr, "Failed to initialize SDL backend!\n");
        ssg_free_canvas(canvas);
        return -1;
    }

    bool world[WORLD_HEIGHT][WORLD_WIDTH];
    for (int y = 0; y < WORLD_HEIGHT; ++y) {
        for (int x = 0; x < WORLD_WIDTH; ++x) {
            world[y][x]=rand()%2;
        }
    }

    while (ssg_window_running()) {
        ssg_window_begin_frame();

        UpdateWorld(world);

        ssg_fill(canvas, EMPTY_CELL_COLOR);
        DrawWorld(canvas, world);

        ssg_window_end_frame(canvas, 60);
    }

    ssg_window_shutdown();
    ssg_free_canvas(canvas);

    return 0;
}
