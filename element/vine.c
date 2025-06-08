#include "vine.h"
#include "../shapes/Rectangle.h"
#include <allegro5/allegro_primitives.h>
#include "../scene/sceneManager.h" // for scene variable
#include <allegro5/allegro_audio.h>
#include "../audio/audio.h"
#include "charater.h"
#include <stdlib.h>
#include <time.h>
#define SHOW_DURATION 5.0          // 藤蔓顯示 5 秒
#define MIN_GAP 1.0                // 下次出現最短間隔
#define MAX_GAP 4.0                // 最長間隔
#define DEBUG_HITBOX 0             // <== 開啟 hitbox debug 模式

extern Scene *scene;
/*
   [Vine function]
*/
Elements *New_Vine(int label)
{
    srand(time(NULL));
    Vine *pDerivedObj = (Vine *)malloc(sizeof(Vine));
    Elements *pObj = New_Elements(label);
    // setting derived object member
    pDerivedObj->img = al_load_bitmap("assets/image/vine1.png");
    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    pDerivedObj->x = -30;
    pDerivedObj->y = HEIGHT - pDerivedObj->height+30;
    pDerivedObj->hitbox = New_Rectangle(pDerivedObj->x + pDerivedObj->width / 3,
                                        pDerivedObj->y ,
                                        pDerivedObj->x + 2 * pDerivedObj->width / 3,
                                        pDerivedObj->y +pDerivedObj->height );
    
    pObj->inter_obj[pObj->inter_len++] = Character_L;  // 與角色交互（後續效果）
    // setting derived object function
    pObj->pDerivedObj = pDerivedObj;
    pDerivedObj->visible        = false;            // 一開始不顯示
    pDerivedObj->next_appear_ts = al_get_time() + 2;// 2 秒後第一次嘗試出現
    pDerivedObj->visible_since  = 0;
    pObj->Update = Vine_update;
    pObj->Interact = Vine_interact;
    pObj->Draw = Vine_draw;
    pObj->Destroy = Vine_destory;
    return pObj;
}
void Vine_update(Elements *self, float delta_time) {
    Vine *v = (Vine *)self->pDerivedObj;
    double now = al_get_time();

     /* 更新 hitbox 位置*/
    Rectangle *rect = (Rectangle *)(v->hitbox->pDerivedObj);
    rect->x1 = v->x + v->width / 3;
    rect->y1 = v->y ;
    rect->x2 = v->x + 2 * v->width / 3;
    rect->y2 = v->y + v->height; 

    if (v->visible) {                                 // ★ 已經顯示
        //printf("[DEBUG] Vine is visible at x=%d\n", v->x);
        if (now - v->visible_since >= SHOW_DURATION) {
            v->visible = false;                       // 消失
            /* 安排下一次出現時間 */
            double gap = MIN_GAP + (rand() / (double)RAND_MAX) * (MAX_GAP - MIN_GAP);
            v->next_appear_ts = now + gap;
        }
    }
    else {                                            // ★ 尚未顯示
        if (now >= v->next_appear_ts) {
            /* 隨機橫向位置重新生成 */
            v->x = rand() % (WIDTH - v->width);
            v->visible = true;
            v->visible_since = now; 
        }
    }
}
void Vine_interact(Elements *self) {
   Vine *vine = (Vine *)(self->pDerivedObj);
    if (!vine->visible) return;

    ElementVec chars = _Get_label_elements(scene, Character_L);
    for (int i = 0; i < chars.len; i++) {
        Character *chara = (Character *)(chars.arr[i]->pDerivedObj);
        if (vine->hitbox->overlap(vine->hitbox, chara->hitbox)) {
            chara->health -= 15;
            if (chara->health < 0) chara->health = 0;
            printf("[VINE HIT] HP reduced to %d\n", chara->health);
            play_hurt_sound();
            vine->visible = false;
            vine->next_appear_ts = al_get_time() + 3.0;
        }
    }
}
void Vine_draw(Elements *self)
{
    Vine *Obj = ((Vine *)(self->pDerivedObj));
     //al_draw_rectangle(300, 300, 400, 400, al_map_rgb(255, 0, 0), 2);
    if (!Obj->visible) return;

    al_draw_bitmap(Obj->img, Obj->x, Obj->y, 0);
    
    #if DEBUG_HITBOX
    if (Obj->hitbox && Obj->hitbox->pDerivedObj && Obj->hitbox->getType && Obj->hitbox->getType() == RECTANGLE) {
        Rectangle *rect = (Rectangle *)(Obj->hitbox->pDerivedObj);
        al_draw_rectangle(
            rect->x1, rect->y1,
            rect->x2, rect->y2,
            al_map_rgb(255, 0, 0),10
        );
    }
    #endif

}
void Vine_destory(Elements *self)
{
    Vine *Obj = ((Vine *)(self->pDerivedObj));
    al_destroy_bitmap(Obj->img);
    free(Obj->hitbox);
    free(Obj);
    free(self);
}
