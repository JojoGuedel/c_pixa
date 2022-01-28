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

Color target_color;

GLFWwindow* window;

void glfw_error_callback(int error, const char *desc)
{
    printf("[ERROR] glfw: %i %s\n", error, desc);
}

void gl_debug_callback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *user_data)
{
    printf("[DEBUG] gl: %s\n", msg);
}

#pragma region engine
void create_engine(int w_w, int w_h, int r_x, int r_y)
{
    // TODO: Error checking
    // TODO: Logging

    // init glfw
    glfwSetErrorCallback(glfw_error_callback);

    glfwInit();
    
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    
    window = glfwCreateWindow(w_w, w_h, "Pixa", NULL, NULL);
    glfwMakeContextCurrent(window);

    // init glew
    if (glewInit() != GLEW_OK)
        printf("[ERROR]: failed to initialize glew.\n");
    
    glDebugMessageCallback(gl_debug_callback, NULL);
    
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_TEXTURE_2D);

    // init all globals
    active = true;

    width = w_w / r_x;
    height = w_h / r_y;

    resolution_x = r_x;
    resolution_y = r_y;

    delta_time = 0;
    elapsed_time = glfwGetTime();

    scene_c = 0;

    target_color = COLOR_WHITE;

    // init layer 0
    Texture *draw_target = create_texture(width / resolution_x, height / resolution_y, false, false);
    draw_target->scale_x = 1.0f / r_x;
    draw_target->scale_y = 1.0f / r_y;

    layer_draw_stack_count = 0;
    layer_target = layer_default = create_layer(0, draw_target);

    // bind_layer(0);

    // TODO: move this to resize event
    glViewport(0, 0, width * resolution_x, width * resolution_x);
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
#pragma endregion


// void create_layers(int count)
// {
//     Texture **temp = malloc((layer_c + count) * sizeof(Texture *));
//     memcpy(temp, layers, layer_c * sizeof(Texture *));

//     free(layers);

//     layers = temp;

//     for (int i = 0; i < count; i++)
//         layers[layer_c + i] = create_texture(world_width, world_height, false, false);

//     layer_c += count;
// }

// void destroy_layers(int count)
// {
//     if (count >= layer_c)
//         count = layer_c - 1;

//     Texture **temp = malloc((layer_c - count) * sizeof(Texture *));
//     memcpy(temp, layers, (layer_c - count) * sizeof(Texture *));

//     free(layers);

//     layers = temp;
//     layer_c -= count;
// }

void set_scene_active(int scene, bool active)
{
    if (scene >= 0 && scene < scene_c)
        scenes[scene].is_active = active;
}

// bool set_layer(int layer)
// {
//     if (layer >= layer_c || layer < 0)
//         return false;

//     target_layer = layer;
//     return true;
// }

// void set_color(Color color) 
// {
//     target_color = color;
// }

// void set_clear_color(Color color) 
// {
//     clear_texture(target_clear_color, color);
// }

// void set_title(const char *title)
// {
//     glfwSetWindowTitle(window, title);
// }

// void draw_pixel(int x, int y)
// {
//     draw_pixel_to_texture(layers[layer_target].texture, x, y, target_color);
// }

// void draw_line(int x1, int y1, int x2, int y2)
// {
//     draw_line_to_texture(layers[target_layer], x1, y1, x2, y2, target_color);
// }

// void clear_layer()
// {
//     clear_texture(layers[layer_target].texture, COLOR_BLACK);
//     // memcpy(layers[target_layer]->data, target_clear_color->data, target_clear_color->width * target_clear_color->height * sizeof(Color));
// }