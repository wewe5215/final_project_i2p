#include "Monster.h"
#include<iostream>
using namespace std;
const int axis_x[] = {-1, 1, 0, 0};
const int axis_y[] = {0, 0, 0, 0};
const char direction_name[][10] = {"LEFT", "RIGHT"};

// set counter frequency of drawing moving animation
const int draw_frequency = 2000;

Monster::Monster(std::vector<int> path)
{
    this->path = path;
    this->step = 0;

    // default direction is right
    direction = RIGHT;

    circle = new Circle;
    circle->x = grid_width/2 + (float)rand() / RAND_MAX * (field_width- grid_width);
    circle->y = window_height - 200;
    circle->r = grid_width/2;

    direction_count[LEFT] = 1;
    direction_count[RIGHT] = 1;

    sprite_pos = 0;
    counter = 0;
    strncpy(class_name, "Emerald", 20);
}

Monster::~Monster()
{
    for(unsigned int i=0; i<moveImg.size(); i++)
    {
        ALLEGRO_BITMAP *img = moveImg[i];

        moveImg.erase(moveImg.begin() + i);

        i--;
        al_destroy_bitmap(img);
    }
    moveImg.clear();

    delete circle;
}

void
Monster::Load_Move()
{
    char buffer[50];

    for(int i=0; i < 2; i++)
    {
        for(int j=0; j<direction_count[i]; j++)
        {
            ALLEGRO_BITMAP *img;
            sprintf(buffer, "./%s/%s_%d.png", class_name, direction_name[i], j);

            img = al_load_bitmap(buffer);
            if(img)
                moveImg.push_back(img);
        }
    }
}

void
Monster::Draw()
{
    int w, h;
    int offset = 0;

    // calculate the number of pictures before current direction
    for(int i=0; i<direction; i++)
        offset += direction_count[i];

    if(!moveImg[offset + sprite_pos])
        return;

    // get height and width of sprite bitmap
    w = al_get_bitmap_width(moveImg[offset + sprite_pos]);
    h = al_get_bitmap_height(moveImg[offset + sprite_pos]);


    // draw bitmap align grid edge
    al_draw_bitmap(moveImg[offset + sprite_pos], circle->x - w/2, circle->y - (h - grid_height/2), 0);

    //al_draw_filled_circle(circle->x, circle->y, circle->r, al_map_rgba(196, 79, 79, 200));
}

bool
Monster::Move()
{
    int target_grid_x, target_grid_y;

    counter = (counter + 1) % draw_frequency;

    if(counter == 0)
        sprite_pos = (sprite_pos + 1) % direction_count[direction];
    if(direction == RIGHT && circle->x + grid_width/2 == field_width){
        cout << "reach the rightmost!" << "\n";
        direction = LEFT;
    }
    else{
        circle->x += speed;
    }
    if(direction == LEFT && circle->x - grid_width/2 == 0){
        cout << "reach the leftmost!" << "\n";
        direction = RIGHT;
    }
    else{
        circle->x -= speed;
    }
    // when getting to end point, return true
    if(circle->x == end_x && circle->y == end_y)
        return true;

    circle->x += speed * axis_x[direction];
    //circle->y += speed * axis_y[direction];

    // if not reaching end point, return false
    return false;
}

bool
Monster::Subtract_HP(int harm_point)
{
    HealthPoint -= harm_point;

    return (HealthPoint <= 0);
}

