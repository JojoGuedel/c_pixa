#include <stddef.h>
#include <stdlib.h>
#include <string.h>

#include "Pixa/globals.h"
#include "Pixa/internals.h"
#include "Pixa/layer.h"


Layer *layer_default;
Layer *layer_target;
Layer **layer_draw_stack;
size_t layer_draw_stack_count;

#pragma region layers
Layer *create_layer(int layer_level, Texture *draw_target)
{
    Layer *layer = malloc(sizeof(Layer));

    if (draw_target == NULL)
    {
        draw_target = create_texture(width / resolution_x, height / resolution_y, false, false);

        draw_target->scale_x = 1.0f / resolution_x;
        draw_target->scale_y = 1.0f / resolution_y;
    }
    
    layer->layer_level = layer_level;
    layer->draw_target = draw_target;

    layer->clear_color = malloc(layer->draw_target->width * layer->draw_target->height * sizeof(Color));

    for (int i = 0; i < layer->draw_target->width * layer->draw_target->height; i++)
        layer->clear_color[i] = COLOR_BLANK;

    Layer **temp = malloc((layer_draw_stack_count + 1) * sizeof(Layer *));

    for(int offset = 0; offset < layer_draw_stack_count + 1; offset++)
        if (offset == layer_draw_stack_count || layer_draw_stack[offset]->layer_level > layer->layer_level)
        {
            memcpy(temp, layer_draw_stack, offset * sizeof(Layer *));
            temp[offset] = layer;
            memcpy(&temp[offset + 1], &layer_draw_stack[offset], (layer_draw_stack_count - offset) * sizeof(Layer *));

            free(layer_draw_stack);
            layer_draw_stack = temp;
            layer_draw_stack_count++;
            
            break;
        }

    return layer;
}

bool destroy_layer(Layer *layer)
{
    if (layer == LAYER_DEFAULT)
        // LOG: removing layer 0 is illegal
        return false;

    for(int i = 0; i < layer_draw_stack_count; i++)
    {
        if (layer_draw_stack[i] == layer)
        {
            Layer **temp = malloc((layer_draw_stack_count - 1) * sizeof(Layer *));

            memcpy(temp, layer_draw_stack, i * sizeof(Layer *));
            memcpy(&temp[i], &layer_draw_stack[i + 1], (layer_draw_stack_count - i - 1) * sizeof(Layer *));

            free(layer_draw_stack);
            layer_draw_stack = temp;
            layer_draw_stack_count--;

            // TODO: deallocate if removed at the end
            free(layer);

            return true;
        }
    }

    // LOG: layer was not found
    return false;
}

void bind_layer(Layer *layer)
{
    if (layer == 0)
        layer_target = layer_default;

    layer_target = layer;
}

void clear_color(Color color)
{
    for (int i = 0; i < layer_target->draw_target->width * layer_target->draw_target->height; i++)
        layer_target->clear_color[i] = color;
}

void clear()
{
    memcpy(layer_target->draw_target->data,
           layer_target->clear_color,
           layer_target->draw_target->width * layer_target->draw_target->height * sizeof(Color));
}