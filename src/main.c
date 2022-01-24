#include "pixa/pixa.h"

#include <stdio.h>

// Texture *t;
void main_onCreate()
{
    // t = create_texture(10, 10, false, true);

    // draw_pixel(10, 10);
}

void main_onUpdate()
{
    clear_layer();

    // for (int i = 9; i < get_width(); i += 10)
    //     for(int j = 9; j < get_height(); j += 10)
    //         draw_pixel(j, i);


    printf("%f\n", get_fps());
}

void main_onDestroy()
{   
}

int main()
{
    create_engine(500, 500, 2, 2);

    set_clear_color(COLOR_VERY_DARK_GREY);

    int main_scene = create_scene(main_onCreate, main_onUpdate, main_onDestroy);

    start_engine();
    exit_engine();
    destroy_engine();
}