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
// TODO: make a struct layers for overview
static Texture **layers;
static size_t layers_c;

// TODO: implement textures for gpu (olc::Decal)

static GLFWwindow* window;

Texture *create_texture(int width, int height, bool filtered, bool clamp)
{
    Texture *tex = malloc(sizeof(Texture));

    tex->width = width;
    tex->height = height;
    tex->data = malloc(width * height * sizeof(Color));

    glGenTextures(1, &tex->id);
    glBindTexture(GL_TEXTURE_2D, tex->id);

    if (filtered)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    }

    if (clamp)
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    }
    else
    {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    // I don't know what this does yet so...
    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    return tex;
}

void update_texture(Texture *texture) 
{
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
}

void create_engine(int w, int h)
{   
    // TODO: Error checking
    // TODO: Logging

    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    window = glfwCreateWindow(w, h, "pixa", NULL, NULL);
    glfwMakeContextCurrent(window);

    active = true;

    width = w;
    height = h;

    scenes_c = 0;

    layer = 0;
    layers_c = 0;
    create_layers(1);
}

void start_engine()
{
    // TODO: multithreading

    while (active)
    {
        glfwPollEvents();

        for(int i = 0; i < scenes_c; i++)
        {
            if (!scenes[i].is_active)
                continue;

            if (layers_c == 0)
                create_layers(1);

            if (!set_layer(scenes[i].layer))
                set_layer(layers_c - 1); 

            scenes[i].onUpdate();
        }

        for(int i = 0; i < layers_c; i++)
        {
            update_texture(layers[i]);
            draw_texture(layers[i], 0, 0);
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
            scenes[i].layer         = 0;
            scenes[i].is_active     = true;
            scenes[i].onCreate      = onCreate;
            scenes[i].onUpdate      = onUpdate;
            scenes[i].onDestroy     = onDestroy;

            return i;
        }
    }

    Scene *temp = malloc((scenes_c + 1) * sizeof(Scene));
    memcpy(temp, scenes, scenes_c * sizeof(Scene));
    free(scenes);
    scenes = temp;

    scenes[scenes_c].layer          = 0;
    scenes[scenes_c].is_active      = true;
    scenes[scenes_c].onCreate       = onCreate;
    scenes[scenes_c].onUpdate       = onUpdate;
    scenes[scenes_c].onDestroy      = onDestroy;

    scenes[scenes_c].onCreate();

    return scenes_c++;
}

void destroy_scene(int id) 
{
    scenes[id].onDestroy();

    scenes[id].layer                = 0;
    scenes[id].is_active            = false;
    scenes[id].onCreate             = NULL;
    scenes[id].onUpdate             = NULL;
    scenes[id].onDestroy            = NULL;
}

void set_scene_active(int scene, bool active)
{
    if (scene < layers_c)
        scenes[scene].is_active = active;
}

bool set_scene_layer(int scene, int level) 
{
    if (level >= layers_c || level < 0)
        return false;
    
    scenes[scene].layer = level;
    return true;
}

bool set_layer(int level)
{
    if (level >= layers_c || level < 0)
        return false;

    layer = level;
    return true;
}

void create_layers(int count)
{
    Texture **temp = malloc((layers_c + count) * sizeof(Texture *));
    memcpy(temp, layers, layers_c * sizeof(Texture *));

    free(layers);

    layers = temp;

    for (int i = 0; i < count; i++)
        layers[layers_c + i] = create_texture(width, height, false, true);

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

void draw_pixel_to_texture(Texture *texture, int x, int y, Color color)
{
    if (x < 0 || y < 0 || x >= texture->width || y >= texture->height)
        return;
    
    texture->data[y * texture->width + x] = color;
}

void draw_texture(Texture *texture, int x, int y)
{
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glBegin(GL_QUADS);
    // TODO: tint
    // glColor4ub(tint.r, tint.g, tint.b, tint.a);
    // TODO: scale
    glTexCoord2f(0.0f /** scale.x */+ x, 1.0f * /*scale.y */+ y);
    glVertex3f(-1.0f /*+ vSubPixelOffset.x*/, -1.0f /*+ vSubPixelOffset.y*/, 0.0f);
    glTexCoord2f(0.0f /** scale.x */+ x, 0.0f /** scale.y */+ y);
    glVertex3f(-1.0f /*+ vSubPixelOffset.x*/, 1.0f /*+ vSubPixelOffset.y*/, 0.0f);
    glTexCoord2f(1.0f /** scale.x */+ x, 0.0f /** scale.y */+ y);
    glVertex3f(1.0f /*+ vSubPixelOffset.x*/, 1.0f /*+ vSubPixelOffset.y*/, 0.0f);
    glTexCoord2f(1.0f /** scale.x */+ x, 1.0f /** scale.y */+ y);
    glVertex3f(1.0f /*+ vSubPixelOffset.x*/, -1.0f /*+ vSubPixelOffset.y*/, 0.0f);
    glEnd();
}

void clear_texture(Texture *texture, Color color) 
{
    for(int y = 0; y < texture->height; y++)
        for(int x = 0; x < texture->width; x++)
            texture->data[y * texture->width + x] = color;
}



void draw_pixel(int x, int y, Color color)
{
    draw_pixel_to_texture(layers[layer], x, y, color);
}

void draw_background(Color color)
{
    clear_texture(layers[layer], color);
}