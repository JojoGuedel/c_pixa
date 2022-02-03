#ifndef PIXA_SPRITE_H
#define PIXA_SPRITE_H

#include <stdbool.h>

#include "Pixa/color.h"

typedef struct
{
    unsigned int id;

    int width;
    int height;

    int x;
    int y;

    float scale_x;
    float scale_y;

    Color tint;
    Color *data;
} Sprite;

Sprite *create_sprite(int width, int height, bool filtered, bool clamp);
void destroy_sprite(Sprite *sprite);

Sprite *copy_sprite(Sprite *src);
void copy_sprite_data(Sprite *dst, Sprite *src);

void update_sprite(Sprite *sprite);  

void draw_sprite(Sprite *sprite);
void clear_sprite(Sprite *sprite, Color color);

void sprite_draw_pixel(Sprite *sprite, int x, int y, Color color);
void sprite_draw_line(Sprite *sprite, int x1, int y1, int x2, int y2, Color color);
void sprite_draw_rect(Sprite *sprite, int x1, int y1, int x2, int y2, Color color);

void set_sprite_pos(Sprite *sprite, int x, int y);
void set_sprite_scale(Sprite *sprite, float scale_x, float scale_y);

#endif