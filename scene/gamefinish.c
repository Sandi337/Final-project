#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "gamefinish.h"
#include <stdbool.h>
/*
   [gamefinish function]
*/
extern int final_score_global;

Scene *New_gamefinish(int label)
{
    gamefinish *pDerivedObj = (gamefinish *)malloc(sizeof(gamefinish));
    Scene *pObj = New_Scene(label);
    // setting derived object member
    pDerivedObj->font = al_load_ttf_font("assets/font/pirulen.ttf", 36, 0);
    // Load sound
    pDerivedObj->song = al_load_sample("assets/sound/menu.mp3");
    al_reserve_samples(20);
    pDerivedObj->sample_instance = al_create_sample_instance(pDerivedObj->song);
    pDerivedObj->title_x = WIDTH / 2;
    pDerivedObj->title_y = HEIGHT / 2-40;
    // Loop the song until the display closes
    al_set_sample_instance_playmode(pDerivedObj->sample_instance, ALLEGRO_PLAYMODE_LOOP);
    al_restore_default_mixer();
    al_attach_sample_instance_to_mixer(pDerivedObj->sample_instance, al_get_default_mixer());
    pDerivedObj->final_score  = final_score_global;
    pDerivedObj->music_played = false;
    // set the volume of instance
    al_set_sample_instance_gain(pDerivedObj->sample_instance, 0.1);
    pObj->pDerivedObj = pDerivedObj;
    // setting derived object function
    pObj->Update = gamefinish_update;
    pObj->Draw = gamefinish_draw;
    pObj->Destroy = gamefinish_destroy;
    return pObj;
}
void gamefinish_update(Scene *self, float delta_time)
{
    if (key_state[ALLEGRO_KEY_ENTER])
    {
        self->scene_end = true;
        window = 0; //可以改成回主場景
    }
    return;
}
void gamefinish_draw(Scene *self)
{
    gamefinish *gf = ((gamefinish *)(self->pDerivedObj));
    /* 背景可直接填黑 */
    al_clear_to_color(al_map_rgb(0,0,0));

    /* 顯示 Final Score */
    char buf[64];
    sprintf(buf, "Final Score: %d", gf->final_score);
    al_draw_text(gf->font, al_map_rgb(255,255,0),
                 WIDTH/2, HEIGHT/2 - 40, ALLEGRO_ALIGN_CENTER, buf);

    /* 提示重新開始 */
    al_draw_text(gf->font, al_map_rgb(255,255,255),
                 WIDTH/2, HEIGHT/2 + 20, ALLEGRO_ALIGN_CENTER,
                 "Press <Enter> to return to menu");
}
void gamefinish_destroy(Scene *self)
{
    gamefinish *Obj = ((gamefinish *)(self->pDerivedObj));
    al_destroy_font(Obj->font);
    al_destroy_sample(Obj->song);
    al_destroy_sample_instance(Obj->sample_instance);
    free(Obj);
    free(self);
}
