#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Pixa/core.h"
#include "Pixa/globals.h"
#include "Pixa/internals.h"
#include "Pixa/layer.h"
#include "Pixa/scene.h"
#include "Pixa/texture.h"

bool active;

int width;
int height;

int resolution_x;
int resolution_y;

double elapsed_time;
double delta_time;

GLFWwindow* window;

void glfw_error_callback(int error, const char *desc)
{
    printf("[ERROR] glfw: %i %s\n", error, desc);
}

void gl_debug_callback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *user_data)
{
    printf("[DEBUG] gl: %s\n", msg);
}

void create_engine(int w, int h, int res_x, int res_y)
{
    // TODO: Error checking
    // TODO: Logging

    // init glfw
    glfwSetErrorCallback(glfw_error_callback);

    glfwInit();
    
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
    window = glfwCreateWindow(w, h, "Pixa", NULL, NULL);
    glfwMakeContextCurrent(window);

    // init glew
    if (glewInit() != GLEW_OK)
        printf("[ERROR]: failed to initialize glew.\n");
    
    glDebugMessageCallback(gl_debug_callback, NULL);
    
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_TEXTURE_2D);

    // init all globals
    active = true;

    width = w;
    height = h;

    delta_time = 0;
    elapsed_time = glfwGetTime();

    scene_c = 0;

    color_target = COLOR_WHITE;

    // init layer 0
    Texture *draw_target = create_texture(width / res_x, height / res_y, false, false);

    layer_draw_stack_count = 0;
    layer_target = layer_default = create_layer(0, draw_target);

    // TODO: move this to resize event
    glViewport(0, 0, width, height);
    // I don't know what this does yet so...
    // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void destroy_engine()
{
    for(int i = 0; i < scene_c; i++)
        scenes[i].onDestroy();
    free(scenes);

    for(int i = 0; i < layer_draw_stack_count; i++)
        free(layer_draw_stack[i]);
    free(layer_draw_stack);

    glfwTerminate();
}

void start_engine()
{
    // TODO: multithreading

    while (active)
    {
        delta_time = glfwGetTime() - elapsed_time;
        elapsed_time = glfwGetTime();

        glfwPollEvents();
        // TODO: handle events

        for(int i = 0; i < scene_c; i++)
        {
            if (!scenes[i].is_active)
                continue;

            scenes[i].onUpdate();
        }

        // TODO: move this to prepare_drawing()
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for(int i = 0; i < layer_draw_stack_count; i++)
        {
            update_texture(layer_draw_stack[i]->draw_target);
            draw_texture(layer_draw_stack[i]->draw_target, 0, 0);
        }

        glfwSwapBuffers(window);

        if (scene_c == 0)
            active = false;

        if (glfwWindowShouldClose(window))
            active = false;
    }

    glfwTerminate();
}

void stop_engine()
{
    active = false;
}