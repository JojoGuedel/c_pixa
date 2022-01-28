#include <stdlib.h>
#include <string.h>

#include "Pixa/internals.h"
#include "Pixa/scene.h"

Scene *scenes;
size_t scene_c;

int create_scene(void (*onCreate)(), void (*onUpdate)(), void (*onDestroy)())
{
    for (int i = 0; i < scene_c; i++)
    {
        if (scenes[i].onCreate  == NULL &&
            scenes[i].onUpdate  == NULL &&
            scenes[i].onDestroy == NULL)
        {
            scenes[scene_c].is_active      = true;
            scenes[scene_c].onCreate       = onCreate;
            scenes[scene_c].onUpdate       = onUpdate;
            scenes[scene_c].onDestroy      = onDestroy;

            scenes[scene_c].onCreate();

            return i;
        }
    }

    Scene *temp = malloc((scene_c + 1) * sizeof(Scene));
    memcpy(temp, scenes, scene_c * sizeof(Scene));
    free(scenes);
    scenes = temp;

    scenes[scene_c].is_active      = true;
    scenes[scene_c].onCreate       = onCreate;
    scenes[scene_c].onUpdate       = onUpdate;
    scenes[scene_c].onDestroy      = onDestroy;

    scenes[scene_c].onCreate();

    return scene_c++;
}

void destroy_scene(int id)
{
    scenes[id].onDestroy();

    scenes[id].is_active            = false;
    scenes[id].onCreate             = NULL;
    scenes[id].onUpdate             = NULL;
    scenes[id].onDestroy            = NULL;

    // TODO: deallocate
}