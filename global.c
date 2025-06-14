#include "global.h"
#include "shapes/Shape.h"
#include <stdbool.h>
// variables for global usage
const double FPS = 60.0;
const int WIDTH = 1280;
const int HEIGHT = 720;
int window = 0;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT event;
ALLEGRO_TIMER *fps = NULL;
CharacterStatus g_status = {100, 100, 100}; // 初始值
bool key_state[ALLEGRO_KEY_MAX] = {false};
bool mouse_state[ALLEGRO_MOUSE_MAX_EXTRA_AXES] = {false};
Point mouse;
bool debug_mode = true;
bool is_paused = false;
bool legend = false;

//開發者模式
bool dev_mode = true;  // demo 開啟，正式版改成 false

bool is_invincible = false; //無敵
bool is_speed_up = false; //加速