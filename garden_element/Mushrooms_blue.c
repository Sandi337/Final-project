#include "Mushrooms_blue.h"
#include "../shapes/Rectangle.h"
/*
   [Mushrooms_blue function]
*/
Elements *New_Mushrooms_blue(int label)
{
    Mushrooms_blue *pDerivedObj = (Mushrooms_blue *)malloc(sizeof(Mushrooms_blue));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/Mushrooms_blue0.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = WIDTH/2-64-300;
    pDerivedObj->y = HEIGHT - pDerivedObj->height-300;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x + pDerivedObj->width / 3,
                                        pDerivedObj->y + pDerivedObj->height / 3,
                                        pDerivedObj->x + 2 * pDerivedObj->width / 3,
                                        pDerivedObj->y + 2 * pDerivedObj->height / 3);
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Mushrooms_blue_update;
    pObj->Interact = Mushrooms_blue_interact;
    pObj->Draw = Mushrooms_blue_draw;
    pObj->Destroy = Mushrooms_blue_destory;
    return pObj;
}
void Mushrooms_blue_update(Elements *self, float delta_time) {}
void Mushrooms_blue_interact(Elements *self) {}
void Mushrooms_blue_draw(Elements *self)
{
    Mushrooms_blue *Obj = ((Mushrooms_blue *)(self->pDerivedObj));
    al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
}
void Mushrooms_blue_destory(Elements *self)
{
    Mushrooms_blue *Obj = ((Mushrooms_blue *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
