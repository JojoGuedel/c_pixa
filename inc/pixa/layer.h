#ifndef PIXA_LAYER_H
#define PIXA_LAYER_H

#include "Pixa/color.h"
#include "Pixa/texture.h"

#define LAYER_DEFAULT 0

typedef struct
{
    int layer;

    Color *clear_color;
    Texture * texture;
} Layer;

int create_layer(int layer, Texture *render_target);
bool destroy_layer(int id);

void set_layer(int id);

#endif