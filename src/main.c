#include <stdio.h>

#include "Pixa/color.h"
#include "Pixa/core.h"
#include "Pixa/globals.h"
#include "Pixa/graphics.h"
#include "Pixa/layer.h"
#include "Pixa/log.h"

void main_onCreate()
{
}

void main_onUpdate()
{
    clear();

    color(COLOR_WHITE);
    draw_line(10, 10, 20, height - 1);

    color(COLOR_RED);
    draw_pixel(50, 50);

    printf("fps: %f\n", 1.0f / delta_time);
}

void main_onDestroy()
{
}

int main(int argc, const char *args[])
{
    // create engine
    create_engine(500, 500, 4, 4);
    // create scenes
    create_scene(main_onCreate, main_onUpdate, main_onDestroy);
    // set clear color
    clear_color(COLOR_VERY_DARK_GREY);
    // start the engine
    start_engine();
}