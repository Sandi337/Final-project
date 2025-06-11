// audio.c
#include "audio.h"
#include <allegro5/allegro_audio.h>
#include <stdio.h>

static ALLEGRO_SAMPLE *bgm = NULL;
static ALLEGRO_SAMPLE *garden_bgm = NULL;
static ALLEGRO_SAMPLE *SEA_bgm = NULL;
static ALLEGRO_SAMPLE_INSTANCE *bgm_instance = NULL;
static ALLEGRO_SAMPLE_INSTANCE *garden_bgm_instance = NULL;
static ALLEGRO_SAMPLE_INSTANCE *SEA_bgm_instance = NULL;
static ALLEGRO_SAMPLE *dig_sound = NULL;
static ALLEGRO_SAMPLE *eat_sound = NULL;
static ALLEGRO_SAMPLE *hurt_sound = NULL;

void AudioManager_Init() {
    bgm = al_load_sample("assets/sound/main_music.mp3");
    if (!bgm) {
        fprintf(stderr, "Failed to load BGM.\n");
    }
    bgm_instance = al_create_sample_instance(bgm);
    al_set_sample_instance_playmode(bgm_instance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(bgm_instance, al_get_default_mixer());
    
    //花園BGM
    garden_bgm = al_load_sample("assets/sound/Garden_dance.mp3");
    if (!garden_bgm) {
        fprintf(stderr, "Failed to load garden_bgm.\n");
    }
    garden_bgm_instance = al_create_sample_instance(garden_bgm);
    al_set_sample_instance_playmode(garden_bgm_instance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(garden_bgm_instance, al_get_default_mixer());

    //海洋BGM
    SEA_bgm = al_load_sample("assets/sound/SEA.mp3");
    if (!SEA_bgm) {
        fprintf(stderr, "Failed to load SEA_bgm.\n");
    }
    SEA_bgm_instance = al_create_sample_instance(SEA_bgm);
    al_set_sample_instance_playmode(SEA_bgm_instance, ALLEGRO_PLAYMODE_LOOP);
    al_attach_sample_instance_to_mixer(SEA_bgm_instance, al_get_default_mixer());
    
    dig_sound = al_load_sample("assets/sound/Shovelsound.mp3");
    if (!dig_sound) {
        fprintf(stderr, "Failed to load Shovelsound.mp3\n");
    }
    eat_sound = al_load_sample("assets/sound/eat_sound.wav");
    if (!eat_sound) {
        fprintf(stderr, "Failed to load eat_sound.wav\n");
    }
}
void play_hurt_sound() {
    if (!hurt_sound) {
        hurt_sound = al_load_sample("assets/sound/hit_2.wav");
        if (!hurt_sound) {
            fprintf(stderr, "Failed to load hit_2.wav\n");
            return;
        }
    }
    al_play_sample(hurt_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}
void play_dig_sound(void) {
    if (!dig_sound) {
        dig_sound = al_load_sample("assets/sound/Shovelsound.mp3");
        if (!dig_sound) {
            fprintf(stderr, "[AUDIO] Failed to load dig_sound.wav\n");
            return;
        }
    }
    al_play_sample(dig_sound, 1.0, 0.0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}
void AudioManager_Destroy() {
    if (bgm_instance) al_destroy_sample_instance(bgm_instance);
    if (bgm) al_destroy_sample(bgm);

    if (garden_bgm_instance) al_destroy_sample_instance(garden_bgm_instance); 
    if (garden_bgm) al_destroy_sample(garden_bgm);

    if (SEA_bgm_instance) al_destroy_sample_instance(SEA_bgm_instance); 
    if (SEA_bgm) al_destroy_sample(SEA_bgm);

    if (dig_sound) al_destroy_sample(dig_sound);
    if (eat_sound) al_destroy_sample(eat_sound);
}

void play_bgm() {
    if (garden_bgm_instance) al_stop_sample_instance(garden_bgm_instance);  
    if (bgm_instance) al_play_sample_instance(bgm_instance);
}

void play_garden_bgm() {
    if (bgm_instance) al_stop_sample_instance(bgm_instance);  // 停止主場音樂

    if (!garden_bgm)
        garden_bgm = al_load_sample("assets/sound/garden_music.mp3");
    
    if (!garden_bgm_instance) {
        garden_bgm_instance = al_create_sample_instance(garden_bgm);
        al_set_sample_instance_playmode(garden_bgm_instance, ALLEGRO_PLAYMODE_LOOP);
        al_attach_sample_instance_to_mixer(garden_bgm_instance, al_get_default_mixer());
    }

    al_play_sample_instance(garden_bgm_instance);
}

void play_SEA_bgm() {
    if (bgm_instance) al_stop_sample_instance(bgm_instance);  // 停止主場音樂
    if (garden_bgm_instance) al_stop_sample_instance(garden_bgm_instance);  // 停止花園音樂

    if (!SEA_bgm)
        SEA_bgm = al_load_sample("assets/sound/SEA.mp3");
    
    if (!SEA_bgm_instance) {
        SEA_bgm_instance = al_create_sample_instance(SEA_bgm);
        al_set_sample_instance_playmode(SEA_bgm_instance, ALLEGRO_PLAYMODE_LOOP);
        al_attach_sample_instance_to_mixer(SEA_bgm_instance, al_get_default_mixer());
    }

    al_play_sample_instance(SEA_bgm_instance);
}

void stop_bgm() {
    if (bgm_instance) al_stop_sample_instance(bgm_instance);
}

void stop_garden_bgm() {
    if (garden_bgm_instance)
        al_stop_sample_instance(garden_bgm_instance);
}

void stop_SEA_bgm() {
    if (SEA_bgm_instance)
        al_stop_sample_instance(SEA_bgm_instance);
}

void play_eat_sound() {
    if (eat_sound) al_play_sample(eat_sound, 1.0, 0, 1.0, ALLEGRO_PLAYMODE_ONCE, NULL);
}

