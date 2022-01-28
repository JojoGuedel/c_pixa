#ifndef PIXA_TEXTURE_H
#define PIXA_TEXTURE_H

#include <stdbool.h>

#include "Pixa/color.h"

typedef struct
{
    unsigned int id;

    int width;
    int height;

    float scale_x;
    float scale_y;

    Color tint;
    Color *data;
} Texture;

Texture *create_texture(int width, int height, bool filtered, bool clamp);
void destroy_texture(Texture *texture);

Texture *copy_texture(Texture *src);
void copy_texture_data(Texture *dst, Texture *src);

void update_texture(Texture *texture);  

void draw_texture(Texture *texture, int x, int y);
void clear_texture(Texture *texture, Color color);

void texture_draw_pixel(Texture *texture, int x, int y, Color color);
void texture_draw_line(Texture *texture, int x1, int y1, int x2, int y2, Color color);

#endif