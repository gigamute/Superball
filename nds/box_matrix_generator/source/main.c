#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <nds.h>
#include <nf_lib.h>

#define TILE_SIZE 8
#define SCREEN_WIDTH 256
#define SCREEN_HEIGHT 192
#define TOP_EDGE 16
#define LEFT_EDGE 16
#define OBJ_ARRAY_SIZE 200


typedef struct {
	int x;
	int y;
	int w;
	int h;
	int xSpeed;
	int ySpeed;
} BallObject;

typedef struct {
	int spID;
	bool ACTIVE;
	int x;
	int y;
	int width;
	int height;
} Destructible;

Destructible boxArray[OBJ_ARRAY_SIZE];

void collideBall(BallObject* b) {

	if (b->x < LEFT_EDGE)
		b->xSpeed = -b->xSpeed;
	else if (b->x + b->w > SCREEN_WIDTH - 16)
		b->xSpeed = -b->xSpeed;

	if (b->y < TOP_EDGE)
		b->ySpeed = -b->ySpeed;
	else if (b->y + b->h > SCREEN_HEIGHT - 16)
		b->ySpeed = -b->ySpeed;

	for (int i = 0; i < OBJ_ARRAY_SIZE; i++)
	{
		if (b->x < boxArray[i].x + boxArray[i].width && b->x + b->w > boxArray[i].x) {
			if (boxArray[i].y < b->y + b->h && boxArray[i].y + boxArray[i].height > b->y) {
				b->xSpeed = -b->xSpeed;
				b->ySpeed = -b->ySpeed;
				NF_DeleteSprite(0,boxArray[i].spID);
				for (int j = i; j < OBJ_ARRAY_SIZE - 1; j++)
					memcpy(&boxArray[j],&boxArray[j+1],sizeof(boxArray[j+1]));
			}
		}
	}

	b->x = b->x + b->xSpeed;
	b->y = b->y + b->ySpeed;

	NF_MoveSprite(0,0,b->x,b->y);
}

int main(int argc, char **argv) {
	
	srand(time(NULL));
	int w = 80;
	int h = 80;
	int x = SCREEN_WIDTH/2 - w/2;
	int y = SCREEN_HEIGHT/2 - h/2;
	int spriteID = 1;
	int xBoxAmount = w/TILE_SIZE;
	int yBoxAmount = y/TILE_SIZE;
	BallObject ball;
	ball.w = TILE_SIZE;
	ball.h = TILE_SIZE;
	ball.x = SCREEN_WIDTH/2 - ball.w/2;
	ball.y = 16;
	ball.xSpeed = 2;
	ball.ySpeed = 2;
	int Xamt = 0;
	NF_Set2D(0,0);
	NF_Set2D(1,0);
	NF_InitSpriteBuffers();
	NF_InitSpriteSys(0);
	NF_SetRootFolder("NITROFS");

	NF_LoadSpriteGfx("superball_sprites_2",0,TILE_SIZE,TILE_SIZE);
	NF_LoadSpritePal("superball_sprites_2",0);
	NF_VramSpriteGfx(0,0,0,false);
	NF_VramSpritePal(0,0,0);

	for (int i = x; i < (x + w); i+=8) {
		for (int j = y; j < (y + h); j+=8) {
			Destructible newBox;
			newBox.x = i;
			newBox.y = j;
			newBox.width = TILE_SIZE;
			newBox.height = TILE_SIZE;
			newBox.ACTIVE = true;
			newBox.spID = spriteID;
			memcpy(&boxArray[Xamt],&newBox,sizeof(newBox));
			NF_CreateSprite(0,boxArray[Xamt].spID,0,0,i,j);
			NF_SpriteFrame(0,boxArray[Xamt].spID,0);
			spriteID++;
			Xamt++;
		}
	}
	
	NF_CreateSprite(0,0,0,0,ball.x,ball.y);
	NF_SpriteFrame(0,0,2);
	consoleDemoInit();
	while(1) {
		collideBall(&ball);
		NF_SpriteOamSet(0);
		swiWaitForVBlank();
		consoleClear();
		oamUpdate(&oamMain);
		iprintf("\x1b[32mSuperBall!\n\nBox Matrix Generator Test\n\nBy Josuel Corporan Vargas\n\n");
		printf("The current sprite ID is %i", spriteID);
	}

	return 0;

}
