#ifndef PIXA_SCENE_H
#define PIXA_SCENE_H

typedef struct
{
    bool is_active;

    void (*onCreate)();
    void (*onUpdate)();
    void (*onDestroy)();
} Scene;

/*! @brief Create a scene.
 *  @param[in] onCreate The callback that is called when the scene is created, or `NULL` if no callback is needed
 *  @param[in] onUpdate The callback that is called each frame
 *  @param[in] onDestroy The callback that is called when the scene is destroyed, or `NULL` if no callback is needed
 *  @return The ID of the scene
 */
int scene_create(void (*onCreate)(), void (*onUpdate)(), void (*onDestroy)());
/*! @brief Destory a scene.
 *  @param[in] id The ID of the new scene.
 */
void scene_destroy(int id);

#endif