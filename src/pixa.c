#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "pixa/pixa.h"
#include "GL/glew.h"
#include "GLFW/glfw3.h"

static bool active;

static int width;
static int height;

// TODO: make resolution scaleable

static Scene *scenes;
static size_t scenes_c;

static int target_layer;
// TODO: make a struct layers for overview
static Texture *layers;
static size_t layers_c;

static double elapsed_time;
static double delta_time;

// TODO: implement textures for gpu (olc::Decal)

static GLFWwindow* window;

Texture create_texture(int width, int height, bool filtered, bool clamp)
{
    Texture tex;

    tex.width = width;
    tex.height = height;
    tex.data = malloc(width * height * sizeof(Color));

    glGenTextures(1, &tex.id);
    glBindTexture(GL_TEXTURE_2D, tex.id);

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

void update_texture(Texture texture)
{
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture.data);
}

void draw_texture(Texture texture, int x, int y)
{
    glBindTexture(GL_TEXTURE_2D, texture.id);
    glBegin(GL_QUADS);
    // TODO: tint
    // glColor4ub(255, 255, 255, 255);
    // TODO: scale
    glTexCoord2f(0.0f /** scale.x */+ x, 0.0f /** scale.y */+ y);
    glVertex3f(-1.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f /** scale.x */+ x, 1.0f * /*scale.y */+ y);
    glVertex3f(-1.0f, -1.0f , 0.0f);

    glTexCoord2f(1.0f /** scale.x */+ x, 1.0f /** scale.y */+ y);
    glVertex3f(1.0f , -1.0f, 0.0f);

    glTexCoord2f(1.0f /** scale.x */+ x, 0.0f /** scale.y */+ y);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glEnd();
}

void draw_pixel_to_texture(Texture texture, int x, int y, Color color)
{
    if (x < 0 || y < 0 || x >= texture.width || y >= texture.height)
        return;
    
    texture.data[y * texture.width + x] = color;
}

void draw_line_to_texture(Texture texture, int x1, int y1, int x2, int y2, Color color) 
{
    if (y1 == y2)
    {
        int tx = x1 < x2? x1 : x2;
        for(int x = 0; x < tx && x < texture.width; x++)
            texture.data[y1 * texture.width + x] = color;
        return;
    }
    if (y1 == y2)
    {
        int ty = y1 < y2? y1 : y2;
        for(int y = 0; y < ty && y < texture.height; y++)
            texture.data[y * texture.width + x1] = color;
        return;
    }
}

void clear_texture(Texture texture, Color color) 
{
    for(int i = 0; i < texture.width * texture.height; i++)
        texture.data[i] = color;
}

void glfw_error_callback(int error, const char *desc)
{
    printf("[ERROR] glfw: %i %s\n", error, desc);
}

void gl_debug_callback(GLenum src, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *msg, const void *user_data)
{
    printf("[DEBUG] gl: %s\n", msg);
}

void create_engine(int w, int h)
{   
    // TODO: Error checking
    // TODO: Logging
    glfwSetErrorCallback(glfw_error_callback);

    glfwInit();
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DOUBLEBUFFER, GLFW_TRUE);
    window = glfwCreateWindow(w, h, "pixa", NULL, NULL);
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
        printf("[ERROR]: failed to initialize glew.\n");
    glEnable(GL_DEBUG_OUTPUT);
    glDebugMessageCallback(gl_debug_callback, NULL);
    glEnable(GL_TEXTURE_2D);
    // I don't know what this does yet so...
    // glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    // glViewport(0, 0, width, height);

    active = true;

    width = w;
    height = h;

    scenes_c = 0;

    target_layer = 0;
    layers_c = 0;
    create_layers(1);

    delta_time = 0;
    elapsed_time = glfwGetTime();
}

void destroy_engine()
{
    for(int i = 0; i < scenes_c; i++)
        scenes[i].onDestroy();
    free(scenes);

    for(int i = 0; i < layers_c; i++)
        free(layers[i].data);
    free(layers);

    glfwTerminate();
}

void exit_engine()
{
    active = false;
}

void start_engine()
{
    // TODO: multithreading
    
    while (active)
    {
        delta_time = glfwGetTime() - elapsed_time;
        elapsed_time = glfwGetTime();

        glfwPollEvents();

        for(int i = 0; i < scenes_c; i++)
        {
            if (!scenes[i].is_active)
                continue;

            if (!set_layer(scenes[i].layer))
                set_layer(layers_c - 1); 

            scenes[i].onUpdate();
        }

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        for(int i = 0; i < layers_c; i++)
        {
            update_texture(layers[i]);
            draw_texture(layers[i], 0, 0);
        }

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

    // TODO: deallocate
}

void create_layers(int count)
{
    Texture *temp = malloc((layers_c + count) * sizeof(Texture));
    memcpy(temp, layers, layers_c * sizeof(Texture));

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

    Texture *temp = malloc((layers_c - count) * sizeof(Texture));
    memcpy(temp, layers, (layers_c - count) * sizeof(Texture));

    free(layers);

    layers = temp;
    layers_c -= count;
}

void set_scene_active(int scene, bool active)
{
    if (scene < layers_c)
        scenes[scene].is_active = active;
}

bool set_scene_layer(int scene, int layer) 
{
    if (layer >= layers_c || layer < 0)
        return false;
    
    scenes[scene].layer = layer;
    return true;
}

bool set_layer(int layer)
{
    if (layer >= layers_c || layer < 0)
        return false;

    target_layer = layer;
    return true;
}

double get_elapsed_time()
{
    return elapsed_time;
}

double get_delta_time()
{
    return delta_time;
}

double get_fps()
{
    return 1.0 / delta_time;
}

void draw_pixel(int x, int y, Color color)
{
    draw_pixel_to_texture(layers[target_layer], x, y, color);
}

void draw_line(int x1, int y1, int x2, int y2, Color color)
{
    draw_line_to_texture(layers[target_layer], x1, y1, x2, y2, color);
}

void clear_layer(Color color)
{
    clear_texture(layers[target_layer], color);
}