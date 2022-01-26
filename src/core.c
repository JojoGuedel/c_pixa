#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Pixa/core.h"
#include "Pixa/scene.h"
#include "Pixa/texture.h"


static bool active;

static int window_width;
static int window_height;

static int world_width;
static int world_height;

static int res_x;
static int res_y;

// TODO: make resolution scaleable

static Scene *scenes;
static size_t scene_c;

static Color target_color;
static Texture *target_clear_color;

static int target_layer;
static Texture **layers;
static size_t layer_c;

static double elapsed_time;
static double delta_time;

// TODO: implement textures for gpu (olc::Decal)

static GLFWwindow* window;


void glfw_error_callback(int error, const char *desc)
{
    printf("[ERROR] glfw: %i %s\n", error, desc);
}

void gl_debug_callback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *user_data)
{
    printf("[DEBUG] gl: %s\n", msg);
}

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

    window_width = w_w;
    window_height = w_h;

    world_width = w_w / r_x; 
    world_height = w_h / r_y;

    res_x = r_x;
    res_y = r_y;

    scene_c = 0;

    target_color = COLOR_WHITE;

    target_clear_color = create_texture(world_width, world_height, false, false);
    set_clear_color(COLOR_BLACK);

    target_layer = 0;
    layer_c = 0;
    create_layers(1);

    delta_time = 0;
    elapsed_time = glfwGetTime();

    // TODO: move this to resize event
    glViewport(0, 0, window_width, window_height);
    // I don't know what this does yet so...
    // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void destroy_engine()
{
    for(int i = 0; i < scene_c; i++)
        scenes[i].onDestroy();
    free(scenes);

    for(int i = 0; i < layer_c; i++)
        free(layers[i]->data);
    free(layers);

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

            if (!set_layer(scenes[i].layer))
                set_layer(layer_c - 1);

            scenes[i].onUpdate();
        }

        // TODO: move this to prepare_drawing()
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for(int i = 0; i < layer_c; i++)
        {
            update_texture(layers[i]);
            draw_texture(layers[i], 0, 0);
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



int create_scene(void (*onCreate)(), void (*onUpdate)(), void (*onDestroy)())
{
    for (int i = 0; i < scene_c; i++)
    {
        if (scenes[i].onCreate  == NULL &&
            scenes[i].onUpdate  == NULL &&
            scenes[i].onDestroy == NULL)
        {
            scenes[scene_c].layer          = 0;
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

    scenes[scene_c].layer          = 0;
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

    scenes[id].layer                = 0;
    scenes[id].is_active            = false;
    scenes[id].onCreate             = NULL;
    scenes[id].onUpdate             = NULL;
    scenes[id].onDestroy            = NULL;

    // TODO: deallocate
}

void create_layers(int count)
{
    Texture **temp = malloc((layer_c + count) * sizeof(Texture *));
    memcpy(temp, layers, layer_c * sizeof(Texture *));

    free(layers);

    layers = temp;

    for (int i = 0; i < count; i++)
        layers[layer_c + i] = create_texture(world_width, world_height, false, false);

    layer_c += count;
}

void destroy_layers(int count)
{
    if (count >= layer_c)
        count = layer_c - 1;

    Texture **temp = malloc((layer_c - count) * sizeof(Texture *));
    memcpy(temp, layers, (layer_c - count) * sizeof(Texture *));

    free(layers);

    layers = temp;
    layer_c -= count;
}

void set_scene_active(int scene, bool active)
{
    if (scene < layer_c)
        scenes[scene].is_active = active;
}

bool set_scene_layer(int scene, int layer)
{
    if (layer >= layer_c || layer < 0)
        return false;

    scenes[scene].layer = layer;
    return true;
}

bool set_layer(int layer)
{
    if (layer >= layer_c || layer < 0)
        return false;

    target_layer = layer;
    return true;
}

void set_color(Color color) 
{
    target_color = color;
}

void set_clear_color(Color color) 
{
    clear_texture(target_clear_color, color);
}

void set_title(const char *title)
{
    glfwSetWindowTitle(window, title);
}

void draw_pixel(int x, int y)
{
    draw_pixel_to_texture(layers[target_layer], x, y, target_color);
}

// void draw_line(int x1, int y1, int x2, int y2)
// {
//     draw_line_to_texture(layers[target_layer], x1, y1, x2, y2, target_color);
// }

void clear_layer()
{
    memcpy(layers[target_layer]->data, target_clear_color->data, target_clear_color->width * target_clear_color->height * sizeof(Color));
}