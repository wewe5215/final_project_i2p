#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED
#include <stdlib.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <stdio.h>
#include <string.h>
#include <vector>
#include <list>
#include <time.h>
#include "Attack.h"
#include "Slider.h"

//#include "player.h"
#define GAME_INIT -1
#define GAME_SETTING 0
#define GAME_MENU 1
#define GAME_CONTINUE 2
#define GAME_TURNBACK 3
#define GAME_TERMINATE 4
#define GAME_EXIT 5
#define MAX_BULLET 8
#define MAX_ENEMY1 10
#define MAX_ENEMY2 10
#define MAX_ENEMY3 8

// The active scene id.

// clock rate
const float FPS = 30;

// total number of level
const int LevelNum = 4;
const float MAX_COOLDOWN = 0.2f;

// 1 coin every 2 seconds
const int CoinSpeed = FPS * 2;
const int Coin_Time_Gain = 1;

class MovableObject{
    public:
    // The center coordinate of the image.
    float x, y;
    // The width and height of the object.
    float w, h;
    // The velocity in x,
    float vx;
    int hp;
    int mp;
    float attack;
    float speed;
    float defence;
    int full_hp;
    int full_mp;
    int initx;
    int inity;
    bool hidden;
    ALLEGRO_BITMAP* img;
    ALLEGRO_BITMAP* img_tool;
} ;
class GameWindow
{
public:
    // constructor
    GameWindow();
    // each process of scene

// Initialize variables and resources.
// Allows the game to perform any initialization it needs before
// starting to run.
    void game_init(void);
// Process events inside the event queue using an infinity loop.
    void game_start_event_loop(void);
// Run game logic such as updating the world, checking for collision,
// switching scenes and so on.
// This is called when the game should update its logic.
    void game_update(void);
// Draw to display.
// This is called when the game should draw itself.
    void game_draw(void);
// Release resources.
// Free the pointers we allocated.
    void game_destroy(void);
// Function to change from one scene to another.
    void game_change_scene(int next_scene);
    ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);
bool pnt_in_rect(int px, int py, int x, int y, int w, int h);
void draw_movable_object(MovableObject obj);
/* Event callbacks. */
void on_key_down(int keycode);
void on_mouse_down(int btn, int x, int y);
void show_err_msg(int msg);
void game_reset();
void game_begin();
void draw_running_map();
public:
    bool initial = true;
    int active_scene;
    int role = 1;
    int score;
    double last_shoot_timestamp_player = 0;
    //double last_shoot_timestamp_enemy;
    //double last_shoot_timestamp_flighter;
    //double last_shoot_timestamp_monster1;
    //double last_shoot_timestamp_monster2;
    // Keyboard state, whether the key is down or not.
    bool key_state[ALLEGRO_KEY_MAX];
    // Mouse state, whether the key is down or not.
    // 1 is for left, 2 is for right, 3 is for middle.
    bool *mouse_state;
    int plus_hp;
    int plus_mp;
    // Mouse position.
    int mouse_x, mouse_y;
    MovableObject player;
    MovableObject player_attack[MAX_BULLET];
    MovableObject enemy1[MAX_ENEMY1];
    MovableObject enemy2[MAX_ENEMY2];
    MovableObject enemy3[MAX_ENEMY3];
    MovableObject boss;
    //MovableObject* player;
    enum {
    SCENE_MENU = 1,
    SCENE_START = 2
    // [HACKATHON 3-7]
    // TODO: Declare a new scene id.
    , SCENE_SETTINGS = 3,SCENE_INTRO=4, SCENE_HOME = 5, SCENE_LEVEL2 = 6, SCENE_LEVEL3 = 7, SCENE_BOSS = 8
};
private:
    ALLEGRO_BITMAP *icon;
    ALLEGRO_BITMAP *background = NULL;
    ALLEGRO_BITMAP *menu_pic = NULL;
    ALLEGRO_BITMAP *settings_pic;
    ALLEGRO_BITMAP *role1;
    ALLEGRO_BITMAP *role2;
    ALLEGRO_BITMAP *role3;
    ALLEGRO_BITMAP *role4;
    ALLEGRO_BITMAP *role1_tool;
    ALLEGRO_BITMAP *role2_tool;
    ALLEGRO_BITMAP *role3_tool;
    ALLEGRO_BITMAP *role4_tool;
    ALLEGRO_BITMAP *home_pic;
    ALLEGRO_BITMAP *enemy1_pic;
    ALLEGRO_BITMAP *enemy2_pic;
    ALLEGRO_BITMAP *enemy3_pic;
    ALLEGRO_BITMAP *boss_pic;
    ALLEGRO_BITMAP *intro_pic;
    ALLEGRO_DISPLAY* display = NULL;
    ALLEGRO_FONT *font = NULL;
    ALLEGRO_FONT *Medium_font = NULL;
    ALLEGRO_FONT *Large_font = NULL;
    ALLEGRO_DISPLAY* game_display;
    ALLEGRO_EVENT_QUEUE* event_queue;
    ALLEGRO_SAMPLE *sample = NULL;
    ALLEGRO_SAMPLE_INSTANCE *startSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *clearSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *failSound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *backgroundSound = NULL;
    ALLEGRO_SAMPLE *menu_sound = NULL;
    ALLEGRO_SAMPLE_INSTANCE *menuSound;
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_TIMER *monster_pro = NULL;
    int emerald_killed = 0;
    int xuejila_killed = 0;
    int ribbonpig_killed = 0;
    int boss_killed = 0;
    int monster_killed = 0;
    int boss_defeated = 0;
    int monster_spawn = 0;
    int heal = 0;
    int invincible = 0;
    int revive = 0;
    bool redraw = false;
    bool mute = false;
};


#endif // MAINWINDOW_H_INCLUDED