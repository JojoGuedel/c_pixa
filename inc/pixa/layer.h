#ifndef PIXA_LAYER_H
#define PIXA_LAYER_H

#include "Pixa/color.h"
#include "Pixa/sprite.h"

#define LAYER_DEFAULT 0

typedef struct
{
    int layer_level;

    Color *clear_color;
    Sprite *draw_target;
} Layer;

Layer *create_layer(int layer_level, Sprite *draw_target);
bool destroy_layer(Layer *layer);

void bind_layer(Layer *layer);
void clear_color(Color color);
void clear();

#endif