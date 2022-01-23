#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "pixa/pixa.h"
#include "GLFW/glfw3.h"

static bool active;

static int width;
static int height;

// TODO: make resolution scaleable

static Scene *scenes;
static size_t scenes_c;

static int layer;
// IDEA: make the array stack allocated?
static Texture **layers;
static size_t layers_c;

static GLFWwindow* window;


Texture *create_texture(int width, int height)
{
    Texture *tex = malloc(sizeof(Texture));

    tex->width = width;
    tex->height = height;
    tex->data = malloc(width * height * sizeof(Color));

    return tex;
}

void create_engine(int width, int height)
{   
    // TODO: Error checking
    // TODO: Logging

    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    window = glfwCreateWindow(width, height, "pixa", NULL, NULL);
    glfwMakeContextCurrent(window);

    active = true;

    scenes_c = 0;

    layer = 0;
    layers_c = 0;
    create_layers(1);
}

void start_engine()
{
    while (active)
    {
        glfwPollEvents();

        for(int i = 0; i < scenes_c; i++)
        {
            if (!set_layer(scenes[i].layer))
                if (!set_layer(layers_c - 1))
                {
                    layers[0] = create_texture(width, height);
                    layer = 0;
                }

            scenes[i].onUpdate();
        }

        // TODO: layer drawing

        glfwSwapBuffers(window);

        if (scenes_c == 0)
            active = false;

        if (glfwWindowShouldClose(window))
            active = false;
    }

    glfwTerminate();
}

int create_scene(void (*onCreate)(), void (*onUpdate)(), void (*onDestroy)())
{
    for (int i = 0; i < scenes_c; i++)
    {
        if (scenes[i].onCreate  == NULL &&
            scenes[i].onUpdate  == NULL &&
            scenes[i].onDestroy == NULL)
        {
            scenes[i].onCreate  = onCreate;
            scenes[i].onUpdate  = onUpdate;
            scenes[i].onDestroy = onDestroy;

            return i;
        }
    }

    Scene *temp = malloc((scenes_c + 1) * sizeof(Scene));
    memcpy(temp, scenes, scenes_c * sizeof(Scene));
    free(scenes);
    scenes = temp;

    Scene scn;
    scn.onCreate = onCreate;
    scn.onUpdate = onUpdate;
    scn.onDestroy = onDestroy;

    scenes[scenes_c].onCreate = onCreate;
    scenes[scenes_c].onUpdate = onUpdate;
    scenes[scenes_c].onDestroy = onDestroy;
    scenes[scenes_c].onCreate();

    return scenes_c++;
}

void destroy_scene(int id) 
{
    scenes[id].onDestroy();

    scenes[id].onCreate = NULL;
    scenes[id].onUpdate = NULL;
    scenes[id].onDestroy = NULL;
}

bool set_scene_layer(int id, int l) 
{
    if (l >= layers_c || l < 0)
        return false;
    
    scenes[id].layer = l;
    return true;
}

bool set_layer(int l) 
{
    if (l >= layers_c || l < 0)
        return false;

    layer = l;
    return true;
}

void create_layers(int count)
{
    Texture **temp = malloc((layers_c + count) * sizeof(Texture *));
    memcpy(temp, layers, layers_c * sizeof(Texture *));

    free(layers);

    layers = temp;

    for (int i = 0; i < count; i++)
        layers[layers_c + i] = create_texture(width, height);

    layers_c += count;
}

void destroy_layers(int count)
{
    if (count >= layers_c)
        count = layers_c - 1;

    Texture **temp = malloc((layers_c - count) * sizeof(Texture *));
    memcpy(temp, layers, (layers_c - count) * sizeof(Texture *));

    for(int i = 0; i < layers_c; i++)
        free(layers[i]);
    free(layers);

    layers = temp;
    layers_c -= count;
}

void draw_pixel(int x, int y, Color color)
{
    draw_pixel_to_texture(layers[layer], x, y, color);
}

void draw_pixel_to_texture(Texture *texture, int x, int y, Color color)
{
    if (x < 0 || y < 0 || x >= texture->width || y >= texture->height)
        return;
    
    texture->data[y * texture->width + x] = color;
}

void clear_texture(Texture *texture, Color color) {
    for(int y = 0; y < texture->height; y++)
        for(int x = 0; x < texture->width; x++)
            texture->data[y * texture->width + x] = color;
}

// TODO: change this to layer lvl
void draw_background(Color c)
{
    glClearColor(c.r / 255.f, c.g / 255.f, c.b / 255.f, c.a / 255.f);
    glClear(GL_COLOR_BUFFER_BIT);
}