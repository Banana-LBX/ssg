#define SSG_IMPLEMENTATION
#include "ssg.h"

#define SSG_SDL_IMPLEMENTATION
#include "backends/ssg_sdl.h"

#define PI 3.14159265359

#define WIDTH 900
#define HEIGHT 900

#define PLAYER_SPEED 3
#define PLAYER_TURN_SPEED 0.08f
#define PLAYER_COLOR (Color){255, 215, 0, 255}

typedef struct {
    float x, y;
    int width, height;
    float angle;
    Color color;
} Player;

void MovePlayer(Player *pacman) {
    if(ssg_window_check_key("W")) {
        pacman->x = pacman->x + PLAYER_SPEED * cosf(pacman->angle);
        pacman->y = pacman->y + PLAYER_SPEED * sinf(pacman->angle);
    }
    if(ssg_window_check_key("S")) {
        pacman->x = pacman->x - PLAYER_SPEED * cosf(pacman->angle);
        pacman->y = pacman->y - PLAYER_SPEED * sinf(pacman->angle);
    }

    if(ssg_window_check_key("A")) {
        pacman->x = pacman->x + PLAYER_SPEED * cosf(pacman->angle-PI/2);
        pacman->y = pacman->y + PLAYER_SPEED * sinf(pacman->angle-PI/2);
    }
    if(ssg_window_check_key("D")) {
        pacman->x = pacman->x + PLAYER_SPEED * cosf(pacman->angle+PI/2);
        pacman->y = pacman->y + PLAYER_SPEED * sinf(pacman->angle+PI/2);
    }

    if(ssg_window_check_key("Left"))
        pacman->angle -= PLAYER_TURN_SPEED;
    if(ssg_window_check_key("Right"))
        pacman->angle += PLAYER_TURN_SPEED;
}

int main(void) {
    SSG_Canvas canvas = ssg_create_canvas(WIDTH, HEIGHT);
    if (!ssg_window_init(canvas, "SSG")) {
        fprintf(stderr, "Failed to initialize SDL backend!\n");
        ssg_free_canvas(canvas);
        return -1;
    }

    Player pacman = {
        .x = WIDTH/2,
        .y = HEIGHT/2,
        .width = 10,
        .height = 10,
        .angle = 0.0f,
        .color = PLAYER_COLOR
    };

    while (ssg_window_running()) {
        ssg_window_begin_frame();

        MovePlayer(&pacman);

        // render things
        ssg_fill(canvas, (Color){0, 0, 0, 255});
        ssg_circle(canvas, pacman.x, pacman.y, pacman.width, pacman.color);
        ssg_line(canvas, pacman.x+10*cosf(pacman.angle), pacman.y+20*sinf(pacman.angle), pacman.x, pacman.y, 1, (Color){0, 0, 0, 255});

        ssg_window_end_frame(canvas);
    }

    ssg_window_shutdown();
    ssg_free_canvas(canvas);

    return 0;
}