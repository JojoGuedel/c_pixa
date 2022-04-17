#include "stddef.h"

#include "Pixa/camera.h"
#include "Pixa/layer.h"

Camera* camera_create(Layer* layer_target) {
    if (layer_target == NULL) {
        layer_create(0, NULL);
    }

    return NULL;
}

void camera_destroy(Camera* camera) {

}