#include <stdlib.h>
#include <math.h>

#include "Pixa/color.h"

Color color_hsv(int h, int s, int v) {
    h = abs(h % 360);

    float _s = (float)s / 100.0f;
    float _v = (float)v / 100.0f;

    float c = _s * _v;
    float x = c * (1 - fabs(fmod(h / 60.0f, 2) - 1));
    float m = _v - c;


    if (h < 60)
        return (Color) {(c + m) * 255, (x + m) * 255, m * 255, 255};
    
    else if (h < 120)
        return (Color) {(x + m) * 255, (c + m) * 255, m * 255, 255};

    else if(h < 180)
        return (Color) {m * 255, (c + m) * 255, (x + m) * 255, 255};

    else if(h < 240)
        return (Color) {m * 255, (x + m) * 255, (c + m) * 255, 255};

    else if(h < 300)
        return (Color) {(x + m) * 255, m * 255, (c + m) * 255, 255};

    else
        return (Color) {(c + m) * 255, m * 255, (x + m) * 255, 255};
}