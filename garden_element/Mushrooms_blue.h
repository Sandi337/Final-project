#ifndef Mushrooms_blue_H_INCLUDED
#define Mushrooms_blue_H_INCLUDED
#include "../element/element.h" 
#include "../element/element_label.h"
#include "../shapes/Shape.h"
/*
   [Mushrooms_blue object]
*/
typedef struct _Mushrooms_blue
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
} Mushrooms_blue;
Elements *New_Mushrooms_blue(int label);
void Mushrooms_blue_update(Elements *self, float delta_time);
void Mushrooms_blue_interact(Elements *self);
void Mushrooms_blue_draw(Elements *self);
void Mushrooms_blue_destory(Elements *self);

#endif
