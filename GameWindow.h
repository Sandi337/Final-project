#ifndef GAMEWINDOW_H_INCLUDED
#define GAMEWINDOW_H_INCLUDED
#include <allegro5/allegro.h>
#include <stdbool.h>

typedef struct _GAME Game;
typedef void (*fptrGameExecute)(Game *);
typedef void (*fptrGameInit)(Game *);
typedef bool (*fptrGameUpdate)(Game *, float);
typedef void (*fptrGameDraw)(Game *);
typedef void (*fptrGameDestroy)(Game *);
void execute(Game *);
void game_init(Game *);
bool game_update(Game *, float delta_time);
void game_draw(Game *);
void game_destroy(Game *);
struct _GAME
{
    const char *title;
    // ALLEGRO Variables
    ALLEGRO_DISPLAY *display;
    int current_scene;
    ALLEGRO_MOUSE_CURSOR *custom_cursor; // 自定義鼠標（可選）
    fptrGameExecute execute;
    fptrGameInit game_init;
    fptrGameUpdate game_update;
    fptrGameDraw game_draw;
    fptrGameDestroy game_destroy;
};
Game *New_Game();

#endif
