#include <allegro5/allegro_audio.h>
#include "../audio/audio.h"
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include "../scene/seascene.h"
#include "../element/element_label.h"
//#include "start.h"
#include <stdio.h>
#include <stdbool.h>
#include "../sea_element/charater_sea.h"
#include "../sea_element/spirit.h"
#include "../scene/sceneManager.h" // for scene variable
#include "../algif5/algif.h"
/*
   [GameScene function]
*/
Scene *New_SEAScene(int label, CharacterStatus *status)
{
    printf("[DEBUG] Entering New_SEAScene, label=%d\n", label);
    if (!status) {
        printf("[ERROR] status is NULL inside New_SEAScene\n");
    } else {
        printf("[DEBUG] status->HP = %d\n", status->HP);
    }
    SEAScene *pDerivedObj = (SEAScene *)malloc(sizeof(SEAScene));
    Scene *pObj = New_Scene(label,status);
    // setting derived object member
    // set back ground
    pDerivedObj->background = al_load_bitmap("assets/image/sea.jpg");
    pObj->pDerivedObj = pDerivedObj;
    pDerivedObj->timer = al_get_time();

    play_SEA_bgm();

    /*static double last_enter_time = 0;
    if (pDerivedObj->timer - last_enter_time < 1.0) {
        printf("[DEBUG] 阻止重複進入 New_SEAScene()，間隔%.2f 秒\n", pDerivedObj->timer - last_enter_time);
        return NULL;
    }
    last_enter_time = pDerivedObj->timer;*/

    // register element
    _Register_elements(pObj, New_Character_SEA(Character_Sea_L, status));
    printf("[DEBUG] Sea character registered\n");
    _Register_elements(pObj, New_Spirit(Spirit_L));
    // setting derived object function
    pObj->Update = SEA_scene_update;
    pObj->Draw = SEA_scene_draw;
    pObj->Destroy = SEA_scene_destroy;
    return pObj;
}
void SEA_scene_update(Scene *self, float delta_time)
{
    // update every element
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Update(ele,  delta_time);
    }
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
void SEA_scene_draw(Scene *self)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    SEAScene *gs = ((SEAScene *)(self->pDerivedObj));

    // 場景定時30s，超過則切換結束場景
    double curr = al_get_time();
    if ((curr - gs->timer) > 30)
    {   
        self->scene_end = true;
        window = GameclearScene_L; // 這邊需要改結束場景的編號
    }
    // 當精神值<=0時，gameover切換結束場景
    ElementVec labelEle = _Get_label_elements(scene, Character_Sea_L);
    for (int i = 0; i < labelEle.len; i++)
    {
        Character_SEA *chara = (Character_SEA *)(labelEle.arr[i]->pDerivedObj);
        if (chara ->spirit <= 0){
            self->scene_end = true;
            window = GameOverScene_L; // 改結束場景的編號
        }
    }
    
    if (gs->background) {
        al_draw_scaled_bitmap(gs->background, 
                              0, 0, al_get_bitmap_width(gs->background), al_get_bitmap_height(gs->background),
                              0, 0, WIDTH, HEIGHT, 0);
    }
    
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Draw(ele);
    }
}
void SEA_scene_destroy(Scene *self)
{
    SEAScene *Obj = ((SEAScene *)(self->pDerivedObj));

    ALLEGRO_BITMAP *background = Obj->background;
    al_destroy_bitmap(background);
    //algif_destroy_animation(Obj->gif_background);

    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Destroy(ele);
    }
    free(Obj);
    free(self);
}
