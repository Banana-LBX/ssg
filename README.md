# SSG
A single file graphics library written in C.
This library doesnt have any dependencies besides the C standard library.
## Example (Checkerboard)
(this example uses [stb_image_write.h](https://raw.githubusercontent.com/nothings/stb/master/stb_image_write.h))
```c
#define SSG_IMPLEMENTATION
#include "ssg.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

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

    if (!stbi_write_png("outputs/output.png", canvas.width, canvas.height, 4, canvas.pixels, canvas.width * sizeof(Color))) {
        fprintf(stderr, "ERROR: could not write output.png\n");
        return -1;
    }
    ssg_free_canvas(canvas);

    return 0;
}
```
## Ports to Other Backends
Include the ssg_backend.h in your code to use pre-made functions for: SDL, WASM(TODO), ASCII(TODO).
The backend header file requires the backends directory which contains the C files for the backends.
```c
#include "ssg_backend.h"
```
## More Examples in example.c
<img width="900" height="900" alt="balls2" src="https://github.com/user-attachments/assets/7b5178e9-206b-49ee-96ef-0ba83b8519c3" />
<img width="900" height="900" alt="checker" src="https://github.com/user-attachments/assets/34f2ac8a-1262-4e5d-ad4a-e30bf6f52729" />
<img width="900" height="900" alt="circle gradient" src="https://github.com/user-attachments/assets/110b5855-4a79-4cdb-a881-1cc41ef10cd1" />
<img width="900" height="900" alt="transparency" src="https://github.com/user-attachments/assets/a08350c0-dbb2-4228-9771-8f703588611c" />
