// Superball
// Designed and Developed by Josuel Corporan Vargas

#include <nds.h>
#include <maxmod9.h>
#include <stdio.h>
#include <nf_lib.h>
#include <string.h>
#include "soundbank.h"
#include "soundbank_bin.h"


#define SCREEN_WIDTH 256	//Width of Nintendo DS Screen
#define SCREEN_HEIGHT 192	// Height of Nintendo DS Screen
#define TILE_SIZE 16		// Tile Size of the boxes
#define PLAYER_TILE_SIZE 8 // Tile size of player paddles and ball
#define PADDLE_WIDTH 32 	// Paddle width
#define PADDLE_HEIGHT 8		// Paddle height
#define PADDLE_SPEED 3		// Paddle Speed 
#define OBJECT_SIZE 160		// Object size
#define BOX_TILE_SIZE 8 	// General tile size

#define TOP_EDGE 16
#define LEFT_EDGE 16

//Define object type, Collision back-end
typedef struct {
	int x_pos;
	int y_pos;
	int width_val;
	int height_val;
} BackEnd;

// Define object type, Destructable
typedef struct {
	int x;
	int y;
	int width;
	int height;
	int spriteID;
} Destructable;

// Define object type, Paddle
typedef struct {

	int x;
	int y;
	int width;
	int height;
	int x1Limit;
	int y1Limit;
	int x2Limit;
	int y2Limit;

} Paddle;

// Define object type, Ball
typedef struct {

	int x_pos;
	int y_pos;
	int width_val;
	int height_val;
	int xSpeed_val;
	int ySpeed_val;
} BallObject;

typedef enum {
	LEFT,
	RIGHT,
	UP,
	DOWN
} PaddleChoice;

typedef enum {
	BACKGROUNDS_CREATED,
	BACKGROUNDS_NOT_CREATED
} ElementCheck;

bool BIG_BOX_CREATED = false;
Destructable boxArray[OBJECT_SIZE];

// Load sprites from the Nitro File System
void loadAssets() {
	NF_LoadTiledBg("s_ball_bg","sbackground",256,256);
	NF_LoadTiledBg("testinfo","info_screen",256,256);
	NF_LoadTiledBg("stars_1_alt","stars",256,256);
	NF_LoadTiledBg("laserback","laser",256,256);
	NF_LoadSpriteGfx("superball_sprites",0,TILE_SIZE,TILE_SIZE);	// Load sprite GFX into slot 0, width 16, height 16
	NF_LoadSpritePal("superball_sprites",0); // Load sprite PAL into slot 0.
	NF_VramSpriteGfx(1,0,0,false); // Load sprite GFX in slot 0 of virtual RAM
	NF_VramSpritePal(1,0,0); // Load sprite PAL in slot 0 of the virtual RAM

	NF_LoadSpriteGfx("superball_sprites_2",1,PLAYER_TILE_SIZE,PLAYER_TILE_SIZE);	// Load sprite GFX into slot 0, width 16, height 16
	NF_LoadSpritePal("superball_sprites_2",1); // Load sprite PAL into slot 0.
	NF_VramSpriteGfx(1,1,1,false); // Load sprite GFX in slot 0 of virtual RAM
	NF_VramSpritePal(1,1,1); // Load sprite PAL in slot 0 of the virtual RAM
}

// // Draws boxes around the screen.
// void drawBoxes(int x1,int y1, int y2, int x2, int x_limit, int y_limit)
// {
// 	int x_pos_1 = 0; // Starting position for the first line of boxes.
// 	int y_pos_1 = TILE_SIZE; // Starting position for the second line of boxes
// 	int y_pos_2 = TILE_SIZE; // Starting position for the third line of boxes
// 	int x_pos_2 = TILE_SIZE; // Starting position for the fourth line of boxes
//  	int spriteID = 0; // Sprite ID assigned to 0.

// 	// While loop for first set of boxes
// 	while (x1 < x_limit) {
		
// 		NF_CreateSprite(1,spriteID,0,0,x_pos_1,0); // Create box sprite with current Sprite ID
// 		NF_SpriteFrame(1,spriteID,0); // Pick sprite Frame of the box

// 		if (x1 >= x_limit)
// 			break; // Leave the while loop if x1 > or equal to x_limit
// 		else {
// 			x_pos_1 = x_pos_1 + TILE_SIZE; // increase the x pos
// 			spriteID++; // increase sprite id
// 			x1++; // increase x1 counter.
// 		}
		
// 	}

// 	while (y1 < y_limit)
// 	{
// 		NF_CreateSprite(1,spriteID,0,0,0,y_pos_1);	// Create box sprite with current Sprite ID
// 		NF_SpriteFrame(1,spriteID,0); 	// Pick sprite Frame of the box

// 		if (y1 >= y_limit)
// 			break;	// Leave the while loop if y1 > or equal to y_limit
// 		else {
// 			y_pos_1 = y_pos_1 + TILE_SIZE;	// increase the y pos
// 			spriteID++;	// increase sprite id
// 			y1++;	// increase y1 counter.
// 		}
// 	}

// 	while (y2 < y_limit)
// 	{
// 		NF_CreateSprite(1,spriteID,0,0,256 - 16,y_pos_2);	// Create box sprite with current Sprite ID
// 		NF_SpriteFrame(1,spriteID,0);	// Pick sprite Frame of the box

// 		if (y2 >= y_limit) {
// 			break;	// Leave the while loop if y2 > or equal to y_limit
// 		}
// 		else
// 		{
// 			y_pos_2 = y_pos_2 + TILE_SIZE;	// increase the y pos
// 			spriteID++;	// increase sprite id
// 			y2++;	// increase y2 counter.
// 		}
// 	}

// 	while (x2 < x_limit - 2)
// 	{
// 		NF_CreateSprite(1,spriteID,0,0,x_pos_2,SCREEN_HEIGHT - TILE_SIZE);	// Create box sprite with current Sprite ID
// 		NF_SpriteFrame(1,spriteID,0);	// Pick sprite Frame of the box

// 		if (x2 >= x_limit - 2)
// 			break;	// Leave the while loop if x2 > or equal to y_limit
// 		else {
// 			x_pos_2 = x_pos_2 + TILE_SIZE;	// increase the x pos
// 			spriteID++;	// increase sprite id
// 			x2++;	// increase x2 counter.
// 		}
// 	}

// 	NF_CreateSprite(1,spriteID+1,0,0,TILE_SIZE,TILE_SIZE);	// Draw laser spawner on up-left corner
// 	NF_CreateSprite(1,spriteID+2,0,0,SCREEN_WIDTH - (TILE_SIZE * 2),TILE_SIZE);	//Draw laser spawner on the up-right corner
// 	NF_CreateSprite(1,spriteID+3,0,0,SCREEN_WIDTH - (TILE_SIZE * 2),SCREEN_HEIGHT - (TILE_SIZE * 2));	//Draw on the down-right corner
// 	NF_CreateSprite(1,spriteID+4,0,0,TILE_SIZE,SCREEN_HEIGHT - (TILE_SIZE * 2));	// Draw laser spawner on the down-left corner
// 	NF_SpriteFrame(1,spriteID+1,1);	// Set frame for first laser spanwer
// 	NF_SpriteFrame(1,spriteID+2,1);	// Set frame for second laser spawner
// 	NF_SpriteFrame(1,spriteID+3,1);	// Set frame for third laser spawner
// 	NF_SpriteFrame(1,spriteID+4,1);	// Set frame for fourth laser spawner
// }

// Create the ball object
void createBall(BallObject* b, int b_speed, int bY) {

	b->width_val = 8;
	b->height_val = 8;
	b->x_pos = SCREEN_WIDTH/2 - b->width_val/2;
	b->y_pos = bY - 16;
	b->xSpeed_val = b_speed;
	b->ySpeed_val = b_speed;
	
}



// Handle the ball collision with the screen, and the paddles.
void handleBallCollision(BallObject* b, Paddle* l, Paddle* r, Paddle* u, Paddle* d, int b_speed, mm_sound_effect sound) {


	// Player collision for the left side of the screen
	if (b->x_pos < LEFT_EDGE) {
		mmEffectEx(&sound);
		b->x_pos = LEFT_EDGE;
		b->xSpeed_val = -b->xSpeed_val;
	}
	else if (b->x_pos + b->width_val >= SCREEN_WIDTH - TILE_SIZE) { // Ball collision for the right side of the screen.
		mmEffectEx(&sound);
		b->x_pos = SCREEN_WIDTH - TILE_SIZE - b->width_val; // We do subtract the tile size and ball width value from the screen so we can ricochet from the border boxes.
		b->xSpeed_val = -b->xSpeed_val;
	}

	// Ball collision for the top edge of the screen
	if (b->y_pos < TOP_EDGE)
	{
		mmEffectEx(&sound);
		b->y_pos = TOP_EDGE;
		b->ySpeed_val = -b->ySpeed_val;
	}
	else if (b->y_pos + b->height_val >= SCREEN_HEIGHT - TILE_SIZE) { // Ball collision for the bottom of the screen
		mmEffectEx(&sound);
		b->y_pos = SCREEN_HEIGHT - TILE_SIZE - b->height_val; // We subtract the tile size and the ball height from the screen height, so the ball can richocet from the border boxes at the bottom.
		b->ySpeed_val = -b->ySpeed_val;
	}

	// Ball collision for the left paddle. We use AABB collision.
	if (b->x_pos <= l->x + l->width && b->x_pos + b->width_val >= l->x) {
		if (b->y_pos <= l->y + l->height && b->y_pos + b->height_val >= l->y) {
			mmEffectEx(&sound);
			b->x_pos = (b->x_pos >= l->x + l->width) ? l->x + l->width : l->x - b->width_val;
			b->xSpeed_val = -b->xSpeed_val;
		}
	}

	// Ball collision for the right paddle. We use AABB collision.
	if (b->x_pos <= r->x + r->width && b->x_pos + b->width_val >= r->x) {
		if (b->y_pos <= r->y + r->height && b->y_pos + b->height_val >= r->y) {
			mmEffectEx(&sound);
			b->x_pos = (b->x_pos <= r->x) ? r->x - b->width_val: r->x + r->width;
			b->xSpeed_val = -b->xSpeed_val;
		}
	}

	// Ball collision for the up paddle. We use AABB collision.
	if (b->x_pos <= u->x + u->width && b->x_pos + b->width_val >= u->x) {
		if (b->y_pos <= u->y + u->height && b->y_pos + b->height_val >= u->y) {
			mmEffectEx(&sound);
			b->y_pos = (b->y_pos >= u->y + u->height) ? u->y + u->height : u->y - b->height_val;
			b->ySpeed_val = -b->ySpeed_val;
		}
	}

	// Ball collision for the down paddle. We use AABB collision.
	if (b->x_pos <= d->x + d->width && b->x_pos + b->width_val >= d->x) {
		if (b->y_pos <= d->y + d->height && b->y_pos + b->height_val >= d->y) {
			mmEffectEx(&sound);
			b->y_pos = (b->y_pos <= d->y) ? d->y - b->height_val : d->y + d->height;
			b->ySpeed_val = -b->ySpeed_val;
		}
	}

	for (int i = 0; i < OBJECT_SIZE; i++) {
		// We use the 
		if (b->x_pos < boxArray[i].x + boxArray[i].width && b->x_pos + b->width_val > boxArray[i].x) {
			if (boxArray[i].y < b->y_pos + b->height_val && boxArray[i].y + boxArray[i].height > b->y_pos) {
				if (b->x_pos >= boxArray[i].x + boxArray[i].width)
					b->x_pos = boxArray[i].x + boxArray[i].width;
				else if (b->x_pos < boxArray[i].x)
					b->x_pos = boxArray[i].x - b->width_val;
				
				if (b->y_pos >= boxArray[i].y + boxArray[i].height)
					b->y_pos = boxArray[i].y + boxArray[i].height;
				else if (b->y_pos < boxArray[i].y)
					b->y_pos = boxArray[i].y - b->height_val;
		

				b->xSpeed_val = -b->xSpeed_val;
				b->ySpeed_val = -b->ySpeed_val;
				mmEffectEx(&sound);
				NF_DeleteSprite(1,boxArray[i].spriteID);
				for (int j = i; j < OBJECT_SIZE - 1; j++)
					memcpy(&boxArray[j],&boxArray[j+1],sizeof(boxArray[j+1]));

			}
		}
	}

	// Increase the ball x pos by adding it with the x speed value.
	b->x_pos = b->x_pos + b->xSpeed_val;
	// Increase the ball y pos by adding it with the y speed value.
	b->y_pos = b->y_pos + b->ySpeed_val;

	// Move the sprite
	NF_MoveSprite(1,16,b->x_pos,b->y_pos);
}

// Draw the ball
void drawBall(BallObject* b) {
	// Create the sprite
	NF_CreateSprite(1,16,1,1,b->x_pos,b->y_pos);
	// Set the frame of the sprite.
	NF_SpriteFrame(1,16,2);
}

// Create player paddles, and set the coordinates.
void createPaddles(Paddle* l_p, Paddle* r_p, Paddle* u_p, Paddle* d_p) {
	
	// First paddle
	l_p->width = 8;
	l_p->height = PADDLE_WIDTH;
	l_p->x = 32 + (TILE_SIZE/2 - l_p->width/2);
	l_p->y = SCREEN_HEIGHT/2 - l_p->height/2;

	// Second Paddle
	r_p->width = 8;
	r_p->height = PADDLE_WIDTH;
	r_p->x = (SCREEN_WIDTH - (TILE_SIZE * 3)) + (TILE_SIZE/2 - r_p->width/2);
	r_p->y = SCREEN_HEIGHT/2 - r_p->height/2;

	// Third paddle
	u_p->width = PADDLE_WIDTH;
	u_p->height = PADDLE_HEIGHT;
	u_p->x = SCREEN_WIDTH/2 - u_p->width/2;
	u_p->y = 32 + (TILE_SIZE/2 - u_p->height/2);

	// Fourth paddle
	d_p->width = PADDLE_WIDTH;
	d_p->height = PADDLE_HEIGHT;
	d_p->x = SCREEN_WIDTH/2 - d_p->width/2;
	d_p->y = (SCREEN_HEIGHT - (TILE_SIZE * 3)) + (TILE_SIZE/2 - d_p->height/2);
	
}

// Create collision backends
void createBackEnds(BackEnd* left, BackEnd* right, BackEnd* up, BackEnd* down, Paddle* l, Paddle* r, Paddle* u, Paddle* d) {
	left->width_val = 24;
	//left->x_pos = 32 + (TILE_SIZE/2 - left->width_val/2);
	left->x_pos = (l->x) - (left->width_val/3);
	left->y_pos = (u->y + u->height + 16);
	left->height_val = (d->y - 16) - (left->y_pos);

	right->width_val = 24;
	//right->x_pos = (SCREEN_WIDTH - (TILE_SIZE * 3)) + (TILE_SIZE/2 - right->width_val/2);
	right->x_pos = (r->x) - (right->width_val/3);
	right->y_pos = (u->y + u->height + 16);
	right->height_val = (d->y - 16) - (right->y_pos);

	up->height_val = 24;
	up->x_pos = (l->x + l->width + 16);
	//up->y_pos = 32 + (TILE_SIZE/2 - up->height_val/2);
	up->y_pos = (u->y) - (up->height_val/3);
	up->width_val = (r->x - 16) - (up->x_pos);

	down->height_val = 24;
	down->x_pos = (l->x + l->width + 16);
	//down->y_pos = (SCREEN_HEIGHT - (TILE_SIZE * 3)) + (TILE_SIZE/2 - down->height_val/2);
	down->y_pos = (d->y) - (down->height_val/3);
	down->width_val = (r->x - 16) - (up->x_pos);

	l->y1Limit = up->y_pos + up->height_val;
	l->y2Limit = down->y_pos;

	r->y1Limit = up->y_pos + up->height_val;
	r->y2Limit = down->y_pos;

	u->x1Limit = left->x_pos + left->width_val;
	u->x2Limit = right->x_pos;

	d->x1Limit = left->x_pos + left->width_val;
	d->x2Limit = right->x_pos;
}
// Draw player paddles
void drawPaddles(Paddle* l_p, Paddle* r_p, Paddle* u_p, Paddle* d_p) {

	// Creates sprites for index 60,61,62. It is a metasprite for the left paddle, with the the x coordinate, and 
	// y coordinates going down by 8. We will end up with 1 sprite made up of three sprites. GFX and VRAM slots are 1,
	// since there is where the source files that contains the sprites used
	NF_CreateSprite(1,0,1,1,l_p->x,l_p->y);
	NF_CreateSprite(1,1,1,1,l_p->x,l_p->y+8);
	NF_CreateSprite(1,2,1,1,l_p->x,l_p->y+16);
	NF_CreateSprite(1,3,1,1,l_p->x,l_p->y+24);

	// Set sprite layers for sprite ids 0-3
	NF_SpriteLayer(1,0,0);
	NF_SpriteLayer(1,1,0);
	NF_SpriteLayer(1,2,0);
	NF_SpriteLayer(1,3,0);
	// Set the frames for the sprites drawn on index (60,61,62)
	NF_SpriteFrame(1,0,0);
	NF_SpriteFrame(1,1,1);
	NF_SpriteFrame(1,2,1);
	NF_SpriteFrame(1,3,0);

	// Creates sprites for index 63,64,65. It is a metasprite for the right paddle, with the the x coordinate, and 
	// y coordinates going down by 8. We will end up with 1 sprite made up of three sprites. GFX and VRAM slots are 1,
	// since there is where the source files that contains the sprites used.
	NF_CreateSprite(1,4,1,1,r_p->x,r_p->y);
	NF_CreateSprite(1,5,1,1,r_p->x,r_p->y+8);
	NF_CreateSprite(1,6,1,1,r_p->x,r_p->y+16);
	NF_CreateSprite(1,7,1,1,r_p->x,r_p->y+24);

	// Set sprite layers for sprite ids 3-5
	NF_SpriteLayer(1,4,0);
	NF_SpriteLayer(1,5,0);
	NF_SpriteLayer(1,6,0);
	NF_SpriteLayer(1,7,0);
	// Set the frames for the sprites drawn on index (63,64,65)
	NF_SpriteFrame(1,4,0);
	NF_SpriteFrame(1,5,1);
	NF_SpriteFrame(1,6,1);
	NF_SpriteFrame(1,7,0);

	// Creates sprites for index 66,67,68. It is a metasprite for the up paddle, with the the x coordinate, and 
	// y coordinates going down by 8. We will end up with 1 sprite made up of three sprites. GFX and VRAM slots are 1,
	// since there is where the source files that contains the sprites used
	NF_CreateSprite(1,8,1,1,u_p->x,u_p->y);
	NF_CreateSprite(1,9,1,1,u_p->x+8,u_p->y);
	NF_CreateSprite(1,10,1,1,u_p->x+16,u_p->y);
	NF_CreateSprite(1,11,1,1,u_p->x+24,u_p->y);
	// Set sprite layers for sprite ids 6-8
	NF_SpriteLayer(1,8,0);
	NF_SpriteLayer(1,9,0);
	NF_SpriteLayer(1,10,0);
	NF_SpriteLayer(1,11,0);
	// Set the frames for the sprites drawn on index (66,67,68)
	NF_SpriteFrame(1,8,0);
	NF_SpriteFrame(1,9,1);
	NF_SpriteFrame(1,10,1);
	NF_SpriteFrame(1,11,0);
	// Creates sprites for index 69,70,71. It is a metasprite for the down paddle, with the the x coordinate, and 
	// y coordinates going down by 8. We will end up with 1 sprite made up of three sprites. GFX and VRAM slots are 1,
	// since there is where the source files that contains the sprites used
	NF_CreateSprite(1,12,1,1,d_p->x,d_p->y);
	NF_CreateSprite(1,13,1,1,d_p->x+8,d_p->y);
	NF_CreateSprite(1,14,1,1,d_p->x+16,d_p->y);
	NF_CreateSprite(1,15,1,1,d_p->x+24,d_p->y);

	// Set sprite layers for sprite ids 9-11
	NF_SpriteLayer(1,12,0);
	NF_SpriteLayer(1,13,0);
	NF_SpriteLayer(1,14,0);
	NF_SpriteLayer(1,15,0);
	// Set the frames for the sprites drawn on index (69,70,71)
	NF_SpriteFrame(1,12,0);
	NF_SpriteFrame(1,13,1);
	NF_SpriteFrame(1,14,1);
	NF_SpriteFrame(1,15,0);


}

void movesSelectPaddles(Paddle* l_p, Paddle* r_p, Paddle* u_p, Paddle* d_p, BackEnd* l, BackEnd* r, BackEnd* u, BackEnd* d, touchPosition pen) {

	if (keysHeld() & KEY_TOUCH) {
		// We only acknowledge the collision below when the pen's x position is less than the x pos limit, where the paddle cannot go left past this point.
		// This also goes to the pen's x position is greater than the second x pos limit, where the paddle cannot go right past the point.
		if (pen.px < u->x_pos + u->width_val && pen.px > u->x_pos) {
			if (pen.py < u->y_pos + u->height_val && pen.py > u->y_pos) {
				u_p->x = pen.px - u_p->width/2;
			}
		}

		// We only acknowledge the collision below when the pen's x position is less than the x pos limit, where the paddle cannot go left past this point.
		// This also goes to the pen's x position is greater than the second x pos limit, where the paddle cannot go right past the point.
		if (pen.px < d->x_pos + d->width_val && pen.px > d->x_pos) {
			if (pen.py < d->y_pos + d->height_val && pen.py > d->y_pos) {
				d_p->x = pen.px - d_p->width/2;
			}
		}

		// We only acknowledge the collision below when the pen's y position is less than the y pos limit, where the paddle cannot go up past this point.
		// This also goes to the pen's x position is greater than the second y pos limit, where the paddle cannot go down past the point.
		if (pen.px < r->x_pos + r->width_val && pen.px > r->x_pos) {
			if (pen.py < r->y_pos + r->height_val && pen.py > r->y_pos) {
				r_p->y = pen.py - r_p->height/2;
			}
		}

		// We only acknowledge the collision below when the pen's y position is less than the y pos limit, where the paddle cannot go up past this point.
		// This also goes to the pen's x position is greater than the second y pos limit, where the paddle cannot go down past the point.
		if (pen.px < l->x_pos + l->width_val && pen.px > l->x_pos) {
			if (pen.py < l->y_pos + l->height_val && pen.py > l->y_pos) {
				l_p->y = pen.py - l_p->height/2;
			}
		}
	}


	NF_MoveSprite(1,0,l_p->x,l_p->y);
	NF_MoveSprite(1,1,l_p->x,l_p->y+8);
	NF_MoveSprite(1,2,l_p->x,l_p->y+16);
	NF_MoveSprite(1,3,l_p->x,l_p->y+24);

	NF_MoveSprite(1,4,r_p->x,r_p->y);
	NF_MoveSprite(1,5,r_p->x,r_p->y+8);
	NF_MoveSprite(1,6,r_p->x,r_p->y+16);
	NF_MoveSprite(1,7,r_p->x,r_p->y+24);

	NF_MoveSprite(1,8,u_p->x,u_p->y);
	NF_MoveSprite(1,9,u_p->x+8,u_p->y);
	NF_MoveSprite(1,10,u_p->x+16,u_p->y);
	NF_MoveSprite(1,11,u_p->x+24,u_p->y);

	NF_MoveSprite(1,12,d_p->x,d_p->y);
	NF_MoveSprite(1,13,d_p->x+8,d_p->y);
	NF_MoveSprite(1,14,d_p->x+16,d_p->y);
	NF_MoveSprite(1,15,d_p->x+24,d_p->y);
}

// Create matrix of boxes
void createDestructibles(int spID, int w, int h, int x, int y, int xamt) {
	if (!BIG_BOX_CREATED) {
		for (int i = x; i < (x + w); i+=8) {
			for (int j = y; j < (y + h); j+=8) {
				// We use a for loop to create the matrix of boxes.
				// Keep in mind we should also keep track of the xamt, and the spriteID
				Destructable box;
				box.x = i;
				box.y = j;
				box.width = BOX_TILE_SIZE;
				box.height = BOX_TILE_SIZE;
				box.spriteID = spID;
				memcpy(&boxArray[xamt],&box,sizeof(box));
				NF_CreateSprite(1,boxArray[xamt].spriteID,1,1,i,j);
				NF_SpriteFrame(1,boxArray[xamt].spriteID,0);
				// Increment sprite ID, and xamt
				spID++;
				xamt++;
			}
		}
		BIG_BOX_CREATED = true;
	}

}
int main() {
	BackEnd left;
	BackEnd right;
	BackEnd up;
	BackEnd down;

	int w = 64;
	int h = 64;
	int boxX = SCREEN_WIDTH/2 - w/2;
	int boxY = SCREEN_HEIGHT/2 - h/2;
	// Set 2D engine on the top screen with mode 0 (Tiled BG)
	NF_Set2D(0,0);
	// Set 2D engine on the bottom screen with mode 0 (Tiled BG)
	NF_Set2D(1,0);
	// Init tiled bg buffers
	NF_InitTiledBgBuffers();
	// Init tiled bg system
	NF_InitTiledBgSys(0);
	NF_InitTiledBgSys(1);
	//NF_InitAffineBgSys(1);
	// Initiate sprite buffers
	NF_InitSpriteBuffers();
	// Init sprite system for bottom screen 1
	NF_InitSpriteSys(1);

	// Set root folder to the nitro filesystem.
	NF_SetRootFolder("NITROFS");
	int boxXAmount = 0;
	// Load assets
	loadAssets();
	int ball_speed = 1;
	// Create left_paddle object
	Paddle left_paddle;

	// Create right_paddle object
	Paddle right_paddle;

	// Create up_paddle object
	Paddle up_paddle;
	
	// Create down_paddle object
	Paddle down_paddle;

	// Create ball object
	BallObject ball;
	// Set choice to left_paddle
	PaddleChoice choice = LEFT;
	int freeSpriteID = 17;
	// Create coordinates, width, height for each paddle.
	createPaddles(&left_paddle,&right_paddle,&up_paddle,&down_paddle);
	createBackEnds(&left,&right,&up,&down,&left_paddle,&right_paddle,&up_paddle,&down_paddle);
	createBall(&ball,ball_speed,boxY);
	mmInitDefaultMem((mm_addr)soundbank_bin);
	mmLoadEffect(SFX_LASER);
	mm_sound_effect collide_sound = {
		{SFX_LASER},
		0x400,
		0,
		255,
		128,
	};
	touchPosition stylus;
	int scrolly1 = 0;
	int scrolly2 = 0;
	ElementCheck check = BACKGROUNDS_NOT_CREATED;
	// Game loop
	while(1) {
		float dt = 1/60;
		if (keysDown() & KEY_RIGHT)
			ball.xSpeed_val++;
		if (keysDown() & KEY_LEFT)
			ball.xSpeed_val--;
		if (keysDown() & KEY_UP)
			ball.ySpeed_val++;
		if (keysDown() & KEY_DOWN)
			ball.ySpeed_val--;
		// Draw ball
		drawBall(&ball);
		// Draw paddles
		drawPaddles(&left_paddle,&right_paddle,&up_paddle,&down_paddle);
		// Handle paddle movement
		movesSelectPaddles(&left_paddle,&right_paddle,&up_paddle,&down_paddle,&left,&right,&up,&down,stylus);
		createDestructibles(freeSpriteID,w,h,boxX,boxY,boxXAmount);
		handleBallCollision(&ball,&left_paddle,&right_paddle,&up_paddle,&down_paddle,ball_speed,collide_sound);

		if (check != BACKGROUNDS_CREATED) {
			NF_CreateTiledBg(0,1,"info_screen");
			NF_CreateTiledBg(1,0,"sbackground");
			NF_CreateTiledBg(1,2,"stars");
			NF_CreateTiledBg(1,3,"stars");
			NF_CreateTiledBg(1,1,"laser");
			check = BACKGROUNDS_CREATED;
		}

		// scrollx1+=-2;
		scrolly1+=-2;
		//scrollx2+=-1;
		scrolly2+=-1;

		NF_ScrollBg(1,2,0,scrolly1);
		NF_ScrollBg(1,3,0,scrolly2);
		if (scrolly1 <= -192)
			scrolly1 = 0;

		if (scrolly2 <= -192)
			scrolly2 = 0;


		NF_SpriteOamSet(1);
		touchRead(&stylus);
		scanKeys();
		swiWaitForVBlank();
		oamUpdate(&oamSub);
	}
	return 0;

}