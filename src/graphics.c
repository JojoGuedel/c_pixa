#include "Pixa/color.h"
#include "Pixa/graphics.h"
#include "Pixa/internals.h"
#include "Pixa/texture.h"

Color color_target;

void color(Color color)
{
    color_target = color;
}

void draw_pixel(int x, int y)
{
    texture_draw_pixel(layer_target->draw_target, x, y, color_target);
}

void draw_line(int x1, int y1, int x2, int y2)
{
    texture_draw_line(layer_target->draw_target, x1, y1, x2, y2, color_target);
}