#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include "Pixa/core.h"
#include "Pixa/layer.h"
#include "Pixa/scene.h"
#include "Pixa/texture.h"

static bool active;

// TODO: remove window_width and window_height so that only widht and hight exists
static int window_width;
static int window_height;

static int world_width;
static int world_height;

static int res_x;
static int res_y;

// TODO: make resolution scaleable
static double elapsed_time;
static double delta_time;


static Scene *scenes;
static size_t scene_c;

static Color target_color;
static Texture *target_clear_color;

// static int target_layer;
// static Texture **layers;
// static size_t layer_c;

Layer *layers;
size_t layer_count;
// TODO: make a layer_clear_color array

int layer_target;
int *layer_draw_stack;
size_t layer_draw_stack_count;


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

#pragma region engine
// TODO: refactor and integrate with params.h
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

    delta_time = 0;
    elapsed_time = glfwGetTime();

    scene_c = 0;

    target_color = COLOR_WHITE;

    target_clear_color = create_texture(world_width, world_height, false, false);
    // set_clear_color(COLOR_BLACK);

    // init layer 0
    Texture *draw_target = create_texture(w_w / r_x, w_h / r_y, false, false);
    draw_target->scale_x = 1.0f / r_x;
    draw_target->scale_y = 1.0f / r_y;

    layer_count = 1;
    layers = malloc(sizeof(Layer));
    layers[0].layer = 0;
    layers[0].texture = draw_target;

    layer_draw_stack_count = 1;
    layer_draw_stack = malloc(sizeof(int));
    layer_draw_stack[0] = LAYER_DEFAULT;

    bind_layer(0);

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

    free(layers);
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
            update_texture(layers[layer_draw_stack[i]].texture);
            draw_texture(layers[layer_draw_stack[i]].texture, 0, 0);
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

#pragma region layers
int create_layer(int layer, Texture *render_target)
{
    int insert = -1;

    for (int i = 0; i < scene_c; i++)
        if (layers[i].texture == NULL)
        {
            insert =  i;
            break;
        }

    if (insert == -1)
    {
        Layer *temp = malloc((layer_count + 1) * sizeof(Layer));
        memcpy(temp, layers, layer_count * sizeof(Layer));
        free(layers);

        layers = temp;

        insert = layer_count;
    }

    // create the new layer
    layers[insert].layer = layer;

    if (render_target == NULL)
        layers[insert].texture = create_texture(world_width, world_height, false, false);
    else
        layers[insert].texture = render_target;

    layers[insert].clear_texture = copy_texture(layers[insert].texture);
    clear_texture(layers[insert].clear_texture, COLOR_BLACK);
    
    // insert new layer at the right location in the layer_draw_stack
    int *temp = malloc((layer_draw_stack_count + 1) * sizeof(int));

    for(int offset = 0; offset < layer_draw_stack_count + 1; offset++)
        if (offset == layer_count || layers[layer_draw_stack[offset]].layer > layer)
            {
                memcpy(temp, layer_draw_stack, offset * sizeof(int));
                temp[offset] = insert;
                memcpy(&temp[offset + 1], &layer_draw_stack[offset], (layer_draw_stack_count - offset) * sizeof(int));

                free(layer_draw_stack);
                layer_draw_stack = temp;
                layer_draw_stack_count++;
                
                break;
            }
    
    // TODO: test this because I have idea if this even works

    return layer_count++;
}

bool destroy_layer(int id)
{
    if (id == 0)
        // LOG: removing layer 0 is illegal
        return false;

    for(int i = 0; i < layer_count; i++)
    {
        if (layer_draw_stack[i] == id)
        {
            int *temp = malloc((layer_count - 1) * sizeof(int));

            memcpy(temp, layer_draw_stack, i * sizeof(int));
            // TODO: check if it needs to be 1 or 2
            memcpy(&temp[i], &layer_draw_stack[i + 1], (layer_draw_stack_count - i - 1) * sizeof(int));

            free(layer_draw_stack);
            layer_draw_stack = temp;
            layer_draw_stack_count--;

            layers[id].layer = 0;
            layers[id].texture = NULL;

            // TODO: deallocate if removed at the end

            return true;
        }
    }

    // TODO: test this because I have idea if this even works
    // LOG: layer was not found
    return false;
}

void clear_color(Color color)
{
    clear_texture(layers[layer_target].clear_texture, color);   
}

void clear(int id)
{
    memcpy(layers[layer_target].texture->data,
           layers[layer_target].clear_texture->data,
           layers[layer_target].clear_texture->width * layers[layer_target].clear_texture->height * sizeof(Color));
}

#pragma endregion

void bind_layer(int id)
{
    if (id >= 0 && id < layer_count)
        layer_target = id;
}




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