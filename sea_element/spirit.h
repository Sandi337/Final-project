#ifndef SPIRIT_H_INCLUDED
#define SPIRIT_H_INCLUDED
#include "../element/element.h"
#include "../scene/seascene.h" // for element label
#include "../element/element_label.h"
#include "../shapes/Shape.h"
#include <allegro5/allegro_font.h>
/*
   [Spirit object]
*/
typedef struct _Spirit
{
    int x, y;          // the position of image
    int r;
    int *spirit;
    double last_time; 
    double time_interval; 
    ALLEGRO_COLOR color;
    ALLEGRO_FONT *font;
} Spirit;
Elements *New_Spirit(int label);
void Spirit_update(Elements *self, float delta_time);
void Spirit_interact(Elements *self);
void _Spirit_interact_Character(Elements *self, Elements *tar);
void Spirit_draw(Elements *self);
void Spirit_destory(Elements *self);
void _Spirit_update_position(Elements *self, int dx, int dy);

#endif
