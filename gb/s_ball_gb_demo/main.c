#include <gb/gb.h>
#include <stdio.h>
#include "map.h"
#include "sprites.h"

#define GB_WIDTH 160
#define GB_HEIGHT 144

#define LEFT_EDGE 16
#define RIGHT_EDGE 136
#define UP_EDGE 16
#define BOTTOM_EDGE 128


typedef struct {
    uint8_t x;
    uint8_t y;
    int8_t w;
    int8_t h;
    int8_t xSpeed;
    int8_t ySpeed;

} Ball;

typedef struct {
    uint8_t x;
    uint8_t y;
    int8_t w;
    int8_t h;
} Paddle;

typedef enum {
    PADDLE_LEFT,
    PADDLE_RIGHT,
    PADDLE_UP,
    PADDLE_DOWN
} PaddleChoice;

Paddle left;
Paddle right;
Paddle up;
Paddle down;

PaddleChoice choice = PADDLE_LEFT;
void createPaddles() {

    left.w = 8;
    left.h = 32;
    left.x = LEFT_EDGE + 16 - (left.w/2);
    left.y = (GB_HEIGHT/2) - (left.h/2);

    right.w = 8;
    right.h = 32;
    right.x = RIGHT_EDGE - 16 + (right.w/2);
    right.y = (GB_HEIGHT/2) - (right.h/2);

    up.w = 32;
    up.h = 8;
    up.x = (GB_WIDTH/2) - (up.w/2);
    up.y = UP_EDGE + 16 - (up.h/2);

    down.w = 32;
    down.h = 8;
    down.x = (GB_WIDTH/2) - (down.w/2);
    down.y = BOTTOM_EDGE - 24 + (down.h/2);


}

void drawPaddleSprites() {
    for (uint8_t i = 1; i <= 16; i++)
        set_sprite_tile(i,4);
}

void movePaddles() {
    move_sprite(1,left.x+8,left.y+16);
    move_sprite(2,left.x+8,(left.y+8)+16);
    move_sprite(3,left.x+8,(left.y+16)+16);
    move_sprite(4,left.x+8,(left.y+24)+16);

    move_sprite(5,right.x+8,(right.y)+16);
    move_sprite(6,right.x+8,(right.y+8)+16);
    move_sprite(7,right.x+8,(right.y+16)+16);
    move_sprite(8,right.x+8,(right.y+24)+16);

    move_sprite(9,up.x+8,up.y+16);
    move_sprite(10,(up.x+8)+8,up.y+16);
    move_sprite(11,(up.x+16)+8,up.y+16);
    move_sprite(12,(up.x+24)+8,up.y+16);

    move_sprite(13,down.x+8,down.y+16);
    move_sprite(14,(down.x+8)+8,down.y+16);
    move_sprite(15,(down.x+16)+8,down.y+16);
    move_sprite(16,(down.x+24)+8,down.y+16);

}

void checkBallPaddleCollision(Ball* b) {

    if (b->x <= left.x + left.w && b->x + b->w >= left.x) {
        if (b->y <= left.y + left.h && b->y + b->h >= left.y) {
            b->x = (b->x >= left.x + left.w) ? left.x + left.w : left.x - b->w;
            b->xSpeed = -b->xSpeed;
        }
    }

    if (b->x <= right.x + right.w && b->x + b->w >= right.x) {
        if (b->y <= right.y + right.h && b->y + b->h >= right.y) {
            b->x = (b->x < right.x) ? right.x - b->w : right.x + right.w;
            b->xSpeed = -b->xSpeed;
        }
    }

    if (b->x <= up.x + up.w && b->x + b->w >= up.x) {
        if (b->y <= up.y + up.h && b->y + b->h >= up.y) {
            b->y = (b->y >= up.y + up.h) ? up.y + up.h : up.y - up.h;
            b->ySpeed = -b->ySpeed;
        }
    }

    if (b->x <= down.x + down.w && b->x + b->w >= down.x) {
        if (b->y <= down.y + down.h && b->y + b->h >= down.y) {
            b->y = (b->y < down.y) ? down.y - b->h : down.y + down.h;
            b->ySpeed = -b->ySpeed;
        }
    }


}
void createBall(Ball* b, int8_t w, int8_t h) {

    b->w = w;
    b->h = h;
    b->x = (GB_WIDTH/2) - (b->w/2);
    b->y = (GB_HEIGHT/2) - (b->h/2);
    b->xSpeed = 1;
    b->ySpeed = 1;
}

void performanceDelay(uint8_t loops) {

    uint8_t i;

    for (i = 0; i < loops; i++) {
        wait_vbl_done();
    }
}

void moveBall(Ball* b) {
    
    if (b->x < LEFT_EDGE)
        b->xSpeed = -b->xSpeed;

    if (b->x >= RIGHT_EDGE)
        b->xSpeed = -b->xSpeed;

    if (b->y < UP_EDGE)
        b->ySpeed = -b->ySpeed;
        
    if (b->y + b->h >= BOTTOM_EDGE)
        b->ySpeed = -b->ySpeed;
}

void playerMovement() {
        switch(joypad()) {
            case J_B:
                switch(choice) {
                    case PADDLE_LEFT:
                        choice = PADDLE_DOWN;
                        break;
                    case PADDLE_DOWN:
                        choice = PADDLE_RIGHT;
                        break;
                    case PADDLE_RIGHT:
                        choice = PADDLE_UP;
                        break;
                    case PADDLE_UP:
                        choice = PADDLE_LEFT;
                        break;
                    default:
                        break;
                }
                break;
            case J_A:
                switch(choice) {
                    case PADDLE_LEFT:
                        choice = PADDLE_UP;
                        break;
                    case PADDLE_UP:
                        choice = PADDLE_RIGHT;
                        break;
                    case PADDLE_RIGHT:
                        choice = PADDLE_DOWN;
                        break;
                    case PADDLE_DOWN:
                        choice = PADDLE_LEFT;
                        break;
                    default:
                        break;
                }
                break;
            case J_LEFT:
                if (choice == PADDLE_UP) {
                    if (up.x >= left.x + left.w)
                        up.x -= 3;
                }
                else if (choice == PADDLE_DOWN) {
                    if (down.x >= left.x + left.w)
                        down.x -= 3;
                }
                break;
            case J_RIGHT:
                if (choice == PADDLE_UP) {
                    if (up.x + up.w <= right.x)
                        up.x += 3;
                }
                else if (choice == PADDLE_DOWN) {
                    if (down.x + down.w <= right.x)
                        down.x += 3;
                }
                break;
            case J_UP:
                if (choice == PADDLE_LEFT) {
                    if (left.y >= up.y + up.h)
                        left.y -= 3;
                }
                else if (choice == PADDLE_RIGHT) {
                    if (right.y >= up.y + up.h)
                        right.y -= 3;
                }
                break;
            case J_DOWN:
                if (choice == PADDLE_LEFT) {
                    if (left.y + left.h <= down.y)
                        left.y += 3;
                }
                else if (choice == PADDLE_RIGHT) {
                    if (right.y + right.h <= down.y)
                        right.y += 3;
                }
                break;
            default:
                break;
        }
}

void main(void)
{
    Ball ballObj;
    createBall(&ballObj,8,8);
    createPaddles();
    drawPaddleSprites();
    set_sprite_data(0,7,supersprites);
    set_sprite_tile(0,5);
    //move_sprite(0,ballObj.x,ballObj.y);
    set_bkg_data(0,7,supersprites);
    set_bkg_tiles(0,0,superMapWidth,superMapHeight,superMap);

    SHOW_BKG;
    DISPLAY_ON;
    SHOW_SPRITES;

    while(1) {
        moveBall(&ballObj);
        ballObj.x += ballObj.xSpeed;
        ballObj.y += ballObj.ySpeed;
        move_sprite(0,ballObj.x + 8,ballObj.y + 16);
        movePaddles();
        checkBallPaddleCollision(&ballObj);
        playerMovement();
        performanceDelay(1);
    }
}