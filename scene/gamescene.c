#include <allegro5/allegro_audio.h>
#include "gamescene.h"
#include "../element/charater.h"
#include "../element/element.h"
#include "../element/mushroom.h"
#include "../element/teleport.h"
#include "../element/tree.h"
#include "../element/projectile.h"
#include "../element/Pause_button.h"
/*#include "../element/Continue.h"
#include "../element/Legend.h"*/

#define FPS 60
/*
   [GameScene function]
*/
Scene *New_GameScene(int label)
{
    GameScene *pDerivedObj = (GameScene *)malloc(sizeof(GameScene));
    Scene *pObj = New_Scene(label);
    // Load sound
    pDerivedObj->song = al_load_sample("assets/sound/main_music.mp3");
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    // Loop the song until the display closes
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
    // Start playing the sound
    al_play_sample_instance(pDerivedObj->sample_instance);
    // set the volume of instance
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.5);
    // setting derived object member
    pDerivedObj->background = al_load_bitmap("assets/image/main_scene.png");
    if (!pDerivedObj->background) {
        fprintf(stderr, "Failed to load background: assets/image/main_scene.png\n");
        exit(1);
    }
    // Initialize scroll variables
    pDerivedObj->bg_offset_x = 0.0;
    pDerivedObj->scroll_speed = 0.2; // 根據需求調整

    pObj->pDerivedObj = pDerivedObj;

    // 初始化隨機數生成器
    srand(time(NULL));

    // register element
    _Register_elements(pObj, New_Character(Character_L));
    _Register_elements(pObj, New_Teleport(Teleport_L));
    _Register_elements(pObj, New_Tree(Tree_L));
    _Register_elements(pObj, New_Mushroom(Mushroom_L));
    _Register_elements(pObj, New_Pause_button(Pause_button_L));
    /*_Register_elements(pObj, New_Continue(Continue_L));
    _Register_elements(pObj, New_Legend(Legend_L));*/

    // 隨機生成 1-3 個蘑菇
    int mushroom_count = 1 + (rand() % 3);
    for (int i = 0; i < mushroom_count; i++) {
        _Register_elements(pObj, New_Mushroom(MUSHROOM_L));
    }


    // setting derived object function
    pObj->Update = game_scene_update;
    pObj->Draw = game_scene_draw;
    pObj->Destroy = game_scene_destroy;
    return pObj;
}
void game_scene_update(Scene *self)
{
    // update every element
    ElementVec allEle = _Get_all_elements(self);
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    Character *chara = NULL;

    // Find the character
    for (int i = 0; i < allEle.len; i++) {
        if (allEle.arr[i]->label == Character_L) {
            chara = (Character *)allEle.arr[i]->pDerivedObj;
            break;
        }
    }

    if (chara && chara->state != STOP) {
        float threshold = WIDTH / 3.0;
        if (chara->x > threshold && chara->dir) {
            gs->bg_offset_x -= gs->scroll_speed;
        } else if (chara->x < threshold && !chara->dir) {
            gs->bg_offset_x += gs->scroll_speed;
        }

        int bg_width = al_get_bitmap_width(gs->background);
        gs->bg_offset_x = fmod(gs->bg_offset_x, bg_width);
        if (gs->bg_offset_x < 0) gs->bg_offset_x += bg_width;
    }

    // 檢查鼠標點擊並處理蘑菇
    float delta_time = 1.0 / FPS; // 假設 FPS 為 60
    if (mouse_state[1]) { // 左鍵點擊
        for (int i = 0; i < allEle.len; i++) {
            Elements *ele = allEle.arr[i];
            if (ele->label == Mushroom_L) {
                Mushroom *mush = (Mushroom *)ele->pDerivedObj;
                // 假設 Mushroom 有 x, y, width, height 成員
                if (mouse.x >= mush->x && mouse.x <= mush->x + mush->width &&
                    mouse.y >= mush->y && mouse.y <= mush->y + mush->height) {
                    // 觸發採集動作
                    // 觸發主角採集動作
                    if (chara) chara->state = ATK;
                    // 創建臨時投射物顯示挖洞 GIF
                    Elements *proj = New_Projectile(Projectile_L, mush->x, mush->y, 0); // 靜止投射物
                    if (proj) {
                        _Register_elements(self, proj);
                        proj->Update(proj/*, delta_time*/); // 立即更新一次
                    }
                    // 播放音效（假設音效已加載）
                    if (gs->sample_instance) {
                        al_set_sample_instance_position(gs->sample_instance, 0); // 重置音效位置
                        al_play_sample_instance(gs->sample_instance); // 播放採集音效
                    }
                    ele->dele = true; // 標記蘑菇刪除
                }
            }
        }
        // 重置鼠標狀態，避免重複觸發
        mouse_state[1] = false;
    }
    
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        if (ele->label != Projectile_L || !((Projectile *)ele->pDerivedObj)->done) {
            ele->Update(ele/*, delta_time*/);
        }
        //ele->Update(ele);

        Character *chara = NULL;
    }
    // 檢查投射物的 done 旗標
    /*if (ele->label == Projectile_L && ((Projectile *)ele->pDerivedObj)->done)
    {
        ele->dele = true; // 標記為刪除
    }*/
    // run interact for every element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Interact(ele);
    }
    // remove element
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        if (ele->dele)
            _Remove_elements(self, ele);
    }
}
void game_scene_draw(Scene *self)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
        
    if (gs->background) {
        int bg_width = al_get_bitmap_width(gs->background);
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
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Draw(ele);
    }
}
void game_scene_destroy(Scene *self)
{
    GameScene *Obj = ((GameScene *)(self->pDerivedObj));
    ALLEGRO_BITMAP *background = Obj->background;
    al_destroy_bitmap(background);
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Destroy(ele);
    }
    free(Obj);
    free(self);
}
