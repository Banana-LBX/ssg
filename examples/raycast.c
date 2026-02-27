#define SSG_IMPLEMENTATION
#include "ssg.h"

#define SSG_SDL_IMPLEMENTATION
#include "backends/ssg_sdl.h"

#define PI 3.14159265359

#define WIDTH 1280
#define HEIGHT 800

#define FOV (PI / 3.0f)   // 60 degrees
#define NUM_RAYS WIDTH
#define MAX_DEPTH 1000

#define PLAYER_SPEED 3
#define PLAYER_TURN_SPEED 0.08f
#define PLAYER_RADIUS 10
#define PLAYER_COLOR (Color){255, 215, 0, 255}

typedef struct {
    float x, y;
    int radius;
    float angle;
    Color color;
} Player;

const int map_width = 19;
const int map_height = 21;
const int map_section_size = 40;
const int dot_radius = 20;
const int map[] = {
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
    1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,0,1,
    1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,
    1,1,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,1,1,
    1,0,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,1,
    1,1,1,1,0,1,0,1,1,0,1,1,0,1,0,1,1,1,1,
    2,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,2,
    1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,
    1,0,0,1,0,1,0,0,0,0,0,0,0,1,0,1,0,0,1,
    1,1,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,1,1,
    1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,
    1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1,
    1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,
    1,1,0,1,0,1,0,1,1,1,1,1,0,1,0,1,0,1,1,
    1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,
    1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,
    1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
};

void DrawMap2D(SSG_Canvas canvas, const int *map, Color color, Color dot_color, Color tele_color) {
    float section_gap = map_section_size/10;
    float dot_render_radius = dot_radius/2;
    for(int y = 0; y < map_height; y++) {
        for(int x = 0; x < map_width; x++) {
            if(map[y*map_width+x] == 1)
                ssg_rect(canvas, x*map_section_size, y*map_section_size, map_section_size-section_gap, map_section_size-section_gap, 0, color);
            else if(map[y*map_width+x] == 2)
                ssg_rect(canvas, x*map_section_size, y*map_section_size, map_section_size-section_gap, map_section_size-section_gap, 0, tele_color);
            else if(map[y*map_width+x] == 0)
                ssg_circle(canvas, x*map_section_size+dot_radius, y*map_section_size+dot_radius, dot_render_radius, dot_color);
        }
    }
}

bool IsWall(float x, float y) {
    int map_x = (int)(x / map_section_size);
    int map_y = (int)(y / map_section_size);

    if (map_x < 0 || map_x >= map_width ||
        map_y < 0 || map_y >= map_height)
        return 1; // treat outside map as wall

    return map[map_y * map_width + map_x] == 1;
}

void TryMove(Player *p, float new_x, float new_y) {
    float radius = p->radius;

    //X axis
    if (!IsWall(new_x - radius, p->y - radius) &&
        !IsWall(new_x + radius, p->y - radius) &&
        !IsWall(new_x - radius, p->y + radius) &&
        !IsWall(new_x + radius, p->y + radius)) {
        p->x = new_x;
    }

    //Y axis
    if (!IsWall(p->x - radius, new_y - radius) &&
        !IsWall(p->x + radius, new_y - radius) &&
        !IsWall(p->x - radius, new_y + radius) &&
        !IsWall(p->x + radius, new_y + radius)) {
        p->y = new_y;
    }
}

void Teleport(Player *p)
{
    int map_x = (int)(p->x / map_section_size);
    int map_y = (int)(p->y / map_section_size);

    if (map[map_y * map_width + map_x] == 2) {
        // If left side teleport
        if (map_x == 0) {
            p->x = (map_width - 2) * map_section_size + map_section_size / 2;
        }
        // If right side teleport
        else if (map_x == map_width - 1) {
            p->x = map_section_size + map_section_size / 2;
        }
    }
}

void MovePlayer(Player *pacman) {
    float new_x = pacman->x;
    float new_y = pacman->y;

    if(ssg_window_get_key_down("W")) {
        new_x += PLAYER_SPEED * cosf(pacman->angle);
        new_y += PLAYER_SPEED * sinf(pacman->angle);
    }
    if(ssg_window_get_key_down("S")) {
        new_x -= PLAYER_SPEED * cosf(pacman->angle);
        new_y -= PLAYER_SPEED * sinf(pacman->angle);
    }
    if(ssg_window_get_key_down("A")) {
        new_x += PLAYER_SPEED * cosf(pacman->angle - PI/2);
        new_y += PLAYER_SPEED * sinf(pacman->angle - PI/2);
    }
    if(ssg_window_get_key_down("D")) {
        new_x += PLAYER_SPEED * cosf(pacman->angle + PI/2);
        new_y += PLAYER_SPEED * sinf(pacman->angle + PI/2);
    }

    TryMove(pacman, new_x, new_y);

    if(ssg_window_get_key_down("Left"))
        pacman->angle -= PLAYER_TURN_SPEED;
    if(ssg_window_get_key_down("Right"))
        pacman->angle += PLAYER_TURN_SPEED;
}

void Render3D(SSG_Canvas canvas, Player *p) {
    for (int i = 0; i < NUM_RAYS; i++) {
        float ray_angle = (p->angle - FOV/2.0f) + ((float)i / NUM_RAYS) * FOV;

        float ray_dir_x = cosf(ray_angle);
        float ray_dir_y = sinf(ray_angle);

        float ray_x = p->x;
        float ray_y = p->y;

        float distance = 0;
        bool hit = 0;

        // DDA stepping
        while (!hit && distance < MAX_DEPTH) {
            ray_x += ray_dir_x;
            ray_y += ray_dir_y;
            distance += 1.0f;

            if (IsWall(ray_x, ray_y))
                hit = true;
        }

        if (hit) {
            // Fix fish-eye distortion
            float corrected_dist = distance * cosf(ray_angle - p->angle);

            float wall_height = (map_section_size * HEIGHT) / corrected_dist;

            if (wall_height > HEIGHT)
                wall_height = HEIGHT;

            float shade = 1.0f - (corrected_dist / 600.0f);
            if (shade < 0.2f) shade = 0.2f;

            Color wall_color = {
                (0 * shade),
                (10 * shade),
                (220 * shade),
                255
            };

            ssg_rect(canvas, i, HEIGHT/2 - wall_height/2, 1, wall_height, 0, wall_color);
        }
    }
}

int main(void) {
    SSG_Canvas canvas = ssg_create_canvas(WIDTH, HEIGHT);
    if (!ssg_window_init(canvas, "SSG")) {
        fprintf(stderr, "Failed to initialize SDL backend!\n");
        ssg_free_canvas(canvas);
        return -1;
    }

    Player pacman = {
        .x = (map_width*map_section_size)/2,
        .y = map_height*map_section_size-map_section_size-PLAYER_RADIUS,
        .radius = PLAYER_RADIUS,
        .angle = -PI/2,
        .color = PLAYER_COLOR
    };

    bool draw_map = false;
    while (ssg_window_running()) {
        ssg_window_begin_frame();

        MovePlayer(&pacman);
        Teleport(&pacman);
        if(ssg_window_get_key_pressed("M"))
            draw_map = !draw_map;

        // render things
        ssg_fill(canvas, (Color){0, 0, 0, 255});

        Render3D(canvas, &pacman);

        // draw minimap
        if(draw_map) {
            ssg_circle(canvas, pacman.x, pacman.y, pacman.radius, pacman.color);
            ssg_line(canvas, pacman.x+10*cosf(pacman.angle), pacman.y+20*sinf(pacman.angle), pacman.x, pacman.y, 1, (Color){0, 0, 0, 255});
            DrawMap2D(canvas, map, (Color){255, 255, 255, 255}, (Color){255, 255, 0, 255}, (Color){220, 0, 0, 255});
        }

        ssg_window_end_frame(canvas);
    }

    ssg_window_shutdown();
    ssg_free_canvas(canvas);

    return 0;
}