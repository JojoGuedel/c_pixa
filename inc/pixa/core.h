#ifndef PIXA_CORE_H
#define PIXA_CORE_H

#include <stdbool.h>

#include "Pixa/color.h"

void create_engine();
void destroy_engine();

void start_engine();
void stop_engine();

int create_scene(void (*onCreate)(), void (*onUpdate)(), void (*onDestroy)());
void destroy_scene(int id);

void create_layers(int count);
void destroy_layers(int count);

void set_scene_active(int scene, bool active);
bool set_scene_layer(int scene, int layer);
void set_layer(int id);
void set_color(Color color);
void set_clear_color(Color color);
void set_title(const char *title);

void draw_pixel(int x, int y);
void clear_layer();

#endif