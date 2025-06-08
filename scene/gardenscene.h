#ifndef GARDENSCENE_H
#define GARDENSCENE_H

#include <allegro5/allegro_audio.h>
#include "../scene/scene.h"
#include "../element/charater.h"
#include "../element/element_label.h"

/*Scene* New_GardenScene(int label);

typedef enum EleType_garden
{
    Character_L,
    Pause_button_L,
    Mushrooms_red_L,
    Mushrooms_blue_L,
    Mushrooms_green_L,
    Mushrooms_purple_L,
    Button_L,
    Mousesign_L ,
    Continue_L,
    Legend_L
} EleType_garden;*/
typedef struct _GardenScene
{
    ALLEGRO_BITMAP *background;
    ALLEGRO_SAMPLE *song;
    ALLEGRO_SAMPLE_INSTANCE *sample_instance;

} GardenScene;
Scene *New_GardenScene(int label);
void garden_scene_update(Scene *self, float delta_time);
void garden_scene_draw(Scene *self);
void garden_scene_destroy(Scene *self);


#endif