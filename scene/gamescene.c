#include <allegro5/allegro_audio.h>
#include "../audio/audio.h"
#include <allegro5/allegro_font.h>

#include "gamescene.h"
#include "../scene/sceneManager.h"
#include "../global.h"
#include "../element/charater.h"
#include "../element/hud.h"
#include "../element/element_label.h"
#include "../element/mushroom.h"
#include "../element/vine.h"
#include "../element/tree.h"
#include "../element/projectile.h"
#include "../element/gardendoor.h"


#define FPS 60
#define MIN_MUSHROOM_DISTANCE 200  // 蘑菇之間最小距離（像素）
/*
   [GameScene function]
*/
Scene *New_GameScene(int label, CharacterStatus *status)
{
    printf("[DEBUG] Entering New_GameScene(label=%d)\n", label);
    fflush(stdout);

    GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
    Scene *pObj = New_Scene(label, status);
    // Load font
    pDerivedObj->font = al_load_font("assets/font/ByteBounce.ttf", 35, 0);
    if (!pDerivedObj->font) {
    fprintf(stderr, "[ERROR] Failed to load font!\n");
    exit(1);
    }
    
    play_bgm();
    
    // setting derived object member
    pDerivedObj->background = al_load_bitmap("assets/image/main_scene.png");
    if (!pDerivedObj->background) {
        fprintf(stderr, "Failed to load background: assets/image/main_scene.png\n");
        exit(1);
    }

    // 初始化隨機數生成器
    srand(time(NULL));

    // register element
    _Register_elements(pObj, New_Character(Character_L, status));
    _Register_elements(pObj, New_Vine(Vine_L));
    _Register_elements(pObj, New_Tree(Tree_L));
    _Register_elements(pObj, New_Mushroom(Mushroom_L));
    //_Register_elements(pObj, New_Gardendoor(Gardendoor_L)); 
    //_Register_elements(pObj, New_Pause_button(Pause_button_L));
    
    /*_Register_elements(pObj, New_Continue(Continue_L));
    _Register_elements(pObj, New_Legend(Legend_L));*/

    // 隨機生成 1-3 個蘑菇
    int mushroom_count = 1 + (rand() % 3);
    int placed = 0;

    while (placed < mushroom_count) {
        Elements *new_mush = New_Mushroom(Mushroom_L);
        Mushroom *mushA = (Mushroom *)new_mush->pDerivedObj;

        bool too_close = false;
        ElementVec all = _Get_all_elements(pObj);
        for (int j = 0; j < all.len; j++) {
            if (all.arr[j]->label != Mushroom_L) continue;
            Mushroom *mushB = (Mushroom *)all.arr[j]->pDerivedObj;

            float dx = mushA->x - mushB->x;
            float dy = mushA->y - mushB->y;
            float dist = sqrtf(dx * dx + dy * dy);

            if (dist < MIN_MUSHROOM_DISTANCE) {
                too_close = true;
                break;
            }
        }

        if (!too_close) {
            _Register_elements(pObj, new_mush);
            placed++;
        } else {
            // 太近了就重做這顆
            Mushroom_destroy(new_mush);
        }
    }
    // Initialize scroll variables
    pDerivedObj->bg_offset_x = 0.0;
    pDerivedObj->scroll_speed = 1.0; // 根據需求調整
    pDerivedObj->start_time = al_get_time();
    pDerivedObj->energy_timer = 0.0;
    pDerivedObj->garden_portal_spawned = false;
    pDerivedObj->garden_portal_timer = 0.0;
    pDerivedObj->has_switched_to_sea = false;
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Update = game_scene_update;
    pObj->Draw = game_scene_draw;
    pObj->Destroy = game_scene_destroy;
    return pObj;
}

void game_scene_update(Scene *self, float delta_time)
{
    /*printf("[DEBUG] in game_scene_update, dt=%.3f\n", delta_time);
    if (delta_time > 1.0f || delta_time <= 0.0f) delta_time = 1.0f / FPS; // 限制 delta_time*/
    // update every element
    ElementVec allEle = _Get_all_elements(self);
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    Character *chara = NULL;
    
    // Find the character
    for (int i = 0; i < allEle.len; i++) {
        if (allEle.arr[i]->label == Character_L) {
            chara = (Character *)allEle.arr[i]->pDerivedObj;
            //printf("[DEBUG] Found Character at index %d, spirit = %d\n", i, chara->spirit); // 調試輸出
            break;
        }
    }
    if (!chara) {
        printf("[ERROR] Character not found in allEle!\n");
        return; // 若未找到，退出更新
    }
    if (chara->status->HP <= 0) {
        window = GameOverScene_L;
        self->scene_end = true;
    }
    
    // 能量每 5 秒扣 5 點
    //delta_time =  1.0 / 60 ;//每秒更新 60 次
    gs->energy_timer += delta_time;
    if (gs->energy_timer >= 5.0f && chara) {
        chara->status->EN -= 5;
        if (chara->status->EN < 0) chara->status->EN = 0;
        gs->energy_timer = 0.0f;

        printf("[TIMER]Auto deduct energy every 5 seconds, remaining Energy = %d\n", chara->status->EN);
    }
    // === 1. 背景與元素滾動邏輯 ===
    bool should_scroll = (chara && chara->state == MOVE && chara->dir && chara->x > WIDTH / 3.0);
    if (should_scroll) {
        gs->bg_offset_x += gs->scroll_speed;

        // 同步所有非角色元素向左位移
        for (int i = 0; i < allEle.len; i++) {
            Elements *ele = allEle.arr[i];
            if (ele->label == Character_L) continue;
            if (ele->label == Mushroom_L) {
                Mushroom *m = (Mushroom *)ele->pDerivedObj;
                m->x -= gs->scroll_speed;
                //printf("[MUSHROOM] x=%d width=%d\n", m->x, m->width);
                if (m->x <= 0) {
                    
                    ele->dele = true;
                }
            }
            else if (ele->label == Tree_L) {
                Tree *t = (Tree *)ele->pDerivedObj;
                t->x -= gs->scroll_speed;
            }
            else if (ele->label == Vine_L) {
                Vine *tp = (Vine *)ele->pDerivedObj;
                tp->x -= gs->scroll_speed;
            }
            else if (ele->label == Gardendoor_L) {
                Gardendoor *gd = (Gardendoor *)ele->pDerivedObj;
                gd->x -= gs->scroll_speed;
            }
        }
        
        // 背景移動時，讓新蘑菇在右邊生成
        if ((int)gs->bg_offset_x % 500 == 0) {
            int mush_count = 1 + rand() % 3;
            int placed = 0;
            while (placed < mush_count) {
                Elements *new_mush = New_Mushroom(Mushroom_L);
                Mushroom *mushA = (Mushroom *)new_mush->pDerivedObj;
                mushA->x += WIDTH;  // 新蘑菇放在畫面右側外一點

                bool too_close = false;
                ElementVec exist = _Get_all_elements(self);
                for (int j = 0; j < exist.len; j++) {
                    if (exist.arr[j]->label != Mushroom_L) continue;
                    Mushroom *mushB = (Mushroom *)exist.arr[j]->pDerivedObj;
                    float dx = mushA->x - mushB->x;
                    float dy = mushA->y - mushB->y;
                    float dist = sqrtf(dx * dx + dy * dy);
                    if (dist < 100) {
                        too_close = true;
                        break;
                    }
                }

                if (!too_close) {
                    _Register_elements(self, new_mush);
                    placed++;
                } else {
                    Mushroom_destroy(new_mush);
                }
            }
        }
        
        int bg_width = al_get_bitmap_width(gs->background);
        gs->bg_offset_x = fmod(gs->bg_offset_x, bg_width);
        if (gs->bg_offset_x < 0) gs->bg_offset_x += bg_width;
    }
    bool mouse_clicked = mouse_state[1]; 
    // 檢查鼠標點擊並處理蘑菇
    if (mouse_clicked) { // 左鍵點擊
        printf("Mouse clicked at (%.0f, %.0f)\n", mouse.x, mouse.y);  // 確認點擊位置
        for (int i = 0; i < allEle.len; i++) {
            Elements *ele = allEle.arr[i];
            if (ele->label == Mushroom_L) {
                Mushroom *mush = (Mushroom *)ele->pDerivedObj;
                // 假設 Mushroom 有 x, y, width, height 成員
                if (mouse.x >= mush->x && mouse.x <= mush->x + mush->width &&
                    mouse.y >= mush->y && mouse.y <= mush->y + mush->height) {
                    printf("Mushroom hit at (%d, %d), width: %d, height: %d\n", 
                            mush->x, mush->y, mush->width, mush->height); // 確認碰撞
                    
                    mush->was_clicked = true;
                    // 觸發主角採集動作
                    if (chara) {
                        chara->state = ATK;
                        chara->new_proj = false; // 重設投射物標記
                        chara->gif_status[ATK]->done = false; 
                        printf("Character state set to ATK\n"); // 確認狀態變化
                    }
                    
                    //挖土音效
                    play_dig_sound();
                    ele->dele = true; // 標記蘑菇刪除
                    printf("Mushroom marked for deletion\n"); // 確認刪除
                }
            }
        }
        
    }
    // 2. 花園 每 5 秒檢查一次精神值是否 < 75，如果是，生成花園入口
    if (chara && chara->status->SP < 75) {
        gs->garden_portal_timer += delta_time;
        if (!gs->garden_portal_spawned && gs->garden_portal_timer >= 5.0f) {
            printf("[GARDEN] Spirit < 75 -> Randomly spawning garden portal...\n");

            Elements *portal = New_Gardendoor(Gardendoor_L);
            Gardendoor *door = (Gardendoor *)(portal->pDerivedObj);
            door->active = true; 

            _Register_elements(self, portal);

            gs->garden_portal_spawned = true;
        }
    } else if (chara && chara->status->SP >= 75) {
        gs->garden_portal_timer = 0.0;
    }
    
    // 血量< 35 或剩下一分鐘自動切換到海洋場景
    double elapsed = al_get_time() - gs->start_time;
    int remaining = 180 - (int)elapsed;

    if (!gs->has_switched_to_sea && (chara->status->HP < 35 || remaining <= 60) && window != SeaScene_L) {
        gs->has_switched_to_sea = true;
        printf("[Trigger] HP = %d, Remaining Time = %d sec -> Switching to SEA Scene\n", chara->status->HP, remaining);
        stop_bgm();
        window = SeaScene_L;
        self->scene_end = true;
        return;
    }

    
    // 更新所有元素
    for (int i = 0; i < allEle.len; i++) {
        allEle.arr[i]->Update(allEle.arr[i], delta_time);
    }

    // 額外標記完成動畫的 Projectile 為 dele
    for (int i = 0; i < allEle.len; i++) {
        Elements *ele = allEle.arr[i];
        if (ele->label == Projectile_L) {
            Projectile *proj = (Projectile *)ele->pDerivedObj;
            if (proj->done) {
                ele->dele = true;
            }
        }
        else if(ele->label == Gardendoor_L){
            Gardendoor *door = (Gardendoor *)ele->pDerivedObj;
            if (!door->active) { 
                gs->garden_portal_spawned = false;
            }
        }
    }
    
    // run interact for every element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Interact(ele);
    }
    // 重置鼠標狀態，避免重複觸發
    mouse_state[1] = false;
    // remove element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        if (ele->dele)
         {
            _Remove_elements(self, ele);
        }
    }
}

void game_scene_draw(Scene *self)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    Character *chara = NULL;
    if (gs->background) {
        //int bg_width = al_get_bitmap_width(gs->background);
        int bg_height = al_get_bitmap_height(gs->background);
        // Draw the background with offset
        al_draw_bitmap_region(gs->background, 
                              (int)gs->bg_offset_x, 0, 
                              WIDTH, bg_height, 
                              0, 0, 0);
        // Draw additional section if offset is negative (looping)
        if (gs->bg_offset_x > 0) {
            al_draw_bitmap_region(gs->background, 
                                  0, 0, 
                                  (int)gs->bg_offset_x, bg_height, 
                                  WIDTH - (int)gs->bg_offset_x, 0, 0);
        }
        
    }
    ElementVec allEle = _Get_all_elements(self);
    // 先畫低層元素（例如蘑菇、樹）
    for (int i = 0; i < allEle.len; i++) {
        if (
            allEle.arr[i]->label == Mushroom_L || 
            allEle.arr[i]->label == Tree_L || 
            allEle.arr[i]->label == Vine_L ||
            allEle.arr[i]->label == Gardendoor_L
        )
            allEle.arr[i]->Draw(allEle.arr[i]);
    }

    // 接著畫主角
    for (int i = 0; i < allEle.len; i++) {
        if (allEle.arr[i]->label == Character_L){
            allEle.arr[i]->Draw(allEle.arr[i]);
            chara = (Character *)allEle.arr[i]->pDerivedObj;
            break;
        }
    }

    // 最後畫其他東西（像是投射物、Pause 按鈕等）
    for (int i = 0; i < allEle.len; i++) {
        int label = allEle.arr[i]->label;
        if (label != Character_L &&
             label != Mushroom_L && 
             label != Tree_L && 
             allEle.arr[i]->label != Vine_L &&
             allEle.arr[i]->label != Gardendoor_L
            )
            allEle.arr[i]->Draw(allEle.arr[i]);
    }
    // 繪製 status HUD
    Draw_HUD(chara, gs->font, WIDTH);
    //計時器
    double now = al_get_time();
    double elapsed = now - gs->start_time;
    int remaining = (180 - (int)elapsed); // 180秒

    if (remaining < 0) {
        // TODO: 轉場、暫停、顯示結束畫面
        remaining = 0;
    }

    int minutes = remaining / 60;
    int seconds = remaining % 60;

    char time_buf[32];
    sprintf(time_buf, "Time: %02d:%02d", minutes, seconds);

    al_draw_text(gs->font, al_map_rgb(255, 255, 255), WIDTH - 150, 110, 0, time_buf);
}
void game_scene_destroy(Scene *self)
{
    GameScene *Obj = ((GameScene *)(self->pDerivedObj));
    ALLEGRO_BITMAP *background = Obj->background;
    al_destroy_bitmap(background);
    //al_destroy_sample(Obj->song);
    //al_destroy_sample_instance(Obj->sample_instance);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Destroy(ele);
    }
    free(Obj);
    free(self);
}
