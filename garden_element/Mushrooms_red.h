#ifndef Mushrooms_red_H_INCLUDED
#define Mushrooms_red_H_INCLUDED
#include "../element/element.h" 
#include "../element/element_label.h"
#include "../shapes/Shape.h"
/*
   [Mushrooms_red object]
*/
typedef struct _Mushrooms_red
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
} Mushrooms_red;
Elements *New_Mushrooms_red(int label);
void Mushrooms_red_update(Elements *self, float delta_time);
void Mushrooms_red_interact(Elements *self);
void Mushrooms_red_draw(Elements *self);
void Mushrooms_red_destory(Elements *self);

#endif
