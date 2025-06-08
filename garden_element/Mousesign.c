#include "Mousesign.h"
#include "Pause_button.h"
#include "../element/charater.h"
#include "../shapes/Circle.h"
#include "../scene/gamescene.h" // for element label
#include "../scene/sceneManager.h" // for scene variable
#include <allegro5/allegro_primitives.h>
/*
   [Mousesign function]
*/
Elements *New_Mousesign(int label)
{
    Mousesign *pDerivedObj = (Mousesign *)malloc(sizeof(Mousesign));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->x = mouse.x;
    pDerivedObj->y = mouse.y;
    pDerivedObj->r = 10;
    pDerivedObj->arrow_img = al_load_bitmap("assets/image/arrow.png");
    pDerivedObj->hand_img = al_load_bitmap("assets/image/hand.png");
    pDerivedObj->img = pDerivedObj->arrow_img;
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->hitbox = New_Circle(pDerivedObj->x,
                                     pDerivedObj->y,
                                     pDerivedObj->r);
    // setting the interact object
    pObj->inter_obj[pObj->inter_len++] = Pause_button_L;
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Mousesign_update;
    pObj->Interact = Mousesign_interact;
    pObj->Draw = Mousesign_draw;
    pObj->Destroy = Mousesign_destory;

    return pObj;
}

void _Mousesign_update_position(Elements *self, int dx, int dy)
{
    Mousesign *Obj = ((Mousesign *)(self->pDerivedObj));
    Obj->x += dx;
    Obj->y += dy;
    Shape *hitbox = Obj->hitbox;
    hitbox->update_center_x(hitbox, dx);
    hitbox->update_center_y(hitbox, dy);
}
void Mousesign_update(Elements *self, float delta_time)
{
    Mousesign *Obj = ((Mousesign *)(self->pDerivedObj));
    _Mousesign_update_position(self, mouse.x-Obj->x, mouse.y-Obj->y);
}
void Mousesign_interact(Elements *self)
{
    Mousesign *Obj = ((Mousesign *)(self->pDerivedObj));
    ElementVec labelEle = _Get_label_elements(scene,Pause_button_L);
    Pause_button *Obj2 = ((Pause_button *)(labelEle.arr[0]->pDerivedObj));
    
    if (Obj->hitbox->overlap(Obj->hitbox, Obj2->hitbox))
    {
        Obj->img = Obj->hand_img;
    }else{
        Obj->img = Obj->arrow_img;
    }
}

void Mousesign_draw(Elements *self)
{
    Mousesign *Obj = ((Mousesign *)(self->pDerivedObj));
    al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
}
void Mousesign_destory(Elements *self)
{
    Mousesign *Obj = ((Mousesign *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->arrow_img);
    al_destroy_bitmap(Obj->hand_img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
