#ifndef PXIA_H
#define PXIA_H

#include <stdint.h>
#include <stdbool.h>

// TODO: make descriptions for everything

#define COLOR_GREY                  (Color) {192, 192, 192, 255}
#define COLOR_DARK_GREY             (Color) {128, 128, 128, 255}
#define COLOR_VERY_DARK_GREY        (Color) { 64,  64,  64, 255}
#define COLOR_RED                   (Color) {255,   0,   0, 255}
#define COLOR_DARK_RED              (Color) {128,   0,   0, 255}
#define COLOR_VERY_DARK_RED         (Color) {64,    0,   0, 255}
#define COLOR_YELLOW                (Color) {255, 255,   0, 255}
#define COLOR_DARK_YELLOW           (Color) {128, 128,   0, 255}
#define COLOR_VERY_DARK_YELLOW      (Color) { 64,  64,   0, 255}
#define COLOR_GREEN                 (Color) {  0, 255,   0, 255}
#define COLOR_DARK_GREEN            (Color) {  0, 128,   0, 255}
#define COLOR_VERY_DARK_GREEN       (Color) {  0,  64,   0, 255}
#define COLOR_CYAN                  (Color) {  0, 255, 255, 255}
#define COLOR_DARK_CYAN             (Color) {  0, 128, 128, 255}
#define COLOR_VERY_DARK_CYAN        (Color) {  0,  64,  64, 255}
#define COLOR_BLUE                  (Color) {  0,   0, 255, 255}
#define COLOR_DARK_BLUE             (Color) {  0,   0, 128, 255}
#define COLOR_VERY_DARK_BLUE        (Color) {  0,   0,  64, 255}
#define COLOR_MAGENTA               (Color) {255,   0, 255, 255}
#define COLOR_DARK_MAGENTA          (Color) {128,   0, 128, 255}
#define COLOR_VERY_DARK_MAGENTA     (Color) { 64,   0,  64, 255}
#define COLOR_WHITE                 (Color) {255, 255, 255, 255}
#define COLOR_BLACK                 (Color) {  0,   0,   0, 255}
#define COLOR_BLANK                 (Color) {  0,   0,   0,   0}

typedef struct
{
    // TODO: tint
    int layer;
    bool is_active;

    void (*onCreate)();
    void (*onUpdate)();
    void (*onDestroy)();
} Scene;

typedef struct
{
    uint8_t r;
    uint8_t g; 
    uint8_t b;
    uint8_t a;
} Color;

typedef struct
{
    int width;
    int height;

    float scale_x;
    float scale_y;

    unsigned int id;

    Color *data;
} Texture;

// there is no use for textures for the user currently
// Texture create_texture(int width, int height, bool filtered, bool clamp);
// void update_texture(Texture texture);
// void draw_pixel_to_texture (Texture texture, int x, int y, Color color);
// void draw_line_to_texture(Texture texture, int x1, int y1, int x2, int y2, Color color);
// void draw_texture(Texture texture, int x, int y);
// void clear_texture(Texture texture, Color color);

void create_engine(int screen_width, int screen_height, int res_x, int res_y);
void start_engine();
void destroy_engine();
void exit_engine();

int create_scene(void (*onCreate)(), void (*onUpdate)(), void (*onDestroy)());
void destroy_scene(int scene);

void create_layers(int count);
void destroy_layers(int count);

void set_scene_active(int scene, bool active);
bool set_scene_layer(int scene, int layer);
bool set_layer(int layer);
void set_color(Color color);
void set_clear_color(Color color);

int get_width();
int get_height();
double get_elapsed_time();
double get_delta_time();
float get_fps();

void draw_pixel(int x, int y);
// void draw_line(int x1, int y1, int x2, int y2);
void clear_layer();

#endif
