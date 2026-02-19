#include "../ssg.h"
#include "../ssg_backend.h"
#include <SDL3/SDL.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;

static int width;
static int height;
static int running = 1;

int ssg_init(SSG_Canvas canvas, const char *title) {
    width = canvas.width;
    height = canvas.height;

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return 0;
    }

    window = SDL_CreateWindow(title, width, height, 0);
    if (!window) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    renderer = SDL_CreateRenderer(window, NULL);
    if (!renderer) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );

    if (!texture) {
        SDL_Log("SDL_CreateTexture failed: %s", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 0;
    }

    return 1;
}

void ssg_shutdown(void) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int ssg_running(void) {
    return running;
}

void ssg_begin_frame(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT)
            running = 0;
    }
}

void ssg_end_frame(SSG_Canvas canvas) {
    SDL_UpdateTexture(texture, NULL,
                      canvas.pixels,
                      width * sizeof(Color));

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}