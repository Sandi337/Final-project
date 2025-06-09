#include "mushroom.h"
#include "charater.h"
#include "hud.h"
#include "projectile.h"
#include <stdlib.h>
#include <time.h>
#include "../shapes/Circle.h"
#include "../scene/sceneManager.h" // for scene variable
#include "../scene/gamescene.h"    // for element label
#include <allegro5/allegro_audio.h>
#include "../audio/audio.h"

#define MIN_Y_HEIGHT_RATIO 0.7 // 新上界比例，範圍 0 到 1
#define HEIGHT 720            // 假設螢幕高度
#define clamp(v, lo, hi) ((v) < (lo) ? (lo) : ((v) > (hi) ? (hi) : (v)))
/*
   [MUSHROOM function]
*/

extern Scene *scene;

static ALLEGRO_SAMPLE *eat_sound = NULL;

static Elements* Get_Character_Element() {
    ElementVec all = _Get_all_elements(scene);
    printf("[DEBUG] Get_Character_Element checking elements\n");
    for (int i = 0; i < all.len; i++) {
        if (all.arr[i]->label == Character_L){
            printf("[DEBUG] label=%d\n", all.arr[i]->label);
            return all.arr[i];
        }
    }
    return NULL;
}

Elements *New_Mushroom(int label)
{
    Mushroom *pDerivedObj = (Mushroom *)malloc(sizeof(Mushroom));
    Elements *pObj = New_Elements(label);
    // 載入音效（僅第一次載入）
    if (!eat_sound) {
        eat_sound = al_load_sample("assets/sound/eat_sound.wav");
        if (!eat_sound) {
            fprintf(stderr, "Failed to load eat_sound.wav!\n");
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
        return NULL;
    }

    pDerivedObj->width = al_get_bitmap_width(pDerivedObj->img);
    pDerivedObj->height = al_get_bitmap_height(pDerivedObj->img);
    // 隨機位置（避免與地面重疊，假設地面在下半部）
    pDerivedObj->x = rand() % (WIDTH - pDerivedObj->width);
    int min_y = HEIGHT * MIN_Y_HEIGHT_RATIO;               // 新上界
    int max_y = HEIGHT - pDerivedObj->height;              // 下界
    pDerivedObj->y = (rand() % (max_y - min_y + 1)) + min_y;
    pDerivedObj->active = true;
    pDerivedObj->hitbox = New_Circle(pDerivedObj->x + pDerivedObj->width / 2,
                                     pDerivedObj->y + pDerivedObj->height / 2,
                                     min(pDerivedObj->width, pDerivedObj->height)* 0.5
                                    );

    pObj->inter_obj[pObj->inter_len++] = Projectile_L; // 與投射物交互
    pObj->inter_obj[pObj->inter_len++] = Character_L;  // 與角色交互（後續效果）

    pObj->pDerivedObj = pDerivedObj;
    pObj->Update = Mushroom_update;
    pObj->Interact = Mushroom_interact;
    pObj->Draw = Mushroom_draw;
    pObj->Destroy = Mushroom_destroy;

    return pObj;
}

void Mushroom_update(Elements *self, float delta_time)
{
    Mushroom *m = (Mushroom *)(self->pDerivedObj);
    m->hitbox->update_center_x(m->hitbox, m->x + m->width / 2);
    m->hitbox->update_center_y(m->hitbox, m->y + m->height / 2);

    if (m->x + m->width <= 1) {
        printf("Mushroom auto delete at x = %d\n", m->x);
        self->dele = true;
    }
    
}

void Mushroom_interact(Elements *self)
{
    //if (self->dele) return;

    for (int j = 0; j < self->inter_len; j++)
    {
        int inter_label = self->inter_obj[j];
        ElementVec labelEle = _Get_label_elements(scene, inter_label);
        for (int i = 0; i < labelEle.len; i++)
        {
            if (inter_label == Character_L)
            {
                _Mushroom_interact_Character(self, labelEle.arr[i]);
            }
            else if (inter_label == Projectile_L)
            {
                _Mushroom_interact_Projectile(self, labelEle.arr[i]);
            }
        }
    }

}

void _Mushroom_interact_Projectile(Elements *self, Elements *tar)
{
    Mushroom *mush = (Mushroom *)(self->pDerivedObj);
    Projectile *proj = (Projectile *)(tar->pDerivedObj);
    if (!mush->active) return;  // 沒被點過就忽略
    if (mush->hitbox->overlap(mush->hitbox, proj->hitbox) ) {
        play_eat_sound();

        proj->done = true;
        mush->active = false;
        self->dele = true;
     }
}

void _Mushroom_interact_Character(Elements *self, Elements *tar)
{
   /* Mushroom *mush = (Mushroom *)(self->pDerivedObj);
    Character *chara = (Character *)(tar->pDerivedObj);
    */
}

void Mushroom_draw(Elements *self)
{
    Mushroom *mush = ((Mushroom *)(self->pDerivedObj));
    if (mush->active) {
        al_draw_bitmap(mush->img, mush->x, mush->y, 0);
        //al_draw_circle(mush->hitbox->center_x, mush->hitbox->center_y,  al_map_rgb(0, 255, 0), 2);
    }
}

void Mushroom_destroy(Elements *self)
{
    Mushroom *mush = (Mushroom *)(self->pDerivedObj);

    // 找角色
    Elements *character_ele = Get_Character_Element();
    if (character_ele && mush->was_clicked) {
        Character *chara = (Character *)character_ele->pDerivedObj;
        printf("[DESTROY] The mushroom has been eaten. Updating the character's stats...\n");

        switch (mush->type) {
            case 0: chara->energy += 10; break; // 綠色：補能量
            case 1: chara->health -= 10; break;// 紅色：扣血
            case 2: chara->spirit -= 10; break;// 藍色：扣精神
            case 3:// 彩虹：全扣
                chara->health -= 20;
                //chara->energy -= 20;
                chara->spirit -= 20;
                break;
        }

        // 限制 0~100
        chara->health = clamp(chara->health, 0, MAX_HEALTH);
        chara->energy = clamp(chara->energy, 0, MAX_ENERGY);
        chara->spirit = clamp(chara->spirit, 0, MAX_SPIRIT);

        printf("[STATUS] HP=%d EN=%d SP=%d\n", chara->health, chara->energy, chara->spirit);
    }

    al_destroy_bitmap(mush->img);
    free(mush->hitbox);
    free(mush);
    free(self);
}