#include <allegro5/allegro_audio.h>
#include "gamescene.h"
#include "../element/element.h"
#include "../element/charater.h"
#include "../element/mushroom.h"
#include "../element/teleport.h"
#include "../element/tree.h"
#include "../element/projectile.h"
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
    pObj->pDerivedObj = pDerivedObj;

    // 初始化隨機數生成器
    srand(time(NULL));

    // register element
    _Register_elements(pObj, New_Character(Character_L));
    _Register_elements(pObj, New_Teleport(Teleport_L));
    _Register_elements(pObj, New_Tree(Tree_L));
    _Register_elements(pObj, New_Mushroom(Mushroom_L));

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
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Update(ele);
        // 檢查投射物的 done 旗標
        if (ele->label == Projectile_L && ((Projectile *)ele->pDerivedObj)->done)
        {
            ele->dele = true; // 標記為刪除
        }
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
void game_scene_draw(Scene *self)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    GameScene *gs = ((GameScene *)(self->pDerivedObj));
    al_draw_bitmap(gs->background, 0, 0, 0);
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
