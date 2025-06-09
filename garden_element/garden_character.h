#ifndef GARDEN_CHARATER_H_INCLUDED
#define GARDEN_CHARATER_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include "../element/element.h"
#include "../element/element_label.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"
#include "../global.h"
#include <stdbool.h>

/*
   [character object]
*/
typedef enum CharacterType_Garden
{
    GARDEN_STOP  = 0,
    UP,
    DOWN,
    LEFT,
    RIGHT,
} CharacterType_Garden;
typedef struct _Character_Garden
{
    int x, y;
    int width, height;              // the width and height of image
    bool dir;                       // true: face to right, false: face to left
    CharacterType_Garden state;                      // the state of character
    ALGIF_ANIMATION *gif_status[5]; // gif for each state. 0: stop, 1: move, 2:attack
    ALLEGRO_SAMPLE_INSTANCE *atk_Sound;
    int health;
    int energy;
    int spirit;
    CharacterStatus *status;
    int anime;      // counting the time of animation
    int anime_time; // indicate how long the animation
    bool new_proj;
    Shape *hitbox; // the hitbox of object
} Character_Garden;
Elements *New_Character_Garden(int label, CharacterStatus *status);
void Character_Garden_update(Elements *self, float delta_time);
void Character_Garden_interact(Elements *self);
void Character_Garden_draw(Elements *self);
void Character_Garden_destroy(Elements *self);
void _Character_Garden_update_position(Elements *self, int dx, int dy);

#endif
