#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#include "Pixa/core.h"
#include "Pixa/internals.h"
#include "Pixa/layer.h"
#include "Pixa/scene.h"
#include "Pixa/sprite.h"

bool active;

int width;
int height;

double mouse_x;
double mouse_y;

void (*on_key_pressed_callback)(int key, int action, int flags);
void (*on_mouse_pressed_callback)(int button, int action, int flags);

double elapsed_time;
double delta_time;

GLFWwindow* window;

void gl_debug_callback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *user_data)
{
    printf("[DEBUG] gl: %s\n", msg);
}

void glfw_error_callback(int error, const char *desc)
{
    printf("[ERROR] glfw: %i %s\n", error, desc);
}

void glfw_key_callback(GLFWwindow* window, int key, int scancode, int action, int flags)
{
    if (on_key_pressed_callback != NULL)
        on_key_pressed_callback(key, action, flags);
}

void glfw_mouse_callback(GLFWwindow* window, int button, int action, int flags)
{
    if (on_mouse_pressed_callback != NULL)
        on_mouse_pressed_callback(button, action, flags);
}

void engine_create(int w, int h, int res_x, int res_y)
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

    glfwGetCursorPos(window,  &mouse_x, &mouse_y);

    on_key_pressed_callback = NULL;
    on_mouse_pressed_callback = NULL;

    glfwSetKeyCallback(window, glfw_key_callback);
    glfwSetMouseButtonCallback(window, glfw_mouse_callback);

    delta_time = 0;
    elapsed_time = glfwGetTime();

    scene_c = 0;

    color_target = COLOR_WHITE;

    // init layer 0
    Sprite *draw_target = sprite_create(width / res_x, height / res_y, false, false);
    sprite_set_scale(draw_target, 1.0f / (float) res_x, 1.0f / (float) res_y);

    layer_draw_stack_count = 0;
    layer_target = layer_default = layer_create(0, draw_target);

    // TODO: move this to resize event
    glViewport(0, 0, width, height);
    // I don't know what this does yet so...
    // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void engine_destroy()
{
    for (int i = 0; i < scene_c; i++)
        scenes[i].on_destroy();
    free(scenes);

    for (int i = 0; i < layer_draw_stack_count; i++)
        free(layer_draw_stack[i]);
    free(layer_draw_stack);

    glfwTerminate();
}

void engine_start()
{
    while (active)
    {
        delta_time = glfwGetTime() - elapsed_time;
        elapsed_time = glfwGetTime();

        glfwPollEvents();
        glfwGetCursorPos(window,  &mouse_x, &mouse_y);

        for (int i = 0; i < scene_c; i++)
        {
            if (!scenes[i].is_active)
                continue;

            scenes[i].on_update();
        }

        // TODO: move this to prepare_drawing()
        glEnable(GL_BLEND);
        glActiveTexture(GL_TEXTURE0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for (int i = 0; i < layer_draw_stack_count; i++)
        {
            sprite_update(layer_draw_stack[i]->draw_target);
            sprite_draw(layer_draw_stack[i]->draw_target);
        }

        glfwSwapBuffers(window);

        if (scene_c == 0)
            active = false;

        if (glfwWindowShouldClose(window))
            active = false;
    }

    glfwTerminate();
}

void engine_stop()
{
    active = false;
}

void engine_set_user_input(void (*on_key_pressed)(int key, int action, int flags), void (*on_mouse_pressed)(int button, int action, int flags))
{
    on_key_pressed_callback = on_key_pressed;
    on_mouse_pressed_callback = on_mouse_pressed;
}

int get_width()
{
    return layer_target->draw_target->width;
}

int get_height()
{
    return layer_target->draw_target->height;
}