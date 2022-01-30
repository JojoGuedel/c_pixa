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
    draw_line((int)elapsed_time % width, 0, (int)elapsed_time % width, height);

    printf("fps: %f\n", 1.0f / delta_time);
}

void main_onDestroy()
{
}

int main(int argc, const char *args[])
{
    // create engine
    create_engine(500, 500, 2, 2);
    // create scenes
    create_scene(main_onCreate, main_onUpdate, main_onDestroy);
    // set clear color
    clear_color(COLOR_VERY_DARK_GREY);
    // start the engine
    start_engine();
}