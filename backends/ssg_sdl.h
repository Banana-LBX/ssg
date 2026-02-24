#ifndef SSG_SDL_H
#define SSG_SDL_H

#include "ssg.h"

#ifdef __cplusplus
extern "C" {
#endif

int  ssg_window_init(SSG_Canvas canvas, const char *title);
void ssg_window_shutdown(void);
int  ssg_window_running(void);
float ssg_window_get_dt(void);
void ssg_window_begin_frame(void);
bool ssg_window_check_key(const char *key_name);
void ssg_window_end_frame(SSG_Canvas canvas);

#ifdef __cplusplus
}
#endif


#ifdef SSG_SDL_IMPLEMENTATION

#include <stdio.h>
#include <SDL3/SDL.h>

static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;

static int ssg_window_width;
static int ssg_window_height;
static int running = 1;

int ssg_window_init(SSG_Canvas canvas, const char *title) {
    ssg_window_width = canvas.width;
    ssg_window_height = canvas.height;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
        printf("SDL_Init failed: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(title, ssg_window_width, ssg_window_height, 0);
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
    SDL_SetRenderVSync(renderer, 1);

    texture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA32,
        SDL_TEXTUREACCESS_STREAMING,
        ssg_window_width,
        ssg_window_height
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

void ssg_window_shutdown(void) {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int ssg_window_running(void) {
    return running;
}

float ssg_window_get_dt(void) {
    static Uint64 last_counter = 0;
    Uint64 current_counter = SDL_GetPerformanceCounter();

    if (last_counter == 0) {
        last_counter = current_counter;
        return 0.0f; // first frame
    }

    Uint64 frequency = SDL_GetPerformanceFrequency();
    float dt = (float)(current_counter - last_counter) / (float)frequency;
    last_counter = current_counter;

    return dt;
}

void ssg_window_begin_frame(void) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT)
            running = 0;
    }
}

static bool previous[SDL_SCANCODE_COUNT];
bool ssg_window_get_key_pressed(const char *key_name) {
    SDL_Scancode scancode = SDL_GetScancodeFromName(key_name);
    if (scancode == SDL_SCANCODE_UNKNOWN)
        return false;

    const bool *keyboard = SDL_GetKeyboardState(NULL);

    bool pressed = keyboard[scancode] && !previous[scancode];
    previous[scancode] = keyboard[scancode];

    return pressed;
}

bool ssg_window_get_key_down(const char *key_name) {
    SDL_Scancode scancode = SDL_GetScancodeFromName(key_name);

    if (scancode == SDL_SCANCODE_UNKNOWN)
        return false;

    const bool *keyboard = SDL_GetKeyboardState(NULL);

    return keyboard[scancode];
}

void ssg_window_end_frame(SSG_Canvas canvas) {
    SDL_UpdateTexture(texture, NULL,
                      canvas.pixels,
                      ssg_window_width * sizeof(Color));

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

#endif // SSG_SDL_IMPLEMENTATION

#endif // SSG_SDL_H