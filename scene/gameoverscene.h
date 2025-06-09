#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include "../scene/scene.h"

Scene* New_GameOverScene(int label);
void gameover_scene_update(Scene *self, float delta_time);
void gameover_scene_draw(Scene *self);
void gameover_scene_destroy(Scene *self);

#endif