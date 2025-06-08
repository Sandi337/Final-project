#ifndef Gardendoor_H_INCLUDED
#define Gardendoor_H_INCLUDED
#include "../element/element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
/*
   [Gardendoor object]
*/
typedef struct _Gardendoor
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
} Gardendoor;
Elements *New_Gardendoor(int label);
void Gardendoor_update(Elements *self, float delta_time);
void Gardendoor_interact(Elements *self);
void Gardendoor_draw(Elements *self);
void Gardendoor_destory(Elements *self);

#endif
