#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "GLFW/glfw3.h"
#include "Pixa/color.h"
#include "Pixa/texture.h"

Texture *create_texture(int width, int height, bool filtered, bool clamp)
{
    Texture *texture = malloc(sizeof(Texture));

    texture->width = width;
    texture->height = height;

    texture->scale_x = 1.0f;
    texture->scale_y = 1.0f;

    texture->tint = COLOR_WHITE;

    texture->data = malloc(width * height * sizeof(Color));

    glGenTextures(1, &texture->id);
    glBindTexture(GL_TEXTURE_2D, texture->id);

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
    return texture;
}

void destroy_texture(Texture *texture)
{
    glDeleteTextures(1, &texture->id);

    free(texture->data);
}

Texture *copy_texture(Texture *src)
{
    Texture * dst = create_texture(src->width, src->height, false, false);

    dst->scale_x = src->scale_x;
    dst->scale_y = src->scale_y;

    dst->tint = src->tint;

    memcpy(dst->data, src->data, src->width * src->height * sizeof(Color));

    return dst;
}

void copy_texture_data(Texture *dst, Texture *src)
{
    memcpy(dst->data, src->data, src->width * src->height * sizeof(Color));
}

void update_texture(Texture *texture)
{
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture->data);
}

// TODO: maybe rename texture to sprite and store x and y in the sprite
void draw_texture(Texture *texture, int x, int y)
{
    glBindTexture(GL_TEXTURE_2D, texture->id);
    glBegin(GL_QUADS);
    // glColor4ub(255, 255, 255, 255);
    glTexCoord2f(0.0f * texture->scale_x + x, 0.0f * texture->scale_y + y);
    glVertex3f(-1.0f, 1.0f, 0.0f);

    glTexCoord2f(0.0f * texture->scale_x + x, 1.0f * texture->scale_y + y);
    glVertex3f(-1.0f, -1.0f , 0.0f);

    glTexCoord2f(1.0f * texture->scale_x + x, 1.0f * texture->scale_y + y);
    glVertex3f(1.0f , -1.0f, 0.0f);

    glTexCoord2f(1.0f * texture->scale_x + x, 0.0f * texture->scale_y + y);
    glVertex3f(1.0f, 1.0f, 0.0f);
    glEnd();
}

void draw_pixel_to_texture(Texture *texture, int x, int y, Color color)
{
    if (x < 0 || y < 0 || x >= texture->width || y >= texture->height)
        return;

    texture->data[y * texture->width + x] = color;
}

// void draw_line_to_texture(Texture texture, int x1, int y1, int x2, int y2, Color color)
// {
//     int dx = x2 - x1;
//     int dy = y2 - y1;

//     if (dy == 0)
//     {
//         if (dx > 0)
//         {
//             for(int x = x1; x < x2; x++)
//                 draw_pixel_to_texture(texture, x, y1, color);
//         }
//     }

//     if (dx == 0)
//     {
//     }
// }

void clear_texture(Texture *texture, Color color)
{
    for(int i = 0; i < texture->width * texture->height; i++)
        texture->data[i] = color;
}