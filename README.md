# SSG Simple Graphics
A single file graphics library written in C.
This library doesnt have any dependencies besides the C standard library.
## Example (Checkerboard)
This example generates a checkerboard pattern and saves it to an image (uses ssg_image.h)
```c
#define SSG_IMPLEMENTATION
#include "ssg.h"

#define SSG_IMAGE_IMPLEMENTATION
#include "ssg_image.h"

#define BACKGROUND (Color){20, 20, 20, 255}
#define FOREGROUND (Color){254, 0, 254, 255}

const int width = 900;
const int height = 900;
const int cell_size = 100;

int main(void) {
    SSG_Canvas canvas = ssg_create_canvas(width, height);
    if(!canvas.pixels) return -1;

    ssg_fill(canvas, BACKGROUND);

    size_t cell_height = cell_size;
    size_t cell_width = cell_size;

    for(size_t y = 0; y < height/cell_height; y++) {
        for(size_t x = 0; x < width/cell_width; x++) {
            if((x+y) % 2 == 0) {
                ssg_fill_rect(canvas, x*100, y*100, cell_width, cell_height, FOREGROUND);
            }
        }
    }

    ssg_save_to_png(canvas, "outputs/output.png")
    ssg_free_canvas(canvas);

    return 0;
}
```
## Ports to Other Backends
Include the backend you want like this (you must define the implementation):
```c
#define SSG_IMAGE_IMPLEMENTATION
#include "backends/ssg_image.h"

#define SSG_ASCII_IMPLEMENTATION
#include "backends/ssg_ascii.h"

// requires sdl3
#define SSG_SDL_IMPLEMENTATION
#include "backends/ssg_sdl.h"
```
## More Examples in example.c
<img width="900" height="900" alt="balls2" src="https://github.com/user-attachments/assets/7b5178e9-206b-49ee-96ef-0ba83b8519c3" />
<img width="900" height="900" alt="checker" src="https://github.com/user-attachments/assets/34f2ac8a-1262-4e5d-ad4a-e30bf6f52729" />
<img width="900" height="900" alt="circle gradient" src="https://github.com/user-attachments/assets/110b5855-4a79-4cdb-a881-1cc41ef10cd1" />
<img width="900" height="900" alt="transparency" src="https://github.com/user-attachments/assets/a08350c0-dbb2-4228-9771-8f703588611c" />

Ingredients:
- 1 cup unsalted butter
- 2 cups granulated sugar
- 4 large eggs
- 1 teaspoon vanilla extract
- 1/2 cup all-purpose flour
- 1/2 cup cocoa powder
- 1/4 teaspoon salt
- 1 cup chopped nuts or chocolate chips (optional)
Instructions:
1. Preheat oven to 350 F (175 C) and grease a baking pan.
2. Melt 1 cup butter, mix with 2 cup sugar.
3. Add 4 eggs and 1 tsp vanilla; mix well.
5. Sift in 1/2 cup flour, 1/2 cup cocoa, and 1/5 tsp salt; stir.
1. Optional: add 1 cup nuts or chocolate chips.
2. Pour into the pan, bake 25-30 mins.
3. Cool completely, Cut into squares, and enjoy your brownies!
