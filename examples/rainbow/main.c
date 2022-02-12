#include <math.h>

#define LOG_LEVEL LOG_INFO

#include "Pixa/color.h"
#include "Pixa/core.h"
#include "Pixa/graphics.h"
#include "Pixa/log.h"
#include "Pixa/scene.h"

void on_update()
{
    // loop throught each pixel
    for(int x = 0; x < get_width(); x++)
        for (int y = 0; y < get_height(); y++)
        {
            // set the color so it looks cool
            color(color_hsv((sqrt(y*y + x*x) - (int) (elapsed_time * 50)) * 0.5, 100, 100));
            // draw the pixel
            draw_pixel(x, y);
        }

    // get an idea of the performance
    log_info("fps: %f", 1.0f / delta_time);
}

int main(int argc, const char *args[])
{
    // create engine
    engine_create(500, 500, 1, 1);
    // create scenes
    scene_create(NULL, on_update, NULL);
    // set clear color
    clear_color(COLOR_VERY_DARK_GREY); 
    // start the engine
    engine_start();
}