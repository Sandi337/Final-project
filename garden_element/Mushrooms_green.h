#ifndef Mushrooms_green_H_INCLUDED
#define Mushrooms_green_H_INCLUDED
#include "../element/element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
/*
   [Mushrooms_green object]
*/
typedef struct _Mushrooms_green
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
} Mushrooms_green;
Elements *New_Mushrooms_green(int label);
void Mushrooms_green_update(Elements *self, float delta_time);
void Mushrooms_green_interact(Elements *self);
void Mushrooms_green_draw(Elements *self);
void Mushrooms_green_destory(Elements *self);

#endif
