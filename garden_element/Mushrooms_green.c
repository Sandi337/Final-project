#include "Mushrooms_green.h"
#include "../shapes/Rectangle.h"
/*
   [Mushrooms_green function]
*/
Elements *New_Mushrooms_green(int label)
{
    Mushrooms_green *pDerivedObj = (Mushrooms_green *)malloc(sizeof(Mushrooms_green));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/Mushrooms_green0.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = WIDTH/2-64+100;
    pDerivedObj->y = HEIGHT - pDerivedObj->height-300;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x + pDerivedObj->width / 3,
                                        pDerivedObj->y + pDerivedObj->height / 3,
                                        pDerivedObj->x + 2 * pDerivedObj->width / 3,
                                        pDerivedObj->y + 2 * pDerivedObj->height / 3);
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Mushrooms_green_update;
    pObj->Interact = Mushrooms_green_interact;
    pObj->Draw = Mushrooms_green_draw;
    pObj->Destroy = Mushrooms_green_destory;
    return pObj;
}
void Mushrooms_green_update(Elements *self, float delta_time) {}
void Mushrooms_green_interact(Elements *self) {}
void Mushrooms_green_draw(Elements *self)
{
    Mushrooms_green *Obj = ((Mushrooms_green *)(self->pDerivedObj));
    al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
}
void Mushrooms_green_destory(Elements *self)
{
    Mushrooms_green *Obj = ((Mushrooms_green *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
