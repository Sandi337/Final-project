#include "gameoverscene.h"
#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include "../global.h"
#include "gamescene.h"
#include "../scene/sceneManager.h"

typedef struct _GameOverScene {
    ALLEGRO_BITMAP *image;
} GameOverScene;

Scene* New_GameOverScene(int label) {
    Scene *pObj = New_Scene(label);
    GameOverScene *data = (GameOverScene*)malloc(sizeof(GameOverScene));
    data->image = al_load_bitmap("assets/image/gameover.png");

    if (!data->image) {
        fprintf(stderr, "[ERROR] Failed to load gameover.png!\n");
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
    if (key_state[ALLEGRO_KEY_ENTER]) {
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
}

void gameover_scene_destroy(Scene *self) {
    GameOverScene *data = (GameOverScene*)self->pDerivedObj;
    al_destroy_bitmap(data->image);
    free(data);
    free(self);
}
