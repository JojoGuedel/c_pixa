#include "pixa/pixa.h"

#include <stdio.h>

// Texture *t;
void main_onCreate()
{
    // t = create_texture(10, 10, false, true);
}

void main_onUpdate()
{
    draw_background(COLOR_DARK_GRAY);
    draw_pixel(10, 10, COLOR_WHITE);

    // draw_texture(t);
}

void main_onDestroy()
{   
}

int main()
{
    create_engine(500, 500);

    int main_scene = create_scene(main_onCreate, main_onUpdate, main_onDestroy);

    start_engine();
}