#ifndef Continue_H_INCLUDED
#define Continue_H_INCLUDED
#include "../element/element.h" 
#include "../element/element_label.h"
#include "../shapes/Shape.h"
/*
   [Continue object]
*/
typedef struct _Continue
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
} Continue;
Elements *New_Continue(int label);
void Continue_update(Elements *self, float delta_time);
void Continue_interact(Elements *self);
void Continue_draw(Elements *self);
void Continue_destory(Elements *self);

#endif
