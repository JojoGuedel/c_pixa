#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#define LOG_LEVEL LOG_INFO

#include "Pixa/color.h"
#include "Pixa/core.h"
#include "Pixa/graphics.h"
#include "Pixa/log.h"
#include "Pixa/scene.h"

void on_update() {
  clear();
  // draw_line(get_width(), 1, get_width(), get_height());
  // draw_line(0, get_height() / 2, get_width(), get_height() / 2);

  // draw_line(0 + 1, 0, 0 + get_width(), 0);
  // draw_line(0, 0 + get_height(), 0 + get_width(), 0 + get_height());
  // draw_line(0, 0, 0, 0 + get_height() - 1);
  // draw_line(get_width(), 0 + 1, get_width(), 0 + get_height() + 1);

  // draw_rect(0, 0, 50, 50);
  draw_circle(50, 50, 20);

  draw_line(50, 50, 50 + 20 * cos(elapsed_time), 50 + 20 * sin(elapsed_time));
}

int main(int argc, const char *args[]) {
  // create engine
  engine_create(500, 500, 5, 5);
  // create scenes
  scene_create(NULL, on_update, NULL);
  // set clear color
  clear_color(COLOR_VERY_DARK_GREY);
  // start the engine
  engine_start();
}