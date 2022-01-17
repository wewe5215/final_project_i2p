#include "GameWindow.h"
#include "global.h"
#include <iostream>

#define WHITE al_map_rgb(255, 255, 255)
#define BLACK al_map_rgb(0, 0, 0)
#define ORANGE_LIGHT al_map_rgb(255, 196, 87)
#define ORANGE_DARK al_map_rgb(255, 142, 71)
#define PURPLE al_map_rgb(149, 128, 255)
#define BLUE al_map_rgb(77, 129, 179)

#define min(a, b) ((a) < (b)? (a) : (b))
#define max(a, b) ((a) > (b)? (a) : (b))
float Attack::volume = 1.0;

void set_attack_volume(float volume)
{
    Attack::volume = volume;
}

bool compare(Tower *t1, Tower *t2)
{
    return (t1->getY() <= t2->getY());
}
GameWindow::GameWindow()
{
    if (!al_init())
        show_err_msg(-1);

    printf("Game Initializing...\n");

    display = al_create_display(window_width, window_height);
    event_queue = al_create_event_queue();

    game_update_timer = al_create_timer(1.0f / FPS);
    timer = al_create_timer(1.0f/ FPS);
    monster_pro = al_create_timer(1.0 / FPS);

    if(timer == NULL || monster_pro == NULL)
        show_err_msg(-1);

    if (display == NULL || event_queue == NULL)
        show_err_msg(-1);

    al_init_primitives_addon();
    al_init_font_addon(); // initialize the font addon
    al_init_ttf_addon(); // initialize the ttf (True Type Font) addon
    al_init_image_addon(); // initialize the image addon
    al_init_acodec_addon(); // initialize acodec addon

    al_install_keyboard(); // install keyboard event
    al_install_mouse();    // install mouse event
    al_install_audio();    // install audio event

    font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",12,0); // load small font
    Medium_font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",24,0); //load medium font
    Large_font = al_load_ttf_font("Caviar_Dreams_Bold.ttf",36,0); //load large font

    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_timer_event_source(game_update_timer));
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_timer_event_source(monster_pro));

    game_init();
    al_start_timer(timer);
    al_start_timer(game_update_timer);
    al_start_timer(monster_pro);
}

void GameWindow::game_init(void) {
    char buffer[50];

    icon = al_load_bitmap("./icon.png");
    background = al_load_bitmap("./StartBackground.png");
    menu_pic = al_load_bitmap("./menu.png");
    settings_pic = al_load_bitmap("./setting.jpg");
    role1 = al_load_bitmap("./role1.png");
    role2 = al_load_bitmap("./role2.png");
    role3 = al_load_bitmap("./role3.png");
    role4 = al_load_bitmap("./role4.png");
    role1_tool = al_load_bitmap("./role1_tool.png");
    role2_tool = al_load_bitmap("./role2_tool.png");
    role3_tool = al_load_bitmap("./role3_tool.png");
    role4_tool = al_load_bitmap("./role4_tool.png");
    home_pic = al_load_bitmap("./home.png");
    enemy1_pic = al_load_bitmap("./enemy1.png");
    enemy2_pic = al_load_bitmap("./enemy2.png");
    enemy3_pic = al_load_bitmap("./enemy3.png");
    /*for(int i = 0; i < Num_TowerType; i++)
    {
        sprintf(buffer, "./Tower/%s.png", TowerClass[i]);
        tower[i] = al_load_bitmap(buffer);
    }*/

    al_set_display_icon(display, icon);
    al_reserve_samples(3);

    sample = al_load_sample("growl.wav");
    startSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(startSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(startSound, al_get_default_mixer());

    sample = al_load_sample("BackgroundMusic.ogg");
    backgroundSound = al_create_sample_instance(sample);
    al_set_sample_instance_playmode(backgroundSound, ALLEGRO_PLAYMODE_ONCE);
    al_attach_sample_instance_to_mixer(backgroundSound, al_get_default_mixer());
    //level = new LEVEL(1);
    //menu = new Menu();
    game_reset();
    active_scene = SCENE_MENU;
}

void GameWindow::game_start_event_loop(void) {
    bool done = false;
    ALLEGRO_EVENT event;
    int redraws = 0;
    while (!done) {
        al_wait_for_event(event_queue, &event);
        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE) {

            done = true;
        } else if (event.type == ALLEGRO_EVENT_TIMER) {
            // Event for redrawing the display.
            if (event.timer.source == game_update_timer)
                // The redraw timer has ticked.
                redraws++;
        } else if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
            // Event for keyboard key down.

            key_state[event.keyboard.keycode] = true;
            on_key_down(event.keyboard.keycode);
        } else if (event.type == ALLEGRO_EVENT_KEY_UP) {
            // Event for keyboard key up.

            key_state[event.keyboard.keycode] = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN) {
            // Event for mouse key down.

            mouse_state[event.mouse.button] = true;
            on_mouse_down(event.mouse.button, event.mouse.x, event.mouse.y);
        } else if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_UP) {
            // Event for mouse key up.

            mouse_state[event.mouse.button] = false;
        } else if (event.type == ALLEGRO_EVENT_MOUSE_AXES) {
            if (event.mouse.dx != 0 || event.mouse.dy != 0) {
                // Event for mouse move.
                // game_log("Mouse move to (%d, %d)", event.mouse.x, event.mouse.y);
                mouse_x = event.mouse.x;
                mouse_y = event.mouse.y;
            } else if (event.mouse.dz != 0) {
                // Event for mouse scroll.

            }
        }
        // TODO: Process more events and call callbacks by adding more
        // entries inside Scene.

        // Redraw
        if (redraws > 0 && al_is_event_queue_empty(event_queue)) {
            // Update and draw the next frame.
            game_update();
            game_draw();
            redraws = 0;
        }
    }
}

void GameWindow::game_update(void) {
    if (active_scene == SCENE_START) {
        unsigned int i, j;
    std::list<Tower*>::iterator it;
       char buffer[100];
        player.vx = 0;

        if (key_state[ALLEGRO_KEY_LEFT])
            player.vx -= 1;
        if (key_state[ALLEGRO_KEY_RIGHT])
            player.vx += 1;
        // 0.71 is (1/sqrt(2)).
        //player.y += player.vy * 4 * (player.vx ? 0.71f : 1);
        player.x += player.vx * 4 * 1;
        // [HACKATHON 1-1]
        // TODO: Limit the plane's collision box inside the frame.
        //       (x, y axes can be separated.)
        // Uncomment and fill in the code below.
        if (player.x-0-player.w/2 < 0)
            player.x = (player.w)/2;
        else if (player.x + player.w/2 > window_width)
            player.x = window_width-(player.w)/2;
        for(int i = 0; i < MAX_ENEMY1; i++){
            if(enemy1[i].hidden){
				enemy1[i].hidden=false;
			}
				else{
				enemy1[i].x += enemy1[i].vx * ( i % 2 ? 1 : -1);
				if(enemy1[i].x < 0 || enemy1[i].x > window_width)
					enemy1[i].vx *= -1;
				}
        }
         for(int i = 0; i < MAX_ENEMY2; i++){
            if(enemy2[i].hidden){
				enemy2[i].hidden=false;
			}
				else{
				enemy2[i].x += enemy2[i].vx * ( i % 2 ? 1 : -1);
				if(enemy2[i].x < 0 || enemy2[i].x > window_width)
					enemy2[i].vx *= -1;
				}
        }
         for(int i = 0; i < MAX_ENEMY3; i++){
            if(enemy3[i].hidden){
				enemy3[i].hidden=false;
			}
				else{
				enemy3[i].x += enemy3[i].vx * ( i % 2 ? 1 : -1);
				if(enemy3[i].x < 0 || enemy3[i].x > window_width)
					enemy3[i].vx *= -1;
				}
        }
        for (int i=0;i<MAX_BULLET;i++)
        {
            if (player_attack[i].hidden==true)
                continue;
            player_attack[i].x += player_attack[i].vx;

            if (player_attack[i].x< 0||player_attack[i].y<0)
                player_attack[i].hidden = true;
        }
        double now1 = al_get_time();
        if ((key_state[ALLEGRO_KEY_R]||key_state[ALLEGRO_KEY_L] )&& now1 - last_shoot_timestamp_player >= MAX_COOLDOWN) {
            for (i = 0; i<MAX_BULLET;i++) {
               if (player_attack[i].hidden==true) {
                    last_shoot_timestamp_player = now1;
                    player_attack[i].hidden = false;
                    if(key_state[ALLEGRO_KEY_R])
                        player_attack[i].x = player.x + player.w/2;
                    else
                        player_attack[i].x = player.x - player.w/2;
                    player_attack[i].y = player.y;

                    break;
                }
            }
        }
        
        for(int i=0;i<MAX_BULLET;i++)
        {
            for(int j=0;j<MAX_ENEMY1;j++)
            {
                if(player_attack[i].hidden || enemy1[j].hidden) continue;
                if((player_attack[i].y-(player_attack[j].h)/2<=enemy1[j].y+(enemy1[j].h)/2
                    &&player_attack[i].y+(player_attack[j].h)/2>=enemy1[j].y-(enemy1[j].h)/2)
                   &&(player_attack[i].x<=enemy1[j].x+enemy1[j].w/2
                      &&player_attack[i].x>=enemy1[j].x-enemy1[j].w/2))
               {
                   enemy1[j].hp-=player_attack[i].attack;

                   score+=player_attack[i].attack;
                   if(enemy1[j].hp<=0)
                   {
                       enemy1[j].hidden=true;
                        plus_hp++;
                        plus_mp++;

                   }
                   player_attack[i].hidden=true;
               }
            }
        }
                for(i = 0; i < MAX_ENEMY1; i++){
			if(enemy1[i].hidden)
        		continue;
        	if( enemy1[i].x - enemy1[i].w / 2 <= player.x + player.w / 2 && enemy1[i].x + enemy1[i].w/3 >= player.x - player.w / 2){
        		if( enemy1[i].y - enemy1[i].h / 2<= player.y + player.h / 2 && enemy1[i].y + enemy1[i].h / 2 >= player.y - player.h / 2){
        			enemy1[i].hidden = true;
                    plus_hp++;
                    plus_mp++;
					if(player.defence - enemy1[i].attack <= 0){
                        player.hp -= (enemy1[i].attack - player.defence);
        			}
        			else{
                        player.defence -= enemy1[i].attack;
        			}
				}
			}
		}
		for(i = 0; i < MAX_ENEMY2; i++){
			if(enemy2[i].hidden)
        		continue;
        	if( enemy2[i].x - enemy2[i].w / 2 <= player.x + player.w / 2 && enemy2[i].x + enemy2[i].w/3 >= player.x - player.w / 2){
        		if( enemy2[i].y - enemy2[i].h / 2<= player.y + player.h / 2 && enemy2[i].y + enemy2[i].h / 2 >= player.y - player.h / 2){
        			enemy2[i].hidden = true;
                    plus_hp++;
                    plus_mp++;
        			if(player.defence - enemy2[i].attack <= 0){
                        player.hp -= (enemy2[i].attack - player.defence);
        			}
        			else{
                        player.defence -= enemy2[i].attack;
        			}
				}
			}
		}
		for(i = 0; i < MAX_ENEMY3; i++){
			if(enemy3[i].hidden)
        		continue;
        	if( enemy3[i].x - enemy3[i].w / 2 <= player.x + player.w / 2 && enemy3[i].x + enemy3[i].w/3 >= player.x - player.w / 2){
        		if( enemy3[i].y - enemy3[i].h / 2<= player.y + player.h / 2 && enemy3[i].y + enemy3[i].h / 2 >= player.y - player.h / 2){
        			enemy3[i].hidden = true;
                    plus_hp++;
                    plus_mp++;
					if(player.defence - enemy2[i].attack <= 0){
                        player.hp -= (enemy3[i].attack - player.defence);
        			}
        			else{
                        player.defence -= enemy3[i].attack;
        			}
				}
			}
		}
        if(plus_hp % 3 == 0)
            player.hp += 5;
        if(plus_mp % 3 == 0)
            player.mp += 5;
        if(player.hp <= 0){
            player.hp = player.full_hp;
            game_change_scene(SCENE_HOME);

        }


    }
    else if(active_scene == SCENE_HOME){
        player.vx = 0;

        if (key_state[ALLEGRO_KEY_LEFT])
            player.vx -= 1;
        if (key_state[ALLEGRO_KEY_RIGHT])
            player.vx += 1;
        // 0.71 is (1/sqrt(2)).
        //player.y += player.vy * 4 * (player.vx ? 0.71f : 1);
        player.x += player.vx * 4 * 1;
        // [HACKATHON 1-1]
        // TODO: Limit the plane's collision box inside the frame.
        //       (x, y axes can be separated.)
        // Uncomment and fill in the code below.
        if (player.x-0-player.w/2 < 0)
            player.x = (player.w)/2;
        else if (player.x + player.w/2 > field_width)
            player.x = field_width-(player.w)/2;
    }
}

void GameWindow::game_draw(void) {
    const int gapX = 40, gapY = 30;
    const int offsetX = 30 + field_width, offsetY = 150;
    const int Initial_Health = 1;
    char buffer[100];
    if (active_scene == SCENE_MENU) {
        al_draw_bitmap(menu_pic, 0, 0, 0);
        sprintf(buffer, "PRESS ENTER TO START");
        al_draw_text(Medium_font, al_map_rgb(255, 255, 255), 20, 20 + gapY, 0, buffer);

        sprintf(buffer, "PRESS S TO SETTING");
        al_draw_text(Medium_font, al_map_rgb(255, 255, 255), 20, 20 + 2*gapY, 0, buffer);
    }
//
    else if (active_scene == SCENE_START) {

        //al_clear_to_color(al_map_rgb(100, 100, 100));
        al_draw_bitmap(home_pic, 0, 0, 0);

        //al_play_sample_instance(startSound);
        //while(al_get_sample_instance_playing(startSound));
          //  al_play_sample_instance(backgroundSound);
        

        for(int i = 0;i < MAX_BULLET;i++){
            player_attack[i].img = player.img_tool;
            draw_movable_object(player_attack[i]);
        }
        draw_movable_object(player);
        for(int i = 0; i < MAX_ENEMY1; i++){
            draw_movable_object(enemy1[i]);
        }
        for(int i = 0; i < MAX_ENEMY2; i++){
            draw_movable_object(enemy2[i]);
        }
         for(int i = 0; i < MAX_ENEMY3; i++){
            draw_movable_object(enemy3[i]);
        }
        /*for(int i=0; i<monsterSet.size(); i++)
        {
            monsterSet[i]->Draw();
        }*/
        sprintf(buffer, "hp : %d",player.hp);
                //printf("%d\n",player.hp);
                //printf("%d\n",player.mp);
        al_draw_text(Medium_font, al_map_rgb(255, 255, 255), 200, 100, 0, buffer);
        sprintf(buffer, "mp : %d",player.mp);
        al_draw_text(Medium_font, al_map_rgb(255, 255, 255), 300, 100, 0, buffer);

    /*for(std::list<Tower*>::iterator it = towerSet.begin(); it != towerSet.end(); it++)
        (*it)->Draw();
    if(selectedTower != -1)
        Tower::SelectedTower(mouse_x, mouse_y, selectedTower);
    al_draw_filled_rectangle(field_width, 0, window_width, window_height, al_map_rgb(100, 100, 100));*/

    //menu->Draw();
    }
    else if(active_scene == SCENE_SETTINGS){
        al_draw_bitmap(settings_pic, 0, 0, 0);
        al_draw_bitmap(role1, 0, 30, 0);
        al_draw_bitmap(role2, 150, 30, 0);
        al_draw_bitmap(role3, 300, 30, 0);
        al_draw_bitmap(role4, 450, 30, 0);
        sprintf(buffer, "role1           role2           role3           role4");
        al_draw_text(Medium_font, al_map_rgb(255, 255, 255), 100, 300, 0, buffer);
        sprintf(buffer, "PRESS 1 2 3 4 TO CHOOSE ROLE");
        al_draw_text(Medium_font, al_map_rgb(255, 255, 255), 100, 350, 0, buffer);
    }
    else if(active_scene == SCENE_HOME){
        al_draw_bitmap(home_pic, 0, 0, 0);
        al_draw_bitmap(player.img, player.x, player.y, 0);
        sprintf(buffer, "hp : %d",player.hp);
        al_draw_text(Medium_font, al_map_rgb(255, 255, 255), 200, 740, 0, buffer);
        sprintf(buffer, "mp : %d",player.mp);
        al_draw_text(Medium_font, al_map_rgb(255, 255, 255), 300, 740, 0, buffer);
    }
    else if(active_scene == SCENE_INTRO){

    }
    al_flip_display();
}

void
GameWindow::game_reset()
{
    // reset game and begin
    /*for(auto&& child : towerSet) {
        delete child;
    }
    towerSet.clear();
    monsterSet.clear();*/


    selectedTower = -1;
    lastClicked = -1;
    Coin_Inc_Count = 0;
    Monster_Pro_Count = 0;
    mute = false;
    redraw = false;
    //menu->Reset();

    // stop sample instance
    al_stop_sample_instance(backgroundSound);
    al_stop_sample_instance(startSound);

    // stop timer
    //al_stop_timer(timer);
    //al_stop_timer(monster_pro);
}

void
GameWindow::game_destroy()
{
    game_reset();

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    al_destroy_font(font);
    al_destroy_font(Medium_font);
    al_destroy_font(Large_font);

    al_destroy_timer(timer);
    al_destroy_timer(monster_pro);

    //for(int i=0;i<5; i++)
      //  al_destroy_bitmap(tower[i]);

    al_destroy_bitmap(icon);
    al_destroy_bitmap(background);

    al_destroy_sample(sample);
    al_destroy_sample_instance(startSound);
    al_destroy_sample_instance(backgroundSound);

    //delete level;
    //delete menu;
}

void GameWindow::game_change_scene(int next_scene) {

    /*if (active_scene == SCENE_MENU) {
    }
    else if (active_scene == SCENE_START) {
    }
    else if (active_scene == SCENE_WIN) {
    }
    else if (active_scene == SCENE_GAME_OVER) {
    }*/
    active_scene = next_scene;
    // TODO: Allocate resources before entering scene.
    if (active_scene == SCENE_MENU)
    {

    }
    else if (active_scene == SCENE_START)
    {
        if(role == 1){
            player.img = role1;
            player.img_tool = role1_tool;
            player.attack = 20;
            player.full_hp = 300;
            player.full_mp = 400;
            player.hp = player.full_hp;
            player.mp = player.full_mp;
            player.vx = 0;
            player.x = 200;
            player.y = 600;
            player.defence = 200;
            player.speed = 40;
            player.hidden = false;
            player.w = al_get_bitmap_width(role1);
            player.h = al_get_bitmap_height(role1);
            for(int i = 0;i < MAX_BULLET;i ++){
                player_attack[i].hidden = true;
                player_attack[i].attack = player.attack;
                player_attack[i].w = al_get_bitmap_width(role1_tool);
                player_attack[i].h = al_get_bitmap_height(role1_tool);
                player_attack[i].vx = -1;
                
            }
        }
        else if(role == 2){
            player.img = role2;
            player.img_tool = role2_tool;
            player.attack = 20;
            player.full_hp = 300;
            player.full_mp = 400;
            player.hp = player.full_hp;
            player.mp = player.full_mp;
            player.vx = 0;
            player.x = 200;
            player.y = 400;
            player.defence = 200;
            player.speed = 40;
            player.w = al_get_bitmap_width(role2);
            player.h = al_get_bitmap_height(role2);
            for(int i = 0;i < MAX_BULLET;i ++){
                player_attack[i].hidden = true;
                player_attack[i].attack = player.attack;
                player_attack[i].w = al_get_bitmap_width(role2_tool);
                player_attack[i].h = al_get_bitmap_height(role2_tool);
                player_attack[i].vx = -3;
            }
        }
        else if(role == 3){
            player.img = role3;
            player.img_tool = role2_tool;
            player.attack = 20;
            player.full_hp = 300;
            player.full_mp = 400;
            player.hp = player.full_hp;
            player.mp = player.full_mp;
            player.vx = 0;
            player.x = 200;
            player.y = 400;
            player.defence = 200;
            player.speed = 40;
            player.w = al_get_bitmap_width(role3);
            player.h = al_get_bitmap_height(role3);
            for(int i = 0;i < MAX_BULLET;i ++){
                player_attack[i].hidden = true;
                player_attack[i].attack = player.attack;
                player_attack[i].w = al_get_bitmap_width(role3_tool);
                player_attack[i].h = al_get_bitmap_height(role3_tool);
                player_attack[i].vx = -3;
            }
        }
        else if(role == 4){
            player.img = role4;
            player.img_tool = role3_tool;
            player.attack = 20;
            player.full_hp = 300;
            player.full_mp = 400;
            player.hp = player.full_hp;
            player.mp = player.full_mp;
            player.vx = 0;
            player.x = 200;
            player.y = 400;
            player.defence = 200;
            player.speed = 40;
            player.w = al_get_bitmap_width(role4);
            player.h = al_get_bitmap_height(role4);
            for(int i = 0;i < MAX_BULLET;i ++){
                player_attack[i].hidden = true;
                player_attack[i].attack = player.attack;
                player_attack[i].w = al_get_bitmap_width(role4_tool);
                player_attack[i].h = al_get_bitmap_height(role4_tool);
                player_attack[i].vx = -3;
            }
        }
        for(int i = 0; i < MAX_ENEMY1; i++){
            enemy1[i].img=enemy1_pic;
            enemy1[i].x=enemy1[i].w / 2 + (float)rand() / RAND_MAX * (window_width - enemy1[i].w);
            enemy1[i].y=20;
            enemy1[i].vx = 10;
            enemy1[i].w=al_get_bitmap_width(enemy1[i].img);
            enemy1[i].h=al_get_bitmap_height(enemy1[i].img);
            enemy1[i].hp=50;
            enemy1[i].attack=20;
            enemy1[i].hidden=true;
        }
        for(int i = 0; i < MAX_ENEMY2; i++){
            enemy2[i].img=enemy2_pic;
            enemy2[i].x=enemy2[i].w / 2 + (float)rand() / RAND_MAX * (window_width - enemy2[i].w);
            enemy2[i].y=40;
            enemy2[i].vx = 15;
            enemy2[i].w=al_get_bitmap_width(enemy2[i].img);
            enemy2[i].h=al_get_bitmap_height(enemy2[i].img);
            enemy2[i].hp=100;
            enemy2[i].attack=20;
            enemy2[i].hidden=true;
        }
        for(int i = 0; i < MAX_ENEMY3; i++){
            enemy3[i].img=enemy3_pic;
            enemy3[i].x=enemy3[i].w / 2 + (float)rand() / RAND_MAX * (window_width - enemy3[i].w);
            enemy3[i].y=60;
            enemy3[i].vx = 20;
            enemy3[i].w=al_get_bitmap_width(enemy3[i].img);
            enemy3[i].h=al_get_bitmap_height(enemy3[i].img);
            enemy3[i].hp=200;
            enemy3[i].attack=20;
            enemy3[i].hidden=true;
        }
    }
    else if(active_scene == SCENE_HOME){
        player.hp = player.full_hp;
        player.mp = player.full_mp;
    }
    /*else if(active_scene==SCENE_WIN)
    {
    }
    else if(active_scene==SCENE_GAME_OVER)
    {
    }*/

}

void GameWindow::on_key_down(int keycode) {
    if (active_scene == SCENE_MENU)
    {
        if (keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(SCENE_START);
        else if(keycode == ALLEGRO_KEY_S)
            game_change_scene(SCENE_SETTINGS);

    }

    // [HACKATHON 3-10]
    // TODO: If active_scene is SCENE_SETTINGS and Backspace is pressed,
    // return to SCENE_MENU.
    else if (active_scene == SCENE_SETTINGS) {
        if (keycode == ALLEGRO_KEY_BACKSPACE)
           game_change_scene(SCENE_MENU);
        else if(keycode == ALLEGRO_KEY_ENTER)
            game_change_scene(SCENE_START);
        else if(keycode == ALLEGRO_KEY_1)
            role = 1;
        else if(keycode == ALLEGRO_KEY_2)
            role = 2;
        else if(keycode == ALLEGRO_KEY_3)
            role = 3;
        else if(keycode == ALLEGRO_KEY_4)
            role = 4;
    }

    else if(active_scene == SCENE_START){
        if(keycode == ALLEGRO_KEY_UP)
            game_change_scene(SCENE_HOME);
    }
    else if(active_scene == SCENE_HOME){
        if(keycode == ALLEGRO_KEY_UP)
            game_change_scene(SCENE_START);
    }

}


void GameWindow::on_mouse_down(int btn, int x, int y) {
    // [HACKATHON 3-8]
    // TODO: When settings clicked, switch to settings scene.
    // Uncomment and fill in the code below.
    if (active_scene == SCENE_MENU) {
      if (btn == 1) {
            if (pnt_in_rect(x, y,field_width - 48, 10, 38, 38))
              game_change_scene(SCENE_SETTINGS);
          if(pnt_in_rect(mouse_x, mouse_y,0, 50, 200, 91))
              game_change_scene(SCENE_INTRO);

      }
    }
}

void GameWindow::draw_movable_object(MovableObject obj) {

    al_draw_bitmap(obj.img, round(obj.x - obj.w / 2), round(obj.y - obj.h / 2), 0);
}
ALLEGRO_BITMAP *load_bitmap_resized(const char *filename, int w, int h) {
    ALLEGRO_BITMAP* loaded_bmp = al_load_bitmap(filename);
    ALLEGRO_BITMAP *resized_bmp = al_create_bitmap(w, h);
    ALLEGRO_BITMAP *prev_target = al_get_target_bitmap();

    al_set_target_bitmap(resized_bmp);
    al_draw_scaled_bitmap(loaded_bmp, 0, 0,
        al_get_bitmap_width(loaded_bmp),
        al_get_bitmap_height(loaded_bmp),
        0, 0, w, h, 0);
    al_set_target_bitmap(prev_target);
    al_destroy_bitmap(loaded_bmp);
    return resized_bmp;
}


bool GameWindow::pnt_in_rect(int px, int py, int x, int y, int w, int h) {
    if(px<=x+w&&px>=x&&py<=y+h&&py>=y)
   return true;
    else
   return false;
}
void GameWindow::show_err_msg(int msg)
{
    /*if(msg == GAME_TERMINATE)
        fprintf(stderr, "Game Terminated...");
    else
        fprintf(stderr, "unexpected msg: %d", msg);*/

    game_destroy();
    exit(9);
}