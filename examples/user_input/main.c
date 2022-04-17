#include <math.h>
#include <stddef.h>

#define LOG_LEVEL LOG_INFO

#include "Pixa/color.h"
#include "Pixa/core.h"
#include "Pixa/graphics.h"
#include "Pixa/log.h"
#include "Pixa/scene.h"

void key_cb(int key, int action, int flags) {
    log_info("%c: %i", key, action);
}

void mouse_cb(int button, int action, int flags) {
    log_info("%c: %i", button, action);
}

void on_update() {
    clear();
}

int main(int argc, const char *args[]) {
    // create engine
    engine_create(500, 500, 1, 1);
    // set keyboard and mouse callback
    engine_set_user_input(key_cb, mouse_cb);
    // create scenes
    scene_create(NULL, on_update, NULL);
    // set clear color
    clear_color(COLOR_VERY_DARK_GREY); 
    // start the engine
    engine_start();
}