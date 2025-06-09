#include "Legend.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../shapes/Rectangle.h"
#include "../global.h" 
#include "../GameWindow.h" 
#include "../element/element_label.h"
/*
   [Legend function]
*/
Elements *New_Legend(int label)
{
    Legend *pDerivedObj = (Legend *)malloc(sizeof(Legend));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/legend.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = (WIDTH/2)-(pDerivedObj->width/2);
    pDerivedObj->y = pDerivedObj->height+250;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x + pDerivedObj->width / 3,
                                        pDerivedObj->y + pDerivedObj->height / 3,
                                        pDerivedObj->x + 2 * pDerivedObj->width / 3,
                                        pDerivedObj->y + 2 * pDerivedObj->height / 3);
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Legend_update;
    pObj->Interact = Legend_interact;
    pObj->Draw = Legend_draw;
    pObj->Destroy = Legend_destory;
    return pObj;
}
void Legend_update(Elements *self, float delta_time) {
    Legend *Obj = (Legend *)(self->pDerivedObj);

    // 判斷是否點擊到按鈕
    if (mouse_state[1] && 
        mouse.x >= Obj->x && mouse.x <= Obj->x + Obj->width &&
        mouse.y >= Obj->y && mouse.y <= Obj->y + Obj->height) 
    {
        // 切換 pause 狀態
        legend = !legend;

        // 為了避免連續觸發，加入防重複點擊判斷
        mouse_state[1] = false; // 清除點擊狀態
    }
}
void Legend_interact(Elements *self) {}
void Legend_draw(Elements *self)
{
    Legend *Obj = ((Legend *)(self->pDerivedObj));
    if(is_paused){
        al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
    }
}
void Legend_destory(Elements *self)
{
    Legend *Obj = ((Legend *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
