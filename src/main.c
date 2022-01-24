#include "pixa/pixa.h"

#include <stdio.h>


void main_onCreate()
{
}

void main_onUpdate()
{
    clear_layer();

    for (int i = 0; i < get_width(); i += 10)
        for(int j = 0; j < get_height(); j += 10)
            draw_pixel(j, i);

    printf("%f\n", get_fps());
}

void main_onDestroy()
{   
}

int main()
{
    create_engine(500, 500, 1, 1);
    create_scene(main_onCreate, main_onUpdate, main_onDestroy);

    set_clear_color(COLOR_VERY_DARK_GREY);

    start_engine();
}