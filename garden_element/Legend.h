#ifndef Legend_H_INCLUDED
#define Legend_H_INCLUDED
#include "../element/element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
/*
   [Legend object]
*/
typedef struct _Legend
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
} Legend;
Elements *New_Legend(int label);
void Legend_update(Elements *self, float delta_time);
void Legend_interact(Elements *self);
void Legend_draw(Elements *self);
void Legend_destory(Elements *self);

#endif
