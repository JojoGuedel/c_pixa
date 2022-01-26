#ifndef PIXA_SCENE_H
#define PIXA_SCENE_H

typedef struct
{
    // TODO: tint
    int layer;
    bool is_active;

    void (*onCreate)();
    void (*onUpdate)();
    void (*onDestroy)();
} Scene;

int create_scene(void (*onCreate)(), void (*onUpdate)(), void (*onDestroy)());
void destroy_scene(int scene);

#endif