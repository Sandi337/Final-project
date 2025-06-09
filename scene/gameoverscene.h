#ifndef GAMEOVERSCENE_H
#define GAMEOVERSCENE_H

#include "../scene/scene.h"
#include <allegro5/allegro_font.h>

typedef struct _GameOverScene 
{
    ALLEGRO_BITMAP *image;
    ALLEGRO_FONT *font;
    CharacterStatus *status;
} GameOverScene;

Scene* New_GameOverScene(int label, CharacterStatus *status);
void gameover_scene_update(Scene *self, float delta_time);
void gameover_scene_draw(Scene *self);
void gameover_scene_destroy(Scene *self);

#endif