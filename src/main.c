#include <stdio.h>

#include "Pixa/core.h"
#include "Pixa/color.h"

void main_onCreate()
{
}

void main_onUpdate()
{
    clear_layer();

    for (int i = 0; i < 1000; i += 10)
        for(int j = 0; j < 1000; j += 10)
            draw_pixel(j, i);
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