#include "player.h"
#include<iostream>
using namespace std;
const int axis_x[] = {-1, 1, 0, 0};
const int axis_y[] = {0, 0, 0, 0};
const char direction_name[][10] = {"LEFT", "RIGHT"};

// set counter frequency of drawing moving animation
const int draw_frequency = 2000;

MovableObject::MovableObject()
{
    
    this->step = 0;

    // default direction is right
    direction = RIGHT;

    circle = new Circle;
    circle->x = grid_width/2 + (float)rand() / RAND_MAX * (field_width- grid_width);
    circle->y = window_height - 200;
    circle->r = grid_width/2;

    

    sprite_pos = 0;
    counter = 0;
    
}

MovableObject::~MovableObject()
{
    al_destroy_bitmap(img);
    

    delete circle;
}

void
MovableObject::Load_Move()
{
    
}

void
MovableObject::Draw()
{
    int w, h;
    int offset = 0;

    // calculate the number of pictures before current direction
    
    // get height and width of sprite bitmap
    w = al_get_bitmap_width(img);
    h = al_get_bitmap_height(img);


    // draw bitmap align grid edge
    

    //al_draw_filled_circle(circle->x, circle->y, circle->r, al_map_rgba(196, 79, 79, 200));
}

bool
MovableObject::Move()
{
    int target_grid_x, target_grid_y;

    

    
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
MovableObject::Subtract_HP(int harm_point)
{
    hp -= harm_point;

    return (hp <= 0);
}

