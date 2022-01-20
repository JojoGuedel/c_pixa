#ifndef PXIA_H
#define PXIA_H

typedef struct
{
    void (*onCreate)();
    void (*onUpdate)();
    void (*onDestroy)();
} Scene;

typedef struct
{
    float r;
    float g; 
    float b;
    float a;
} Color;

typedef struct
{
    int width;
    int height;

    Color *data;
} Texture;


const static Color COLOR_WHITE        = {1.0, 1.0, 1.0, 1.0};
const static Color COLOR_RED          = {1.0,   0,   0, 1.0};
const static Color COLOR_GREEN        = {  0, 1.0,   0, 1.0};
const static Color COLOR_BLUE         = {  0,   0, 1.0, 1.0};
const static Color COLOR_DARK_GRAY    = {0.1, 0.1, 0.1, 1.0};
const static Color COLOR_BLACK        = {  0,   0,   0, 1.0};


void create_engine(int width, int height);
void start_engine();

void create_scene(void (*onCreate)(), void (*onUpdate)(), void (*onDestroy)());

Color create_color(int r, int g, int b, int a);

Texture *create_texture(int width, int height);

void draw_background(Color c);

#endif
