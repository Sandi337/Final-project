#ifndef gamefinish_H_INCLUDED
#define gamefinish_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include "scene.h"
/*
   [gamefinish object]
*/
typedef struct _gamefinish
{
    ALLEGRO_FONT *font;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;
    int title_x, title_y;

    int final_score;
    bool music_played;
} gamefinish;
Scene *New_gamefinish(int label);
void gamefinish_update(Scene *self, float delta_time);
void gamefinish_draw(Scene *self);
void gamefinish_destroy(Scene *self);

#endif

