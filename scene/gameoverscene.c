#include "gameoverscene.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include "../global.h"
#include "gamescene.h"
#include "../scene/sceneManager.h"

Scene* New_GameOverScene(int label, CharacterStatus *status) {
    Scene *pObj = New_Scene(label, status);
    GameOverScene *data = (GameOverScene*)malloc(sizeof(GameOverScene));
    
    data->image = al_load_bitmap("assets/image/gameover.png");
    data->font = al_load_ttf_font("assets/font/ByteBounce.ttf", 35, 0);

    if (!data->font) {
    fprintf(stderr, "[ERROR] Failed to load font: assets/font/ByteBounce.ttf\n");
    exit(1);
    }
    pObj->pDerivedObj = data;
    pObj->Update = gameover_scene_update;
    pObj->Draw = gameover_scene_draw;
    pObj->Destroy = gameover_scene_destroy;

    return pObj;
}

void gameover_scene_update(Scene *self, float delta_time) {
    (void)delta_time;

    CharacterStatus *status = self->status; 
    
    if (key_state[ALLEGRO_KEY_ENTER]) {
        status->HP = 100;
        status->EN = 100;
        status->SP = 100; // 或你想要的起始值
        self->scene_end = true;
        window = Menu_L;
    }
}

void gameover_scene_draw(Scene *self) {
    GameOverScene *data = (GameOverScene*)self->pDerivedObj;
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_draw_scaled_bitmap(data->image,
                          0, 0,
                          al_get_bitmap_width(data->image),
                          al_get_bitmap_height(data->image),
                          0, 0,
                          WIDTH, HEIGHT,
                          0);
    if (data->font) {
        al_draw_text(data->font, al_map_rgb(255, 255, 255), WIDTH / 2, HEIGHT - 100,
                 ALLEGRO_ALIGN_CENTER, "Press ENTER to restart");
    } else {
        al_draw_text(al_create_builtin_font(), al_map_rgb(255, 0, 0), WIDTH / 2, HEIGHT - 100,
                 ALLEGRO_ALIGN_CENTER, "Font Missing!");
    }

}

void gameover_scene_destroy(Scene *self) {
    GameOverScene *data = (GameOverScene*)self->pDerivedObj;
    al_destroy_bitmap(data->image);
    free(data);
    free(self);
}
