#ifndef GAMECLEARSCENE_H
#define GAMECLEARSCENE_H

#include "../scene/scene.h"
#include <allegro5/allegro_font.h>

typedef struct _GameclearScene
{
    ALLEGRO_BITMAP *image;
    ALLEGRO_FONT *font;
    CharacterStatus *status;
} GameclearScene;

Scene* New_GameclearScene(int label, CharacterStatus *status);
void GameclearScene_update(Scene *self, float delta_time);
void GameclearScene_draw(Scene *self);
void GameclearScene_destroy(Scene *self);

#endif