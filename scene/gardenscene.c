#include <allegro5/allegro_audio.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "gardenscene.h"
#include "../global.h"
#include "../element/element_label.h"
#include "../element/charater.h"
#include "../garden_element/Mousesign.h"
#include "../garden_element/Mushrooms_red.h"
#include "../garden_element/Mushrooms_blue.h"
#include "../garden_element/Mushrooms_green.h"
#include "../garden_element/Mushrooms_purple.h"
#include "../garden_element/button.h"
#include "../garden_element/Pause_button.h"
#include "../garden_element/Continue.h"
#include "../garden_element/Legend.h"

extern int window;
/*
   [GardenScene function]
*/

Scene* New_GardenScene(int label) {
    GardenScene *pDerivedObj = (GardenScene *)malloc(sizeof(GardenScene));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->song = al_load_sample("assets/sound/Garden_dance.mp3");
    pDerivedObj->background = al_load_bitmap("assets/image/MushroomForest2.jpg");
    if (!pDerivedObj->background) {
        fprintf(stderr, "Failed to load Garden background image!\n");
        exit(1);
    }
    al_reserve_samples(20);
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);

    // register element
    _Register_elements(pObj, New_Character(Character_L));
    _Register_elements(pObj, New_Mousesign(Mousesign_L));
    _Register_elements(pObj, New_Mushrooms_red(Mushrooms_red_L));
    _Register_elements(pObj, New_Mushrooms_blue(Mushrooms_blue_L));
    _Register_elements(pObj, New_Mushrooms_green(Mushrooms_green_L));
    _Register_elements(pObj, New_Mushrooms_purple(Mushrooms_purple_L));
    _Register_elements(pObj, New_Button(Button_L));
    _Register_elements(pObj, New_Pause_button(Pause_button_L));
    _Register_elements(pObj, New_Continue(Continue_L));
    _Register_elements(pObj, New_Legend(Legend_L));
    // Loop the song until the display closes
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.1);
    al_play_sample_instance(pDerivedObj->sample_instance);
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Update = garden_scene_update;
    pObj->Draw = garden_scene_draw;
    pObj->Destroy = garden_scene_destroy;
    return pObj;
}
void garden_scene_update(Scene *self, float delta_time)
{
    // update every element
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Update(ele, delta_time);
    }
    for (int i = 0; i < allEle.len; ++i) {
        if (allEle.arr[i]->label == Button_L) {        // 你的按鈕 label
            Button *btn = (Button *)allEle.arr[i]->pDerivedObj;
            if (btn->finished) {
                self->scene_end = true;
                window  = 1;
            }
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
void garden_scene_draw(Scene *self)
{
    al_clear_to_color(al_map_rgb(0, 0, 0));
    GardenScene *gs = ((GardenScene *)(self->pDerivedObj));
    //al_draw_bitmap(gs->background, 0, 0, 0);
    ElementVec allEle = _Get_all_elements(self);
    if (gs->background) {
        al_draw_scaled_bitmap(gs->background, 
                              0, 0, al_get_bitmap_width(gs->background), al_get_bitmap_height(gs->background),
                              0, 0, WIDTH, HEIGHT, 0);
    }
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Draw(ele);
    }
    //al_play_sample_instance(gs->sample_instance);

}
void garden_scene_destroy(Scene *self)
{
    GardenScene *Obj = ((GardenScene *)(self->pDerivedObj));
    ALLEGRO_BITMAP *background = Obj->background;
    al_destroy_bitmap(background);
    al_destroy_sample(Obj->song);
    ElementVec allEle = _Get_all_elements(self);
    for (int i = 0; i < allEle.len; i++)
    {
        Elements *ele = allEle.arr[i];
        ele->Destroy(ele);
    }
    free(Obj);
    free(self);
}