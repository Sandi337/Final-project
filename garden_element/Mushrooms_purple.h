#ifndef Mushrooms_purple_H_INCLUDED
#define Mushrooms_purple_H_INCLUDED
#include "../element/element.h" 
#include "../element/element_label.h"
#include "../shapes/Shape.h"
/*
   [Mushrooms_purple object]
*/
typedef struct _Mushrooms_purple
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
} Mushrooms_purple;
Elements *New_Mushrooms_purple(int label);
void Mushrooms_purple_update(Elements *self, float delta_time);
void Mushrooms_purple_interact(Elements *self);
void Mushrooms_purple_draw(Elements *self);
void Mushrooms_purple_destory(Elements *self);

#endif
