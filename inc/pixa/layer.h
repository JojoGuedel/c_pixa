#ifndef PIXA_LAYER_H
#define PIXA_LAYER_H

#include "Pixa/color.h"
#include "Pixa/texture.h"

#define LAYER_DEFAULT 0

typedef struct
{
    int layer;

    Texture *clear_texture;
    Texture *texture;
} Layer;

int create_layer(int layer, Texture *render_target);
bool destroy_layer(int id);

void bind_layer(int id);
void clear_color(Color color);
void clear();

#endif