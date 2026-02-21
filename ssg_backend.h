#ifndef SSG_BACKEND_H
#define SSG_BACKEND_H

#include <SDL3/SDL.h>
#include <errno.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#ifdef __cplusplus
extern "C" {
#endif

// SDL Backend
int  ssg_window_init(SSG_Canvas canvas, const char *title);
void ssg_window_shutdown(void);
int  ssg_window_running(void);
float ssg_window_get_dt(void);
void ssg_window_begin_frame(void);
void ssg_window_end_frame(SSG_Canvas canvas);

// Images Backend
int ssg_save_to_ppm(SSG_Canvas canvas, const char *file_path);
int ssg_save_to_png(SSG_Canvas canvas, const char *file_path);

#ifdef __cplusplus
}
#endif

#endif // SSG_BACKEND_H


#ifdef SSG_BACKEND_IMPLEMENTATION

/* ===== SDL ===== */
static SDL_Window* window = NULL;
static SDL_Renderer* renderer = NULL;
static SDL_Texture* texture = NULL;

static int width;
static int height;
static int running = 1;

int ssg_window_init(SSG_Canvas canvas, const char *title) {
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
    SDL_SetRenderVSync(renderer, 1);

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

void ssg_window_end_frame(SSG_Canvas canvas) {
    SDL_UpdateTexture(texture, NULL,
                      canvas.pixels,
                      width * sizeof(Color));

    SDL_RenderClear(renderer);
    SDL_RenderTexture(renderer, texture, NULL, NULL);
    SDL_RenderPresent(renderer);
}

/* ===== IMAGES ===== */
int ssg_save_to_ppm(SSG_Canvas canvas, const char *file_path) {
    FILE *f = fopen(file_path, "wb");
    if(!f) return errno;

    fprintf(f, "P6\n%zu %zu\n255\n", canvas.width, canvas.height);

    for(size_t i = 0; i < canvas.width * canvas.height; i++) {
        Color pixel = canvas.pixels[i];
        uint8_t bytes[3] = {pixel.r, pixel.g, pixel.b};
        fwrite(bytes, sizeof(bytes), 1, f);
    }

    fclose(f);
    return 0;
}

int ssg_save_to_png(SSG_Canvas canvas, const char *file_path) {
    if (!stbi_write_png(file_path, canvas.width, canvas.height, 4, canvas.pixels, canvas.width * sizeof(Color))) {
        fprintf(stderr, "ERROR: could not write output.png\n");
        return errno;
    }

    return 0;
}

#endif // SSG_BACKEND_IMPLEMENTATION