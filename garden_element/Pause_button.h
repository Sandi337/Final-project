#ifndef Pause_button_H_INCLUDED
#define Pause_button_H_INCLUDED
#include "../element/element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
/*
   [Pause_button object]
*/
typedef struct _Pause_button
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
} Pause_button;
Elements *New_Pause_button(int label);
void Pause_button_update(Elements *self, float delta_time);
void Pause_button_interact(Elements *self);
void Pause_button_draw(Elements *self);
void Pause_button_destory(Elements *self);

#endif
