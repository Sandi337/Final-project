#ifndef CHARATER_H_INCLUDED
#define CHARATER_H_INCLUDED
#include <allegro5/allegro_audio.h>
#include "../element/element.h"
#include "../element/element_label.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"
#include <stdbool.h>

/*
   [character object]
*/
typedef struct _Character_SEA
{
    int x, y;
    int width, height;              // the width and height of image
    bool dir;                       // true: face to right, false: face to left
    ALGIF_ANIMATION *gif_Ichi;
    int anime;      // counting the time of animation
    int anime_time; // indicate how long the animation
    bool new_proj;

    float angle;
    float balance_speed;
    float random_force; // 隨機力
    double last_force_angle_time; // 上次改隨機力的時間
    double force_angle_time_interval;  // 每隔多久要改隨機

    // 精神值
    int spirit;
} Character_SEA;
Elements *New_Character_SEA(int label, CharacterStatus *status);
void Character_SEA_update(Elements *self, float delta_time);
void Character_SEA_interact(Elements *self);
void Character_SEA_draw(Elements *self);
void Character_SEA_destory(Elements *self);
void _Character_SEA_update_position(Elements *self, int dx, int dy);

#endif
