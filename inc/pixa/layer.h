#ifndef PIXA_LAYER_H
#define PIXA_LAYER_H

#include "Pixa/texture.h"

#define LAYER_DEFAULT 0

typedef struct
{
    int layer;
    Texture * texture;
} Layer;

int create_layer();
bool destroy_layer(int id);

void set_layer(int id);



#endif