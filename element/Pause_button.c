#include "Pause_button.h"
#include "../shapes/Rectangle.h"
#include "../global.h" 
#include "../GameWindow.h" 
/*
   [Pause_button function]
*/
Elements *New_Pause_button(int label)
{
    Pause_button *pDerivedObj = (Pause_button *)malloc(sizeof(Pause_button));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/pause1.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = WIDTH-pDerivedObj->width-30;
    pDerivedObj->y = pDerivedObj->height-60;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x + pDerivedObj->width / 3,
                                        pDerivedObj->y + pDerivedObj->height / 3,
                                        pDerivedObj->x + 2 * pDerivedObj->width / 3,
                                        pDerivedObj->y + 2 * pDerivedObj->height / 3);
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Pause_button_update;
    pObj->Interact = Pause_button_interact;
    pObj->Draw = Pause_button_draw;
    pObj->Destroy = Pause_button_destory;
    return pObj;
}
void Pause_button_update(Elements *self) {
    Pause_button *Obj = (Pause_button *)(self->pDerivedObj);

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
void Pause_button_interact(Elements *self) {}
void Pause_button_draw(Elements *self)
{
    Pause_button *Obj = ((Pause_button *)(self->pDerivedObj));
    al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
}
void Pause_button_destory(Elements *self)
{
    Pause_button *Obj = ((Pause_button *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
