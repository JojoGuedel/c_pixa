#include "pixa/pixa.h"

#include <stdio.h>

// Texture *t;
void main_onCreate()
{
    // t = create_texture(10, 10, false, true);
}

void main_onUpdate()
{
    clear_layer(COLOR_DARK_GRAY);

    draw_line(0, 250, 500, 250, COLOR_WHITE);
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