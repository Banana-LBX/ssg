#include <SDL3/SDL.h>
#include <stdbool.h>
#include <math.h>

#define SSG_IMPLEMENTATION
#include "ssg.h"

#define WIDTH 900
#define HEIGHT 900

#define BACKGROUND (Color){20, 20, 20, 255}
#define FOREGROUND (Color){254, 0, 254, 255}

int main(void)
{
    // Initialize SDL
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }

    // Create window
    SDL_Window *window = SDL_CreateWindow(
        "SSG",
        WIDTH,
        HEIGHT,
        0
    );

    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // Create renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // Create streaming texture
    SDL_Texture *texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        WIDTH,
        HEIGHT
    );

    if (!texture) {
        SDL_Log("SDL_CreateTexture failed: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SSG_Canvas canvas = ssg_create_canvas(WIDTH, HEIGHT);

    bool running = true;
    float t = 0.0f;

    while (running) {
        // Event Handling
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                running = false;
            }
        }

        // Update
        t += 0.02f;

        // Render to software buffer
        ssg_fill(canvas, BACKGROUND);

        int x = WIDTH / 2 + (int)(sin(t) * 250);
        int y = HEIGHT / 2;

        ssg_fill_triangle(canvas, 400, 200, 200, 600, 600, 600, FOREGROUND);
        ssg_fill_circle(canvas, x, y, 100, (Color){0, 220, 10, 100});

        // Upload to GPU
        SDL_UpdateTexture(
            texture,
            NULL,
            canvas.pixels,
            WIDTH * sizeof(Color)
        );

        SDL_RenderClear(renderer);
        SDL_RenderTexture(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);

        // 60 fps
        SDL_Delay(16);
    }

    // Cleanup
    ssg_free_canvas(canvas);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}