#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define SSG_IMPLEMENTATION
#include "ssg.h"

#define SSG_SDL_IMPLEMENTATION
#include "backends/ssg_sdl.h"

#define WIDTH 900
#define HEIGHT 900
#define CELL_SIZE 2
#define CELL_COLOR (Color){255,255,255,255}
#define EMPTY_CELL_COLOR (Color){0,0,0,255}
#define WORLD_WIDTH WIDTH/CELL_SIZE
#define WORLD_HEIGHT HEIGHT/CELL_SIZE

typedef struct {
    int survive_min;
    int survive_max;
    int birth_min;
    int birth_max;
    int neighbor_size;
} Rules;

void DrawWorld(SSG_Canvas canvas, bool world[WORLD_HEIGHT][WORLD_WIDTH]) {
    for (int y = 0; y < WORLD_HEIGHT; ++y) {
        for (int x = 0; x < WORLD_WIDTH; ++x) {
            if (world[y][x]) {
                ssg_rect(canvas, x*CELL_SIZE, y*CELL_SIZE, CELL_SIZE, CELL_SIZE, 0, CELL_COLOR);
            }
        }
    }
}

void UpdateWorld(bool world[WORLD_HEIGHT][WORLD_WIDTH], Rules rules) {
    bool next[WORLD_HEIGHT][WORLD_WIDTH];
    for (int y = 0; y < WORLD_HEIGHT; ++y) {
        for (int x = 0; x < WORLD_WIDTH; ++x) {
            int neighbors = 0;
            for (int dy = -rules.neighbor_size; dy <= rules.neighbor_size; ++dy) {
                for (int dx = -rules.neighbor_size; dx <= rules.neighbor_size; ++dx) {
                    if (dx == 0 && dy == 0) {
                        continue;
                    }
                    int nx = x + dx;
                    int ny = y + dy;
                    // Bounds check
                    if (nx >= 0 && nx < WORLD_WIDTH && ny >= 0 && ny < WORLD_HEIGHT) {
                        if (world[ny][nx]) {
                            neighbors++;
                        }
                    }
                }
            }

            // Apply rules
            if (world[y][x]) {
                next[y][x] = (neighbors >= rules.survive_min && neighbors <= rules.survive_max);
            } else {
                next[y][x] = (neighbors >= rules.birth_min && neighbors <= rules.birth_max);
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
    Rules rules = {34, 45, 37, 58, 5};
    SSG_Canvas canvas = ssg_create_canvas(WIDTH, HEIGHT);

    if (!ssg_window_init(canvas, "SSG")) {
        fprintf(stderr, "Failed to initialize SDL backend!\n");
        ssg_free_canvas(canvas);
        return -1;
    }

    int side = 2 * rules.neighbor_size + 1;
    int max_neighbors = side * side - 1;
    float target = (rules.birth_min + rules.birth_max) / 2.0f;
    float p = target / max_neighbors;

    if (p < 0.01f) {
        p = 0.01f;
    }
    if (p > 0.99f) {
        p = 0.99f;
    }

    bool world[WORLD_HEIGHT][WORLD_WIDTH];
    for (int y = 0; y < WORLD_HEIGHT; ++y) {
        for (int x = 0; x < WORLD_WIDTH; ++x) {
            world[y][x] = ((float)rand() / RAND_MAX) < p;
        }
    }

    while (ssg_window_running()) {
        ssg_window_begin_frame();
        UpdateWorld(world, rules);
        ssg_fill(canvas, EMPTY_CELL_COLOR);
        DrawWorld(canvas, world);
        ssg_window_end_frame(canvas, 60);
    }

    ssg_window_shutdown();
    ssg_free_canvas(canvas);
    return 0;
}

