#include "pixa/pixa.h"

#include <stdio.h>

// Texture *t;
void main_onCreate()
{
    // t = create_texture(10, 10, false, true);
}

void main_onUpdate()
{
    clear_layer(COLOR_VERY_DARK_GREY);

    for(int i = 0; i < get_width(); i++)
        for(int j = 0; j < get_height(); j++)
            if (i == j)
                draw_pixel(i, j);


    draw_pixel(20, 20);
    // draw_pixel(20, 20);
}

void main_onDestroy()
{   
}

int main()
{
    create_engine(500, 500);

    int main_scene = create_scene(main_onCreate, main_onUpdate, main_onDestroy);

    start_engine();
    exit_engine();
    destroy_engine();
}