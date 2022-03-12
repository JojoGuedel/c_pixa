#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "GLFW/glfw3.h"
#include "Pixa/color.h"
#include "Pixa/core.h"
#include "Pixa/log.h"
#include "Pixa/sprite.h"

Sprite *sprite_create(int width, int height, bool filtered, bool clamp)
{
    Sprite *sprite = malloc(sizeof(Sprite));

    sprite->width = width;
    sprite->height = height;

    sprite->x = 0.0f;
    sprite->y = 0.0f;

    sprite->scale_x = 1.0f;
    sprite->scale_y = 1.0f;

    sprite->tint = COLOR_WHITE;

    sprite->data = malloc(width * height * sizeof(Color));

    glGenTextures(1, &sprite->id);
    glBindTexture(GL_TEXTURE_2D, sprite->id);

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

    // I don't know what this does so...
    // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    return sprite;
}

void sprite_destroy(Sprite *sprite)
{
    glDeleteTextures(1, &sprite->id);

    free(sprite->data);
}

Sprite *sprite_copy(Sprite *src)
{
    Sprite * dst = sprite_create(src->width, src->height, false, false);

    dst->scale_x = src->scale_x;
    dst->scale_y = src->scale_y;

    dst->tint = src->tint;

    memcpy(dst->data, src->data, src->width * src->height * sizeof(Color));

    return dst;
}

void sprite_copy_data(Sprite *dst, Sprite *src)
{
    memcpy(dst->data, src->data, src->width * src->height * sizeof(Color));
}


void sprite_update(Sprite *sprite)
{
    glBindTexture(GL_TEXTURE_2D, sprite->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sprite->width, sprite->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, sprite->data);
}

void sprite_draw(Sprite *sprite)
{
    float x1 = -1.0f + (float) sprite->x / (float) width  * 2;
    float y1 =  1.0f - (float) sprite->y / (float) height * 2;

    float x2 = -1.0f + ((float) (sprite->x + sprite->width)  / sprite->scale_x) / (float) width  * 2;
    float y2 =  1.0f - ((float) (sprite->y + sprite->height) / sprite->scale_y) / (float) height * 2;

    glBindTexture(GL_TEXTURE_2D, sprite->id);
    glBegin(GL_QUADS);

    glColor4ub(sprite->tint.r, sprite->tint.g, sprite->tint.b, sprite->tint.a);

    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(    x1,   y1, 0.0f);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(    x1,   y2, 0.0f);

    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(    x2,   y2, 0.0f);

    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(    x2,   y1, 0.0f);
    glEnd();
}

void sprite_clear(Sprite *sprite, Color color)
{
    for (int i = 0; i < sprite->width * sprite->height; i++)
        sprite->data[i] = color;
}


void sprite_draw_pixel(Sprite *sprite, int x, int y, Color color)
{
    if (x < 0 || y < 0 || x >= sprite->width || y >= sprite->height)
        return;

    sprite->data[y * sprite->width + x] = color;
}

void sprite_draw_line(Sprite *sprite, int x1, int y1, int x2, int y2, Color color)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    if (dx == 0)
    {
        if (x1 < 0 || x1 >= sprite->width)
            return;

        if (dy < 0)
        {
            int t = y1;
            y1 = y2;
            y2 = t;
        }

        if (y1 < 0)
            y1 = 0;

        for (int y = y1; y < y2 && y < sprite->height; y++)
            sprite->data[y * sprite->width + x1] = color;

        return;
    }
    if (dy == 0)
    {
        if (y1 < 0 || y1 >= sprite->height)
            return;

        if (dx < 0)
        {
            int t = x1;
            x1 = x2;
            x2 = t;
        }

        if (x1 < 0)
            x1 = 0;

        for (int x = x1; x < x2 && x < sprite->width; x++)
            sprite->data[y1 * sprite->width + x] = color;
        
        return;
    }

    if (abs(dx) > abs(dy))
    {
        for (int x = x1; x < x2; x++)
        {
            int y = y1 + dy * (x - x1) / dx;
            sprite_draw_pixel(sprite, x, y, color);
        }
    }
    else
    {
        for (int y = y1; y < y2; y++)
        {
            int x = x1 + dx * (y - y1) / dy;
            sprite_draw_pixel(sprite, x, y, color);
        }
    }

}

void sprite_draw_rect(Sprite *sprite, int x, int y, int w, int h, Color color)
{
    sprite_draw_line(sprite, x + 1, y    , x + w, y        , color);
    sprite_draw_line(sprite, x    , y + h, x + w, y + h    , color);
    sprite_draw_line(sprite, x    , y    , x    , y + h - 1, color);
    sprite_draw_line(sprite, x + w, y + 1, x + w, y + h + 1, color);
}

void sprite_fill_rect(Sprite *sprite, int x, int y, int w, int h, Color color)
{
    int x2 = x + w;
    int y2 = y + h;

    if (x > sprite->width)
        return;
    if (y > sprite->height)
        return;

    if (x < 0)
        x = 0;
    if (y < 0)
        y = 0;

    if (x2 > sprite->width)
        return;
    if (y2 > sprite->height)
        return;
    
    for(int _y = y; _y < y2 && _y < sprite->height; _y++)
        for(int _x = x; _x < x2 && _x < sprite->width; _x++)
            sprite->data[_y * sprite->width + _x] = color;
}

void sprite_set_pos(Sprite *sprite, int x, int y)
{
    sprite->x = x;
    sprite->y = y;
}

void sprite_set_scale(Sprite *sprite, float scale_x, float scale_y)
{
    sprite->scale_x = scale_x;
    sprite->scale_y = scale_y;
}