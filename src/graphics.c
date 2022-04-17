#include "Pixa/color.h"
#include "Pixa/graphics.h"
#include "Pixa/internals.h"
#include "Pixa/sprite.h"

Color color_target;

void color(Color color) {
    color_target = color;
}

void draw_pixel(int x, int y) {
    sprite_draw_pixel(layer_target->draw_target, x, y, color_target);
}

void draw_line(int x1, int y1, int x2, int y2) {
    sprite_draw_line(layer_target->draw_target, x1, y1, x2, y2, color_target);
}

void draw_rect(int x, int y, int width, int height) {
    sprite_draw_rect(layer_target->draw_target, x, y, width, height, color_target);
}

void fill_rect(int x, int y, int width, int height) {
    sprite_fill_rect(layer_target->draw_target, x, y, width, height, color_target);
}
