#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "pixa/pixa.h"
#include "GLFW/glfw3.h"

bool active;

Scene *scenes;
size_t scenes_c;

GLFWwindow* window;

void create_engine(int width, int height)
{   
    // TODO: Error checking
    // TODO: Logging

    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    window = glfwCreateWindow(width, height, "pxia", NULL, NULL);
    glfwMakeContextCurrent(window);

    scenes_c = 0;
    active = true;
}

void start_engine()
{
    while (active)
    {
        glfwPollEvents();

        for(int i = 0; i < scenes_c; i++)
            scenes[i].onUpdate();

        // TODO: layer drawing
        glfwSwapBuffers(window);

        if (scenes_c == 0)
            active = false;

        if (glfwWindowShouldClose(window))
            active = false;
    }

    glfwTerminate();
}

void create_scene(void (*onCreate)(), void (*onUpdate)(), void (*onDestroy)())
{
    Scene *new = malloc((scenes_c + 1) * sizeof(Scene));
    for (int i = 0; i < scenes_c; i++)
        new[i] = scenes[i];
    free(scenes);
    scenes = new;

    Scene scn = {onCreate, onUpdate, onDestroy};
    scenes[scenes_c] = scn;
    scenes[scenes_c++].onCreate();
}

Color create_color(int r, int g, int b, int a) 
{
    return (Color) {r / 255.0, g / 255.0, b / 255.0};
}

Texture *create_texture(int width, int height)
{
    Texture *tex = malloc(sizeof(Texture));
    tex->width = width;
    tex->height = height;
    tex->data = malloc(width * height * sizeof(Color));
    return tex;
}

// TODO: change this to layer lvl
void draw_background(Color c)
{
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT);
}