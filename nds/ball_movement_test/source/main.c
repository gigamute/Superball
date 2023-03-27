#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>

#define TOP_EDGE 16
#define RIGHT_EDGE 256
#define LEFT_EDGE 16
#define BOTTOM_EDGE 192
#define TILESIZE 16

typedef struct {
	
	int xSpeed;
	int ySpeed;
	int x;
	int y;
	int width;
	int height;
} Ball;

int main(int argc, char **argv) {
	Ball obj;
	obj.width = 8;
	obj.height = 8;
	obj.x = 256/2 - obj.width/2;
	obj.y = 192/2 - obj.height/2;
	obj.xSpeed = 3;
	obj.ySpeed = 3;

	NF_Set2D(0,0);
	NF_Set2D(1,0);

	NF_InitSpriteBuffers();
	NF_InitSpriteSys(1);

	NF_SetRootFolder("NITROFS");

	NF_LoadSpriteGfx("superball_sprites_2",0,8,8);
	NF_LoadSpritePal("superball_sprites_2",0);
	NF_VramSpriteGfx(1,0,0,false);
	NF_VramSpritePal(1,0,0);

	NF_CreateSprite(1,0,0,0,obj.x,obj.y);
	NF_SpriteFrame(1,0,2);

	while(1) {

		if (obj.x < LEFT_EDGE) {
			obj.x = LEFT_EDGE;
			obj.xSpeed = -obj.xSpeed;

		}
		else if (obj.x + obj.width >= RIGHT_EDGE - TILESIZE)
		{
			obj.x = RIGHT_EDGE - TILESIZE - obj.width;
			obj.xSpeed = -obj.xSpeed;
		}

		if (obj.y < TOP_EDGE)
		{
			obj.y = TOP_EDGE;
			obj.ySpeed = -obj.ySpeed;
		}
		else if (obj.y + obj.height >= BOTTOM_EDGE - TILESIZE)
		{
			obj.y = BOTTOM_EDGE - TILESIZE - obj.height;
			obj.ySpeed = -obj.ySpeed;
		}

		obj.x = obj.x + obj.xSpeed;
		obj.y = obj.y + obj.ySpeed;

		NF_MoveSprite(1,0,obj.x,obj.y);

		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamSub);	

	}

	return 0;

}
