#include <stdlib.h>
#include <string.h>

#include "Pixa/internals.h"
#include "Pixa/scene.h"

Scene *scenes;
size_t scene_c;

int scene_create(void (*on_create)(), void (*on_update)(), void (*on_destroy)())
{
    for (int i = 0; i < scene_c; i++)
    {
        if (scenes[i].on_create  == NULL &&
            scenes[i].on_update  == NULL &&
            scenes[i].on_destroy == NULL)
        {
            scenes[scene_c].is_active      = true;
            scenes[scene_c].on_create       = on_create;
            scenes[scene_c].on_update       = on_update;
            scenes[scene_c].on_destroy      = on_destroy;

            scenes[scene_c].on_create();

            return i;
        }
    }

    Scene *temp = malloc((scene_c + 1) * sizeof(Scene));
    memcpy(temp, scenes, scene_c * sizeof(Scene));
    free(scenes);
    scenes = temp;

    scenes[scene_c].is_active      = true;
    scenes[scene_c].on_create       = on_create;
    scenes[scene_c].on_update       = on_update;
    scenes[scene_c].on_destroy      = on_destroy;

    if (scenes[scene_c].on_create != NULL)
        scenes[scene_c].on_create();

    return scene_c++;
}

void scene_destroy(int id)
{
    if (scenes[scene_c].on_destroy != NULL)
        scenes[id].on_destroy();

    scenes[id].is_active            = false;
    scenes[id].on_create             = NULL;
    scenes[id].on_update             = NULL;
    scenes[id].on_destroy            = NULL;

    // TODO: deallocate
}