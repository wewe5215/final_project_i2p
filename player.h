#include <stdio.h>
#include <string.h>
#include <allegro5/allegro.h>
#include <allegro5/allegro_primitives.h>
#include <vector>
#include "Object.h"
#include "Circle.h"
#include "global.h"

enum {LEFT=0, RIGHT};

class MovableObject: public Object {
public:
    MovableObject();
    virtual ~MovableObject();

    // Draw image per frame
    // override virtual function "Object::Draw"
    void Draw();
    // Load bitmaps of animation image into container "moveImg"
    void Load_Move();

    // Update monster position per frame
    // And detect if it reaches end point but not destroyed
    bool Move();

    // functions that return informations of monster
    int getDir() { return direction; }
    

    bool Subtract_HP(int);
    int x, y;
    // The width and height of the object.
    int w, h;
    // The velocity in x, y axes.
    int vx;
    // The pointer to the object’s image.
    int hp;
    int mp;
    int attack;
    int speed;
    int defence;
    int role_num;
    ALLEGRO_BITMAP* img;
protected:
    
private:
    // direction and index for "path"
    unsigned int step;
    int direction;
    // end point
    int end_x, end_y;
    // animation counter
    int counter;
    // animation image of current direction
    int sprite_pos;

    // set of animation images
    ALLEGRO_BITMAP* moveImg;
    

};

