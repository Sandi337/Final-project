#ifndef SEAGAMESCENE_H_INCLUDED
#define SEAGAMESCENE_H_INCLUDED
#include "scene.h"
#include "../algif5/algif.h"
#include "../element/element_label.h"
#include "../sea_element/charater_sea.h"
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <stdbool.h>
/*
   [game scene object]
*/
/*typedef enum EleType
{
    Character_L,
    Spirit_L
} EleType;*/
typedef struct _SEAScene
{
    ALLEGRO_BITMAP *background;
    //ALGIF_ANIMATION *gif_background;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    double timer;
} SEAScene;
Scene *New_SEAScene(int label, CharacterStatus *status);
void SEA_scene_update(Scene *self, float delta_time);
void SEA_scene_draw(Scene *self);
void SEA_scene_destroy(Scene *self);

#endif
