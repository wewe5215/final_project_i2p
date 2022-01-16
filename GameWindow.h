#ifndef MAINWINDOW_H_INCLUDED
#define MAINWINDOW_H_INCLUDED

#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <vector>
#include <list>
#include <time.h>
#include "Menu.h"
#include "Level.h"
#include "WolfKnight.h"
#include "CaveMan.h"
#include "Wolf.h"
#include "DemonNijia.h"
#include "Arcane.h"
#include "Archer.h"
#include "Canon.h"
#include "Poison.h"
#include "Storm.h"
#include "Attack.h"
#include "Slider.h"

#define GAME_INIT -1
#define GAME_SETTING 0
#define GAME_MENU 1
#define GAME_CONTINUE 2
#define GAME_TURNBACK 3
#define GAME_TERMINATE 4
#define GAME_EXIT 5
/*enum {
    SCENE_MENU = 1,
    SCENE_START = 2
    // [HACKATHON 3-7]
    // TODO: Declare a new scene id.
    , SCENE_SETTINGS = 3,SCENE_GAME_OVER=4,SCENE_WIN=5,SCENE_PALSE=6,SCENE_RANK=7,SCENE_INTRO=8
};/*
/* Input states */

// The active scene id.

// clock rate
const float FPS = 60;

// total number of level
const int LevelNum = 4;

// 1 coin every 2 seconds
const int CoinSpeed = FPS * 2;
const int Coin_Time_Gain = 1;

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
    Monster* create_monster();
// Load resized bitmap and check if failed.
    ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h);
// [HACKATHON 3-2]
// TODO: Declare a function.
// Determines whether the point (px, py) is in rect (x, y, w, h).
// Uncomment the code below.
bool pnt_in_rect(int px, int py, int x, int y, int w, int h);

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
    
    // Keyboard state, whether the key is down or not.
    bool key_state[ALLEGRO_KEY_MAX];
    // Mouse state, whether the key is down or not.
    // 1 is for left, 2 is for right, 3 is for middle.
    bool *mouse_state;
    // Mouse position.
    int mouse_x, mouse_y;
    enum {
    SCENE_MENU = 1,
    SCENE_START = 2
    // [HACKATHON 3-7]
    // TODO: Declare a new scene id.
    , SCENE_SETTINGS = 3,SCENE_GAME_OVER=4,SCENE_WIN=5,SCENE_PALSE=6,SCENE_RANK=7,SCENE_INTRO=8
};
private:
    ALLEGRO_BITMAP *icon;
    ALLEGRO_BITMAP *tower[Num_TowerType];
    ALLEGRO_BITMAP *background = NULL;
    ALLEGRO_BITMAP *menu_pic = NULL;
    ALLEGRO_BITMAP *settings_pic;
    ALLEGRO_BITMAP *start_button1;
    ALLEGRO_BITMAP *start_button2;
    ALLEGRO_BITMAP *role1;
    ALLEGRO_BITMAP *role2;
    ALLEGRO_BITMAP *role3;
    ALLEGRO_BITMAP *role4;
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
    ALLEGRO_TIMER *timer = NULL;
    ALLEGRO_TIMER *monster_pro = NULL;

    
    LEVEL *level = NULL;
    Menu *menu = NULL;

    std::vector<Monster*> monsterSet;
    std::list<Tower*> towerSet;

    int Monster_Pro_Count = 0;
    int Coin_Inc_Count = 0;
    int selectedTower = -1, lastClicked = -1;

    bool redraw = false;
    bool mute = false;
};


#endif // MAINWINDOW_H_INCLUDED