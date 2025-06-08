#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "menu.h"
#include <stdbool.h>
/*
   [Menu function]
*/
Scene *New_Menu(int label)
{
    Menu *pDerivedObj = (Menu *)malloc(sizeof(Menu));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->font = al_load_ttf_font("assets/font/ByteBounce.ttf", 35, 0);
    pDerivedObj->titlefont = al_load_ttf_font("assets/font/ByteBounce.ttf", 350, 0);
    
    // Load sound
    pDerivedObj->song = al_load_sample("assets/sound/menu.mp3");
    pDerivedObj->background = al_load_bitmap("assets/image/enter_scene.png");
    al_reserve_samples(20);
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    pDerivedObj->title_x = WIDTH / 2;
    pDerivedObj->title_y = HEIGHT / 2;
    // Loop the song until the display closes
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
    // set the volume of instance
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.5);
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Update = menu_update;
    pObj->Draw = menu_draw;
    pObj->Destroy = menu_destroy;
    return pObj;
}
void menu_update(Scene *self, float delta_time)
{
    if (key_state[ALLEGRO_KEY_ENTER])
    {
        self->scene_end = true;
        window = 1;
    }
    return;
}
void menu_draw(Scene *self)
{ 
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    Menu *gs = ((Menu *)(self->pDerivedObj));
    if (gs->background) {
        al_draw_scaled_bitmap(gs->background, 
                              0, 0, al_get_bitmap_width(gs->background), al_get_bitmap_height(gs->background),
                              0, 0, WIDTH, HEIGHT, 0);
    }
    al_draw_text(Obj->titlefont, al_map_rgb(255, 255, 255), Obj->title_x, Obj->title_y - 350, ALLEGRO_ALIGN_CENTRE, "ICHIGO");
    al_draw_text(Obj->font, al_map_rgb(255, 255, 255), Obj->title_x, Obj->title_y, ALLEGRO_ALIGN_CENTRE, "Press 'Enter' to start");
    al_draw_rectangle(Obj->title_x - 150, Obj->title_y - 30, Obj->title_x + 150, Obj->title_y + 30, al_map_rgb(255, 255, 255), 0);
    al_play_sample_instance(Obj->sample_instance);
}
void menu_destroy(Scene *self)
{
    Menu *Obj = ((Menu *)(self->pDerivedObj));
    al_destroy_font(Obj->font);
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    free(Obj);
    free(self);
}
