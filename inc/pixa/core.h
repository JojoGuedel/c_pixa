#ifndef PIXA_CORE_H
#define PIXA_CORE_H

#include <stdbool.h>

extern bool active;

extern int width;
extern int height;

extern double elapsed_time;
extern double delta_time;

void create_engine(int width, int height, int res_x, int res_y);
void destroy_engine();

void start_engine();
void stop_engine();

int create_scene(void (*onCreate)(), void (*onUpdate)(), void (*onDestroy)());
void destroy_scene(int id);

int get_width();
int get_height();

#endif