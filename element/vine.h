#ifndef Vine_H_INCLUDED
#define Vine_H_INCLUDED
#include "../element/element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
/*
   [Vine object]
*/
typedef struct _Vine
{
    int x, y;          // the position of image
    int width, height; // the width and height of image
    ALLEGRO_BITMAP *img;
    Shape *hitbox; // the hitbox of object
    bool   visible;          // 當前是否顯示
    double next_appear_ts;   // 下次出現的時間點
    double visible_since;    // 開始顯示的時間
} Vine;
Elements *New_Vine(int label);
void Vine_update(Elements *self, float delta_time);
void Vine_interact(Elements *self);
void Vine_draw(Elements *self);
void Vine_destory(Elements *self);

#endif
