#include <stdio.h>

#include "Pixa/core.h"
#include "Pixa/color.h"

void main_onCreate()
{
}

void main_onUpdate()
{
    clear_layer();

    // for (int i = 0; i < width; i += 10)
    //     for(int j = 0; j < height; j += 10)
    //         draw_pixel(j, i);

    // printf("%f\n", framerate);
}

void main_onDestroy()
{
}

int main(int argc, const char *args[])
{
    create_engine(1000, 1000, 2, 2);
    create_scene(main_onCreate, main_onUpdate, main_onDestroy);

    set_clear_color(COLOR_VERY_DARK_GREY);

    start_engine();
}