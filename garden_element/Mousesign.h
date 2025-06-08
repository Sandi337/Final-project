#ifndef Mousesign_H_INCLUDED
#define Mousesign_H_INCLUDED
#include "../element/element.h"
#include "../scene/gamescene.h" // for element label
#include "../shapes/Shape.h"
#include <allegro5/allegro_color.h>
/*
   [Mousesign object]
*/
typedef struct _Mousesign
{
    int x, y;
    int r;          
    int width, height;
    Shape *hitbox;
    ALLEGRO_BITMAP *img;           // 指向目前顯示的圖  
    ALLEGRO_BITMAP *arrow_img;
    ALLEGRO_BITMAP *hand_img;
} Mousesign;
Elements *New_Mousesign(int label);
void Mousesign_update(Elements *self, float delta_time);
void Mousesign_interact(Elements *self);
void Mousesign_draw(Elements *self);
void Mousesign_destory(Elements *self);

#endif
