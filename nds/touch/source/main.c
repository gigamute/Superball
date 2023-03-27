#include <stdio.h>
#include <nds.h>
#include <nf_lib.h>





typedef struct
{
	int x;
	int y;
	int width;
	int height;
	int spID;
} Box;


int main(int argc, char **argv) {
	touchPosition touch;
	Box sprite;
	sprite.x = 100;
	sprite.y = 100;
	sprite.width = 8;
	sprite.height = 8;
	sprite.spID = 0;
    NF_Set2D(0,0);
	NF_Set2D(1,0);
	NF_InitSpriteBuffers();
	NF_InitSpriteSys(1);
	NF_SetRootFolder("NITROFS");

	NF_LoadSpriteGfx("superball_sprites_2",0,8,8);
	NF_LoadSpritePal("superball_sprites_2",0);
	NF_VramSpriteGfx(1,0,0,false);
	NF_VramSpritePal(1,0,0);
	NF_CreateSprite(1,sprite.spID,0,0,sprite.x,sprite.y);
	NF_SpriteFrame(1,sprite.spID,0);
	while(1) {
		touchRead(&touch);
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamSub);
		scanKeys();
		if (touch.px < sprite.x + sprite.width && touch.px > sprite.x) {
			if (touch.py < sprite.y + sprite.height && touch.py > sprite.y) {
				if (keysHeld() & KEY_TOUCH) {
					sprite.x = touch.px - 4;
				}
			}
		}

		NF_MoveSprite(1,0,sprite.x,sprite.y);

	}

	return 0;

}
