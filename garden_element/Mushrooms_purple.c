#include "Mushrooms_purple.h"
#include "../shapes/Rectangle.h"
/*
   [Mushrooms_purple function]
*/
Elements *New_Mushrooms_purple(int label)
{
    Mushrooms_purple *pDerivedObj = (Mushrooms_purple *)malloc(sizeof(Mushrooms_purple));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/Mushrooms_purple0.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x =  WIDTH/2-64+300;
    pDerivedObj->y = HEIGHT - pDerivedObj->height-300;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x + pDerivedObj->width / 3,
                                        pDerivedObj->y + pDerivedObj->height / 3,
                                        pDerivedObj->x + 2 * pDerivedObj->width / 3,
                                        pDerivedObj->y + 2 * pDerivedObj->height / 3);
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Mushrooms_purple_update;
    pObj->Interact = Mushrooms_purple_interact;
    pObj->Draw = Mushrooms_purple_draw;
    pObj->Destroy = Mushrooms_purple_destory;
    return pObj;
}
void Mushrooms_purple_update(Elements *self, float delta_time) {}
void Mushrooms_purple_interact(Elements *self) {}
void Mushrooms_purple_draw(Elements *self)
{
    Mushrooms_purple *Obj = ((Mushrooms_purple *)(self->pDerivedObj));
    al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
}
void Mushrooms_purple_destory(Elements *self)
{
    Mushrooms_purple *Obj = ((Mushrooms_purple *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
