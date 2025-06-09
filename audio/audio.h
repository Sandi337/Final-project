#ifndef AUDIO_H
#define AUDIO_H

void AudioManager_Init();
void AudioManager_Destroy();

void play_bgm();
void play_garden_bgm();
void stop_bgm();
void play_dig_sound();
void play_eat_sound();
void play_hurt_sound();

#endif
