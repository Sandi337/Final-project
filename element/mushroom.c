#include "mushroom.h"
#include "charater.h"
#include "projectile.h"
#include <stdlib.h>
#include <time.h>
#include "../shapes/Circle.h"
#include "../scene/sceneManager.h" // for scene variable
#include "../scene/gamescene.h"    // for element label
#include <allegro5/allegro_audio.h>
/*
   [MUSHROOM function]
*/

static ALLEGRO_SAMPLE *eat_sound = NULL;

Elements *New_Mushroom(int label)
{
    Mushroom *pDerivedObj = (Mushroom *)malloc(sizeof(Mushroom));
    Elements *pObj = New_Elements(label);
    static ALLEGRO_SAMPLE *eat_sound = NULL;
    // 載入音效（僅第一次載入）
    if (!eat_sound) {
        eat_sound = al_load_sample("assets/sound/eat_sound.wav");
        if (!eat_sound) {
            fprintf(stderr, "Failed to load eat_sound.mp3!\n");
            exit(1);
        }
    }

    // 隨機選擇蘑菇類型 (0: 綠, 1: 紅, 2: 藍, 3: 彩虹)
    pDerivedObj->type = rand() % 4;
    switch (pDerivedObj->type) {
        case 0: pDerivedObj->img = al_load_bitmap("assets/image/green_mushroom.png"); break;
        case 1: pDerivedObj->img = al_load_bitmap("assets/image/red_mushroom.png"); break;
        case 2: pDerivedObj->img = al_load_bitmap("assets/image/blue_mushroom.png"); break;
        case 3: pDerivedObj->img = al_load_bitmap("assets/image/rainbow_mushroom.png"); break;
    }
    if (!pDerivedObj->img) {
        fprintf(stderr, "Failed to load mushroom image!\n");
        exit(1);
    }

    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    // 隨機位置（避免與地面重疊，假設地面在下半部）
    pDerivedObj->x = rand() % (WIDTH - pDerivedObj->width);
    pDerivedObj->y = (HEIGHT / 2) + (rand() % (HEIGHT / 2 - pDerivedObj->height));
    pDerivedObj->active = true;
    pDerivedObj->hitbox = New_Circle(pDerivedObj->x + pDerivedObj->width / 2,
                                     pDerivedObj->y + pDerivedObj->height / 2,
                                     min(pDerivedObj->width, pDerivedObj->height) / 2);

    pObj->inter_obj[pObj->inter_len++] = Projectile_L; // 與投射物交互
    pObj->inter_obj[pObj->inter_len++] = Character_L;  // 與角色交互（後續效果）

    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Mushroom_update;
    pObj->Interact = Mushroom_interact;
    pObj->Draw = Mushroom_draw;
    pObj->Destroy = Mushroom_destroy;

    return pObj;
}

void Mushroom_update(Elements *self)
{
    // 目前無需更新，保持 active 狀態
     Mushroom *Obj = ((Mushroom *)(self->pDerivedObj));
     Obj->hitbox->update_center_x(Obj->hitbox, Obj->x + Obj->width / 2);
    Obj->hitbox->update_center_y(Obj->hitbox, Obj->y + Obj->height / 2);
     //Shape *hitbox = Obj->hitbox;
    //hitbox->update_center_x(hitbox, mouse.x - Obj->x);
    //hitbox->update_center_y(hitbox, mouse.y - Obj->y);
}

void Mushroom_interact(Elements *self)
{
    Mushroom *mush = (Mushroom *)(self->pDerivedObj);
    ElementVec projectiles = _Get_label_elements(scene, Projectile_L);
    for (int i = 0; i < projectiles.len; i++) {
        _Mushroom_interact_Projectile(self, projectiles.arr[i]);
    }
    ElementVec characters = _Get_label_elements(scene, Character_L);
    for (int i = 0; i < characters.len; i++) {
        _Mushroom_interact_Character(self, characters.arr[i]);
    }
    /*Mushroom *Obj = ((Mushroom *)(self->pDerivedObj));
    ElementVec labelEle = _Get_label_elements(scene, Projectile_L);
    Projectile *Obj1 = (Projectile *)(labelEle.arr[0]->pDerivedObj);
    for (int i = 0; i < labelEle.len; i++) {
        _Mushroom_interact_Projectile(self, labelEle.arr[i]);
    }
    ElementVec characters = _Get_label_elements(scene, Character_L);
    labelEle = _Get_label_elements(scene, Character_L);
    Character *Obj2 = (Character *)(labelEle.arr[0]->pDerivedObj);
    for (int i = 0; i < characters.len; i++) {
        _Mushroom_interact_Character(self, characters.arr[i]);
    }
    if (Obj->hitbox->overlap(Obj->hitbox, Obj1->hitbox))
    {
        Obj->active = false;
    }else if (Obj->hitbox->overlap(Obj->hitbox, Obj2->hitbox)){
        Obj->active = false;
    }else{
        Obj->active = true;
    }*/
}

void _Mushroom_interact_Projectile(Elements *self, Elements *tar)
{
    Mushroom *mush = (Mushroom *)(self->pDerivedObj);
    Projectile *proj = (Projectile *)(tar->pDerivedObj);
    if (mush->active && !proj->done) {
        // 使用 hitbox 進行圓形碰撞檢測
        mush->hitbox->update_center_x(mush->hitbox, mush->x + mush->width / 2);
        mush->hitbox->update_center_y(mush->hitbox, mush->y + mush->height / 2);
        proj->hitbox->update_center_x(proj->hitbox, proj->x + proj->width / 2);
        proj->hitbox->update_center_y(proj->hitbox, proj->y + proj->height / 2);

        if (mush->hitbox->overlap(mush->hitbox, proj->hitbox)) {
            mush->active = false; // 蘑菇消失
            proj->done = true;    // 投射物完成
            al_play_sample(eat_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
    }
    /*if (proj->hitbox->overlap(proj->hitbox, mush->hitbox))
    {
        self->dele = true;
    }
    if (mush->active && proj->done == false) {
        if (proj->x >= mush->x && proj->x <= mush->x + mush->width &&
            proj->y >= mush->y && proj->y <= mush->y + mush->height) {
            mush->active = false; // 蘑菇消失
            proj->done = true;    // 投射物完成
            al_play_sample(eat_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
        }
    }*/
}

void _Mushroom_interact_Character(Elements *self, Elements *tar)
{
    Mushroom *mush = (Mushroom *)(self->pDerivedObj);
    Character *chara = (Character *)(tar->pDerivedObj);
    if (!mush->active) { // 蘑菇已被擊中，應用效果
        if (chara->x >= mush->x && chara->x <= mush->x + mush->width &&
            chara->y >= mush->y && chara->y <= mush->y + mush->height) {
            switch (mush->type) {
                case 0: // 綠蘑菇
                    chara->energy += 5;
                    break;
                case 1: // 紅蘑菇
                case 2: // 藍蘑菇
                case 3: // 彩虹蘑菇
                    chara->health -= 10;
                    break;
            }
            mush->active = false; // 確保僅應用一次
        }
    }
}

void Mushroom_draw(Elements *self)
{
    Mushroom *mush = (Mushroom *)(self->pDerivedObj);
    if (mush->active) {
        al_draw_bitmap(mush->img, mush->x, mush->y, 0);
    }
}

void Mushroom_destroy(Elements *self)
{
    Mushroom *mush = (Mushroom *)(self->pDerivedObj);
    al_destroy_bitmap(mush->img);
    if (eat_sound) al_destroy_sample(eat_sound); // 僅銷毀一次
    if (mush->hitbox) free(mush->hitbox); ;
    free(mush);
    free(self);
}