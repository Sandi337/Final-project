#ifndef MUSHROOM_H_INCLUDED
#define MUSHROOM_H_INCLUDED

#include "element.h"
#include "../shapes/Shape.h"
#include "../algif5/algif.h"
#include <stdbool.h>

/*
   [MUSHROOM object]
*/
#define MUSHROOM_L 5

typedef struct _Mushroom{
    ALLEGRO_BITMAP *img;
    int x, y;
    int width, height;
    int type; // 0: 綠, 1: 紅, 2: 藍, 3: 彩虹
    Shape *hitbox;
    bool active;
} Mushroom;

Elements *New_Mushroom(int label);
void Mushroom_update(Elements *self);
void Mushroom_interact(Elements *self);
void _Mushroom_interact_Projectile(Elements *self, Elements *tar);
void _Mushroom_interact_Character(Elements *self, Elements *tar);
void Mushroom_draw(Elements *self);
void Mushroom_destroy(Elements *self);

#endif
