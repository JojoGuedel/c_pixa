#ifndef PXIA_H
#define PXIA_H

#include <stdint.h>
#include <stdbool.h>

#define COLOR_WHITE             (Color) {255, 255, 255, 255}
#define COLOR_RED               (Color) {255,   0,   0, 255}
#define COLOR_GREEN             (Color) {  0, 255,   0, 255}
#define COLOR_BLUE              (Color) {  0,   0, 255, 255}
#define COLOR_DARK_GRAY         (Color) { 40,  40,  40, 255}
#define COLOR_BLACK             (Color) {  0,   0,   0, 255}

typedef struct
{
    int layer;
    void (*onCreate)();
    void (*onUpdate)();
    void (*onDestroy)();
} Scene;

typedef struct
{
    uint8_t r;
    uint8_t g; 
    uint8_t b;
    uint8_t a;
} Color;

typedef struct
{
    int width;
    int height;

    Color *data;
} Texture;

Texture *create_texture(int width, int height);

void create_engine(int width, int height);
void start_engine();

int create_scene(void (*onCreate)(), void (*onUpdate)(), void (*onDestroy)());
void destroy_scene(int id);

bool set_scene_layer(int id, int l);
bool set_layer(int l);

void create_layers(int count);
void destroy_layers(int count);


void draw_background(Color c);

#endif
