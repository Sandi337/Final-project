#include "gardendoor.h"
#include "../shapes/Rectangle.h"
#include "../scene/sceneManager.h"
/*
   [Gardendoor function]
*/
Elements *New_Gardendoor(int label)
{
    Gardendoor *pDerivedObj = (Gardendoor *)malloc(sizeof(Gardendoor));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/garden_door.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = 40;
    pDerivedObj->y = HEIGHT - pDerivedObj->height-10;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x + pDerivedObj->width / 3,
                                        pDerivedObj->y ,
                                        pDerivedObj->x + 2 * pDerivedObj->width / 3,
                                        pDerivedObj->y +pDerivedObj->height);
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Gardendoor_update;
    pObj->Interact = Gardendoor_interact;
    pObj->Draw = Gardendoor_draw;
    pObj->Destroy = Gardendoor_destory;
    return pObj;
}
void Gardendoor_update(Elements *self, float delta_time) {
    Gardendoor *gd = (Gardendoor *)self->pDerivedObj;
    /* 更新 hitbox 位置*/
    Rectangle *rect = (Rectangle *)(gd->hitbox->pDerivedObj);
    rect->x1 = gd->x + gd->width / 3;
    rect->y1 = gd->y ;
    rect->x2 = gd->x + 2 * gd->width / 3;
    rect->y2 = gd->y + gd->height; 
}
void Gardendoor_interact(Elements *self) {
    Gardendoor *door = (Gardendoor *)(self->pDerivedObj);
    extern Scene *scene;
    if (mouse_state[1]) {
    if (mouse.x >= door->x && mouse.x <= door->x + door->width &&
        mouse.y >= door->y && mouse.y <= door->y + door->height) {
        printf("[GARDEN] Door clicked! Entering garden...\n");
        window = 2; // GardenScene_L = 2;
        scene->scene_end = true; // 切換場景
        mouse_state[1] = false;
    }
}
}
void Gardendoor_draw(Elements *self)
{
    Gardendoor *Obj = ((Gardendoor *)(self->pDerivedObj));
    al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
}
void Gardendoor_destory(Elements *self)
{
    Gardendoor *Obj = ((Gardendoor *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
