#include "Mushrooms_red.h"
#include "../shapes/Rectangle.h"
/*
   [Mushrooms_red function]
*/
Elements *New_Mushrooms_red(int label)
{
    Mushrooms_red *pDerivedObj = (Mushrooms_red *)malloc(sizeof(Mushrooms_red));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/Mushrooms_red0.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = WIDTH/2-64-100;
    pDerivedObj->y = HEIGHT - pDerivedObj->height-300;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x + pDerivedObj->width / 3,
                                        pDerivedObj->y + pDerivedObj->height / 3,
                                        pDerivedObj->x + 2 * pDerivedObj->width / 3,
                                        pDerivedObj->y + 2 * pDerivedObj->height / 3);
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Mushrooms_red_update;
    pObj->Interact = Mushrooms_red_interact;
    pObj->Draw = Mushrooms_red_draw;
    pObj->Destroy = Mushrooms_red_destory;
    return pObj;
}
void Mushrooms_red_update(Elements *self, float delta_time) {}
void Mushrooms_red_interact(Elements *self) {}
void Mushrooms_red_draw(Elements *self)
{
    Mushrooms_red *Obj = ((Mushrooms_red *)(self->pDerivedObj));
    al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
}
void Mushrooms_red_destory(Elements *self)
{
    Mushrooms_red *Obj = ((Mushrooms_red *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
