// Superball
// Designed and Developed by Josuel Corporan Vargas

#include <nds.h>
#include <nf_lib.h>

#define SCREEN_WIDTH 256	//Width of Nintendo DS Screen
#define SCREEN_HEIGHT 192	// Height of Nintendo DS Screen
#define TILE_SIZE 16		// Tile Size of the boxes
#define PLAYER_TILE_SIZE 8 // Tile size of player paddles and ball
#define PADDLE_WIDTH 24 	// Paddle width
#define PADDLE_HEIGHT 8		// Paddle height
#define PADDLE_SPEED 3		// Paddle Speed 

#define TOP_EDGE 16
#define LEFT_EDGE 16

// Define object type, Paddle
typedef struct {

	int x;
	int y;
	int width;
	int height;

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

// Load sprites from the Nitro File System
void loadSprites() {
	NF_LoadSpriteGfx("superball_sprites",0,TILE_SIZE,TILE_SIZE);	// Load sprite GFX into slot 0, width 16, height 16
	NF_LoadSpritePal("superball_sprites",0); // Load sprite PAL into slot 0.
	NF_VramSpriteGfx(1,0,0,false); // Load sprite GFX in slot 0 of virtual RAM
	NF_VramSpritePal(1,0,0); // Load sprite PAL in slot 0 of the virtual RAM

	NF_LoadSpriteGfx("superball_sprites_2",1,PLAYER_TILE_SIZE,PLAYER_TILE_SIZE);	// Load sprite GFX into slot 0, width 16, height 16
	NF_LoadSpritePal("superball_sprites_2",1); // Load sprite PAL into slot 0.
	NF_VramSpriteGfx(1,1,1,false); // Load sprite GFX in slot 0 of virtual RAM
	NF_VramSpritePal(1,1,1); // Load sprite PAL in slot 0 of the virtual RAM
}

// Draws boxes around the screen.
void drawBoxes(int x1,int y1, int y2, int x2, int x_limit, int y_limit)
{
	int x_pos_1 = 0; // Starting position for the first line of boxes.
	int y_pos_1 = TILE_SIZE; // Starting position for the second line of boxes
	int y_pos_2 = TILE_SIZE; // Starting position for the third line of boxes
	int x_pos_2 = TILE_SIZE; // Starting position for the fourth line of boxes
 	int spriteID = 0; // Sprite ID assigned to 0.

	// While loop for first set of boxes
	while (x1 < x_limit) {
		
		NF_CreateSprite(1,spriteID,0,0,x_pos_1,0); // Create box sprite with current Sprite ID
		NF_SpriteFrame(1,spriteID,0); // Pick sprite Frame of the box

		if (x1 >= x_limit)
			break; // Leave the while loop if x1 > or equal to x_limit
		else {
			x_pos_1 = x_pos_1 + TILE_SIZE; // increase the x pos
			spriteID++; // increase sprite id
			x1++; // increase x1 counter.
		}
		
	}

	while (y1 < y_limit)
	{
		NF_CreateSprite(1,spriteID,0,0,0,y_pos_1);	// Create box sprite with current Sprite ID
		NF_SpriteFrame(1,spriteID,0); 	// Pick sprite Frame of the box

		if (y1 >= y_limit)
			break;	// Leave the while loop if y1 > or equal to y_limit
		else {
			y_pos_1 = y_pos_1 + TILE_SIZE;	// increase the y pos
			spriteID++;	// increase sprite id
			y1++;	// increase y1 counter.
		}
	}

	while (y2 < y_limit)
	{
		NF_CreateSprite(1,spriteID,0,0,256 - 16,y_pos_2);	// Create box sprite with current Sprite ID
		NF_SpriteFrame(1,spriteID,0);	// Pick sprite Frame of the box

		if (y2 >= y_limit) {
			break;	// Leave the while loop if y2 > or equal to y_limit
		}
		else
		{
			y_pos_2 = y_pos_2 + TILE_SIZE;	// increase the y pos
			spriteID++;	// increase sprite id
			y2++;	// increase y2 counter.
		}
	}

	while (x2 < x_limit - 2)
	{
		NF_CreateSprite(1,spriteID,0,0,x_pos_2,SCREEN_HEIGHT - TILE_SIZE);	// Create box sprite with current Sprite ID
		NF_SpriteFrame(1,spriteID,0);	// Pick sprite Frame of the box

		if (x2 >= x_limit - 2)
			break;	// Leave the while loop if x2 > or equal to y_limit
		else {
			x_pos_2 = x_pos_2 + TILE_SIZE;	// increase the x pos
			spriteID++;	// increase sprite id
			x2++;	// increase x2 counter.
		}
	}

	NF_CreateSprite(1,spriteID+1,0,0,TILE_SIZE,TILE_SIZE);	// Draw laser spawner on up-left corner
	NF_CreateSprite(1,spriteID+2,0,0,SCREEN_WIDTH - (TILE_SIZE * 2),TILE_SIZE);	//Draw laser spawner on the up-right corner
	NF_CreateSprite(1,spriteID+3,0,0,SCREEN_WIDTH - (TILE_SIZE * 2),SCREEN_HEIGHT - (TILE_SIZE * 2));	//Draw on the down-right corner
	NF_CreateSprite(1,spriteID+4,0,0,TILE_SIZE,SCREEN_HEIGHT - (TILE_SIZE * 2));	// Draw laser spawner on the down-left corner
	NF_SpriteFrame(1,spriteID+1,1);	// Set frame for first laser spanwer
	NF_SpriteFrame(1,spriteID+2,1);	// Set frame for second laser spawner
	NF_SpriteFrame(1,spriteID+3,1);	// Set frame for third laser spawner
	NF_SpriteFrame(1,spriteID+4,1);	// Set frame for fourth laser spawner
}

// Create the ball object
void createBall(BallObject* b, int b_speed) {

	b->width_val = 8;
	b->height_val = 8;
	b->x_pos = SCREEN_WIDTH/2 - b->width_val/2;
	b->y_pos = SCREEN_HEIGHT/2 - b->height_val/2;
	b->xSpeed_val = b_speed;
	b->ySpeed_val = b_speed;
	
}


// Handle the ball collision with the screen, and the paddles.
void handleBallCollision(BallObject* b, Paddle* l, Paddle* r, Paddle* u, Paddle* d, int b_speed) {


	// Player collision for the left side of the screen
	if (b->x_pos < LEFT_EDGE) {
		b->x_pos = LEFT_EDGE;
		b->xSpeed_val = -b->xSpeed_val;
	}
	else if (b->x_pos + b->width_val >= SCREEN_WIDTH - TILE_SIZE) { // Ball collision for the right side of the screen.
		b->x_pos = SCREEN_WIDTH - TILE_SIZE - b->width_val; // We do subtract the tile size and ball width value from the screen so we can ricochet from the border boxes.
		b->xSpeed_val = -b->xSpeed_val;
	}

	// Ball collision for the top edge of the screen
	if (b->y_pos < TOP_EDGE)
	{
		b->y_pos = TOP_EDGE;
		b->ySpeed_val = -b->ySpeed_val;
	}
	else if (b->y_pos + b->height_val >= SCREEN_HEIGHT - TILE_SIZE) { // Ball collision for the bottom of the screen
		b->y_pos = SCREEN_HEIGHT - TILE_SIZE - b->height_val; // We subtract the tile size and the ball height from the screen height, so the ball can richocet from the border boxes at the bottom.
		b->ySpeed_val = -b->ySpeed_val;
	}

	// Ball collision for the left paddle. We use AABB collision.
	if (b->x_pos < l->x + l->width && b->x_pos + b->width_val > l->x) {
		if (b->y_pos < l->y + l->height && b->y_pos + b->height_val > l->y) {
			b->x_pos = l->x + l->width;
			b->xSpeed_val = -b->xSpeed_val;
		}
	}

	// Ball collision for the right paddle. We use AABB collision.
	if (b->x_pos < r->x + r->width && b->x_pos + b->width_val > r->x) {
		if (b->y_pos < r->y + r->height && b->y_pos + b->height_val > r->y) {
			b->x_pos = r->x - b->width_val;
			b->xSpeed_val = -b->xSpeed_val;
		}
	}

	// Ball collision for the up paddle. We use AABB collision.
	if (b->x_pos < u->x + u->width && b->x_pos + b->width_val > u->x) {
		if (b->y_pos < u->y + u->height && b->y_pos + b->height_val > u->y) {
			b->y_pos = u->y + u->height;
			b->ySpeed_val = -b->ySpeed_val;
		}
	}

	// Ball collision for the down paddle. We use AABB collision.
	if (b->x_pos < d->x + d->width && b->x_pos + b->width_val > d->x) {
		if (b->y_pos < d->y + d->height && b->y_pos + b->height_val > d->y) {
			b->y_pos = d->y - b->height_val;
			b->ySpeed_val = -b->ySpeed_val;
		}
	}


	// Ball collision for the left paddle (backside). We use AABB collision.
	if (b->x_pos < l->x) {
		if (b->y_pos > l->y && b->y_pos + b->height_val < l->y + l->height) {
			b->x_pos = l->x - b->width_val;
			b->xSpeed_val = -2;
		}
	}

	// Ball collision for the right paddle (backside). We use AABB collision.
	if (b->x_pos > r->x + r->width && b->x_pos + b->width_val > r->x) {
		if (b->y_pos > r->y && b->y_pos + b->width_val < r->y + r->height) {
			b->x_pos = r->x + r->width;
			b->xSpeed_val = 2;
		}
	}

	// Ball collision for the up paddle (topside). We use AABB collision.
	if (b->x_pos > u->x && b->x_pos + b->width_val < u->x + u->width) {
		if (b->y_pos < u->y) {
			b->y_pos = u->y - b->height_val;
			b->ySpeed_val = -2;
		}
	}

	// Ball collision for the down paddle (bottomside). We use AABB collision.
	if (b->x_pos > d->x && b->x_pos + b->width_val < d->x + d->width) {
		if (b->y_pos > d->y + d->height) {
			b->y_pos = d->y + b->height_val;
			b->ySpeed_val = 2;
		}
	}
	// Increase the ball x pos by adding it with the x speed value.
	b->x_pos = b->x_pos + b->xSpeed_val;
	// Increase the ball y pos by adding it with the y speed value.
	b->y_pos = b->y_pos + b->ySpeed_val;

	// Move the sprite
	NF_MoveSprite(1,72,b->x_pos,b->y_pos);
}

// Draw the ball
void drawBall(BallObject* b) {
	// Create the sprite
	NF_CreateSprite(1,72,1,1,b->x_pos,b->y_pos);
	// Set the frame of the sprite.
	NF_SpriteFrame(1,72,2);
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

// Draw player paddles
void drawPaddles(Paddle* l_p, Paddle* r_p, Paddle* u_p, Paddle* d_p) {

	// Creates sprites for index 60,61,62. It is a metasprite for the left paddle, with the the x coordinate, and 
	// y coordinates going down by 8. We will end up with 1 sprite made up of three sprites. GFX and VRAM slots are 1,
	// since there is where the source files that contains the sprites used
	NF_CreateSprite(1,60,1,1,l_p->x,l_p->y);
	NF_CreateSprite(1,61,1,1,l_p->x,l_p->y+8);
	NF_CreateSprite(1,62,1,1,l_p->x,l_p->y+16);

	// Set the frames for the sprites drawn on index (60,61,62)
	NF_SpriteFrame(1,60,0);
	NF_SpriteFrame(1,61,1);
	NF_SpriteFrame(1,62,0);

	// Creates sprites for index 63,64,65. It is a metasprite for the right paddle, with the the x coordinate, and 
	// y coordinates going down by 8. We will end up with 1 sprite made up of three sprites. GFX and VRAM slots are 1,
	// since there is where the source files that contains the sprites used.
	NF_CreateSprite(1,63,1,1,r_p->x,r_p->y);
	NF_CreateSprite(1,64,1,1,r_p->x,r_p->y+8);
	NF_CreateSprite(1,65,1,1,r_p->x,r_p->y+16);

	// Set the frames for the sprites drawn on index (63,64,65)
	NF_SpriteFrame(1,63,0);
	NF_SpriteFrame(1,64,1);
	NF_SpriteFrame(1,65,0);

	// Creates sprites for index 66,67,68. It is a metasprite for the up paddle, with the the x coordinate, and 
	// y coordinates going down by 8. We will end up with 1 sprite made up of three sprites. GFX and VRAM slots are 1,
	// since there is where the source files that contains the sprites used
	NF_CreateSprite(1,66,1,1,u_p->x,u_p->y);
	NF_CreateSprite(1,67,1,1,u_p->x+8,u_p->y);
	NF_CreateSprite(1,68,1,1,u_p->x+16,u_p->y);

	// Set the frames for the sprites drawn on index (66,67,68)
	NF_SpriteFrame(1,66,0);
	NF_SpriteFrame(1,67,1);
	NF_SpriteFrame(1,68,0);

	// Creates sprites for index 69,70,71. It is a metasprite for the down paddle, with the the x coordinate, and 
	// y coordinates going down by 8. We will end up with 1 sprite made up of three sprites. GFX and VRAM slots are 1,
	// since there is where the source files that contains the sprites used
	NF_CreateSprite(1,69,1,1,d_p->x,d_p->y);
	NF_CreateSprite(1,70,1,1,d_p->x+8,d_p->y);
	NF_CreateSprite(1,71,1,1,d_p->x+16,d_p->y);

	// Set the frames for the sprites drawn on index (69,70,71)
	NF_SpriteFrame(1,69,0);
	NF_SpriteFrame(1,70,1);
	NF_SpriteFrame(1,71,0);


}

void movesSelectPaddles(Paddle* l_p, Paddle* r_p, Paddle* u_p, Paddle* d_p, PaddleChoice playerChoice) {

	// If the up button is held
	if (keysHeld() & KEY_UP) {
		// If the player choice is equal to the left paddle.
		if (playerChoice == LEFT) {
			// Move left_paddle up
			l_p->y = l_p->y - PADDLE_SPEED;

			// Move all the all the sprites of index (60,61,62) with the new value of the left_paddle y position.
			// Remember we do this since all sprites on these indexes form a metasprite that we move up and down 
			// all together.
			NF_MoveSprite(1,60,l_p->x,l_p->y);
			NF_MoveSprite(1,61,l_p->x,l_p->y+8);
			NF_MoveSprite(1,62,l_p->x,l_p->y+16);
		}
		else if (playerChoice == RIGHT) {	// If the player choice is equal to the right paddle.
			// Move right_paddle up
			r_p->y = r_p->y - PADDLE_SPEED;

			// Move all the all the sprites of index (63,64,65) with the new value of the right_paddle y position.
			// Remember we do this since all sprites on these indexes form a metasprite that we move up and down 
			// all together.
			NF_MoveSprite(1,63,r_p->x,r_p->y);
			NF_MoveSprite(1,64,r_p->x,r_p->y+8);
			NF_MoveSprite(1,65,r_p->x,r_p->y+16);
		}
	}
	else if (keysHeld() & KEY_DOWN) { // If the down button is pressed

		// If the player choice is equal to the left paddle.
		if (playerChoice == LEFT) {
			// Move left_paddle down
			l_p->y = l_p->y + PADDLE_SPEED;

			// Move all the all the sprites of index (60,61,62) with the new value of the left_paddle y position.
			// Remember we do this since all sprites on these indexes form a metasprite that we move up and down 
			// all together.
			NF_MoveSprite(1,60,l_p->x,l_p->y);
			NF_MoveSprite(1,61,l_p->x,l_p->y+8);
			NF_MoveSprite(1,62,l_p->x,l_p->y+16);
		}
		else if (playerChoice == RIGHT) {	// If the player choice is equal to the right paddle.
			// Move right_paddle down
			r_p->y = r_p->y + PADDLE_SPEED;

			// Move all the all the sprites of index (63,64,65) with the new value of the left_paddle y position.
			// Remember we do this since all sprites on these indexes form a metasprite that we move up and down 
			// all together.
			NF_MoveSprite(1,63,r_p->x,r_p->y);
			NF_MoveSprite(1,64,r_p->x,r_p->y+8);
			NF_MoveSprite(1,65,r_p->x,r_p->y+16);
		}

	}

	// If the left button pressed
	if (keysHeld() & KEY_LEFT) {
		// If the player choice is equal to the up paddle.
		if (playerChoice == UP) {
			// Move up_paddle left
			u_p->x = u_p->x - PADDLE_SPEED;

			// Move all the all the sprites of index (66,67,68) with the new value of the up_paddle x position.
			// Remember we do this since all sprites on these indexes form a metasprite that we move left and right 
			// all together.
			NF_MoveSprite(1,66,u_p->x,u_p->y);
			NF_MoveSprite(1,67,u_p->x+8,u_p->y);
			NF_MoveSprite(1,68,u_p->x+16,u_p->y);
		}
		else if (playerChoice == DOWN) { // If the player choice is equal to the down paddle
			// Move down_paddle left
			d_p->x = d_p->x - PADDLE_SPEED;


			// Move all the all the sprites of index (69,70,71) with the new value of the down_paddle x position.
			// Remember we do this since all sprites on these indexes form a metasprite that we move left and right 
			// all together.
			NF_MoveSprite(1,69,d_p->x,d_p->y);
			NF_MoveSprite(1,70,d_p->x+8,d_p->y);
			NF_MoveSprite(1,71,d_p->x+16,d_p->y);
		}
	}
	else if (keysHeld() & KEY_RIGHT) {
		// If the player choice is equal to the up paddle
		if (playerChoice == UP) { 
			// Move up_paddle right
			u_p->x = u_p->x + PADDLE_SPEED;

			// Move all the all the sprites of index (66,67,68) with the new value of the up_paddle x position.
			// Remember we do this since all sprites on these indexes form a metasprite that we move left and right 
			// all together.
			NF_MoveSprite(1,66,u_p->x,u_p->y);
			NF_MoveSprite(1,67,u_p->x+8,u_p->y);
			NF_MoveSprite(1,68,u_p->x+16,u_p->y);
		}
		else if (playerChoice == DOWN) {	// If the player choice is equal to the down paddle
			// Move down_paddle right
			d_p->x = d_p->x + PADDLE_SPEED;


			// Move all the all the sprites of index (69,70,71) with the new value of the up_paddle x position.
			// Remember we do this since all sprites on these indexes form a metasprite that we move left and right 
			// all together.
			NF_MoveSprite(1,69,d_p->x,d_p->y);
			NF_MoveSprite(1,70,d_p->x+8,d_p->y);
			NF_MoveSprite(1,71,d_p->x+16,d_p->y);
		}

	}
}

int main(int argc, char **argv) {
	// Set 2D engine on the top screen with mode 0 (Tiled BG)
	NF_Set2D(0,0);
	// Set 2D engine on the bottom screen with mode 0 (Tiled BG)
	NF_Set2D(1,0);
	// Initiate sprite buffers
	NF_InitSpriteBuffers();
	// Init sprite system for bottom screen 1
	NF_InitSpriteSys(1);

	// Set root folder to the nitro filesystem.
	NF_SetRootFolder("NITROFS");

	// Load sprites
	loadSprites();

	// X_bot_limit is assigned to the screen width divided by the tile size for the border boxes
	int x_box_limit = SCREEN_WIDTH/TILE_SIZE;

	// Y_box_limit is assigned to the screen height divided by the tile size for the border boxes
	int y_box_limit = SCREEN_HEIGHT/TILE_SIZE;

	// x1_counter is used to count the number of times to draw a box on the very top of the screen.
	int x1_counter = 0;
	// y1_counter is used to count the number of times to draw a box on the very left of the screen.
	int y1_counter = 0;
	// x2_counter is used to count the number of times to draw a box on the very bottom of the screen.
	int x2_counter = 0;
	// y2_counter is used to count the number of times to draw a box on the very right of the screen.
	int y2_counter = 0;
	int ball_speed = 2;
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

	// Create coordinates, width, height for each paddle.
	createPaddles(&left_paddle,&right_paddle,&up_paddle,&down_paddle);
	createBall(&ball,ball_speed);
	drawBall(&ball);
	// Game loop
	while(1) {

		NF_SpriteOamSet(1);
		scanKeys();
		swiWaitForVBlank();
		oamUpdate(&oamSub);

		if (keysDown() & KEY_R) {
			if (choice == LEFT)
				choice = DOWN;
			else if (choice == DOWN)
				choice = RIGHT;
			else if (choice == RIGHT)
				choice = UP;
			else if (choice == UP)
				choice = LEFT;
		}
		
		if (keysDown() & KEY_L) {
			if (choice == LEFT)
				choice = UP;
			else if (choice == UP)
				choice = RIGHT;
			else if (choice == RIGHT)
				choice = DOWN;
			else if (choice == DOWN)
				choice = LEFT;
		}
		
		// Draw rectangle boxes
		drawBoxes(x1_counter,y1_counter,y2_counter,x2_counter,x_box_limit,y_box_limit);
		// Draw paddles
		drawPaddles(&left_paddle,&right_paddle,&up_paddle,&down_paddle);
		// Handle paddle movement
		movesSelectPaddles(&left_paddle,&right_paddle,&up_paddle,&down_paddle,choice);

		handleBallCollision(&ball,&left_paddle,&right_paddle,&up_paddle,&down_paddle,ball_speed);
	}
	return 0;

}