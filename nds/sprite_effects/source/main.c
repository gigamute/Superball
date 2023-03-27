#include <stdio.h>
// Includes propietarios NDS
#include <nds.h>
// Includes librerias propias
#include <nf_lib.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define PARTICLE_ARRAY_SIZE 20

typedef struct {
	int x;
	int y;
	int w;
	int h;
	int xSpeed;
	int ySpeed;
	int sID;

} Particle;

Particle particleArray[PARTICLE_ARRAY_SIZE];

int main(int argc, char **argv) {
	bool PARTICLES_MADE = false;
	int spID = 1;
	int bW = 8;
	int bH = 8;
	int bX = 256/2 - bW/2;
	int bY = 192/2 - bH/2;
	NF_Set2D(0,0);
	NF_Set2D(1,0);

	NF_InitTiledBgBuffers();
	NF_InitTiledBgSys(0);
	NF_InitTiledBgSys(1);
	
	NF_InitSpriteBuffers();
	NF_InitSpriteSys(0);
	NF_InitSpriteSys(1);

	NF_SetRootFolder("NITROFS");

	NF_LoadSpriteGfx("superball_sprites_2",0,bW,bH);
	NF_LoadSpritePal("superball_sprites_2",0);
	NF_VramSpriteGfx(1,0,0,false);
	NF_VramSpritePal(1,0,0);
	NF_CreateSprite(1,0,0,0,bX,bY);
	int x_bound_1 = bX + 8;
	int x_bound_2 = bX;

	int y_bound_1 = bY + 8;
	int y_bound_2 = bY;
	//consoleDemoInit();

	while(1) {
		//NF_SpriteFrame(1,0,index);
		if (keysDown() & KEY_A) {
			if (!PARTICLES_MADE) {
				srand(time(NULL));
				NF_DeleteSprite(1,0);
				int n1 = 1;
				int n2 = -1;
				for (int i = 0; i < PARTICLE_ARRAY_SIZE; i++) {
					Particle newParticle;
					newParticle.w = bW;
					newParticle.h = bH;
					newParticle.x = rand() % ((bX + bW + 16) + 1 - (bX - 16)) + (bX + 16);
					newParticle.y = rand() % ((bY + bH) + 1 - bY) + bY;
					newParticle.sID = spID;
					int randomSpeed = rand() % (1000 + 1 - 0) + 0;
					int randomSpeed2 = rand() % (1000 + 1 - 0) + 0;
					newParticle.xSpeed = (randomSpeed % 2 == 0) ? n1 : n2;
					newParticle.ySpeed = (randomSpeed2 % 4 == 0) ? n2 : n1;
					printf("%i ",randomSpeed);
					spID++;
					NF_CreateSprite(1,newParticle.sID,0,0,newParticle.x,newParticle.y);
					NF_SpriteFrame(1,newParticle.sID,4);
					memcpy(&particleArray[i],&newParticle,sizeof(newParticle));
					int temp = n1;
					n1 = n2;
					n2 = temp;
					
				}
				PARTICLES_MADE = true;
			}
			
		}

		if (PARTICLES_MADE) {
			
			for (int i = 0; i < PARTICLE_ARRAY_SIZE; i++) {
				particleArray[i].x += particleArray[i].xSpeed;
				particleArray[i].y += particleArray[i].ySpeed;
				
				NF_MoveSprite(1,particleArray[i].sID,particleArray[i].x,particleArray[i].y);
			}
		}
		scanKeys();
		NF_SpriteOamSet(1);
		swiWaitForVBlank();
		oamUpdate(&oamSub);

	}

	return 0;

}
