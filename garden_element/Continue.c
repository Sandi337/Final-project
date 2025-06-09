#include "Continue.h"
#include "../shapes/Rectangle.h"
#include "../element/element_label.h"
#include "../global.h" 
#include "../GameWindow.h" 
/*
   [Continue function]
*/
Elements *New_Continue(int label)
{
    Continue *pDerivedObj = (Continue *)malloc(sizeof(Continue));
    Elements *pObj = New_Elements(Continue_L);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/continue.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = (WIDTH/2)-(pDerivedObj->width/2);
    pDerivedObj->y = pDerivedObj->height+50;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x + pDerivedObj->width / 3,
                                        pDerivedObj->y + pDerivedObj->height / 3,
                                        pDerivedObj->x + 2 * pDerivedObj->width / 3,
                                        pDerivedObj->y + 2 * pDerivedObj->height / 3);
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Continue_update;
    pObj->Interact = Continue_interact;
    pObj->Draw = Continue_draw;
    pObj->Destroy = Continue_destory;
    return pObj;
}
void Continue_update(Elements *self, float delta_time) {
    Continue *Obj = (Continue *)(self->pDerivedObj);

    // 判斷是否點擊到按鈕
    if (mouse_state[1] && 
        mouse.x >= Obj->x && mouse.x <= Obj->x + Obj->width &&
        mouse.y >= Obj->y && mouse.y <= Obj->y + Obj->height) 
    {
        // 切換 pause 狀態
        is_paused = !is_paused;

        // 為了避免連續觸發，加入防重複點擊判斷
        mouse_state[1] = false; // 清除點擊狀態
    }
}
void Continue_interact(Elements *self) {}
void Continue_draw(Elements *self)
{
    Continue *Obj = ((Continue *)(self->pDerivedObj));
    if(is_paused){
        al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
    }
}
void Continue_destory(Elements *self)
{
    Continue *Obj = ((Continue *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
