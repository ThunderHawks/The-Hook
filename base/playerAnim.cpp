#include "playerAnim.h"

int animType = 0;
int frame = 0;

/***************
 CONSTRUCTORS
***************/
playerAnim::Player() {
	player = LoadMesh("../Assets/Models/MainChar.dae");
}

playerAnim::Player(char *path) {
	player = LoadMesh(path);
}

/***************
 ANIMATION FUNCTIONS
***************/
void Animate(int type, int startFrame, int duration) {
	if (animType != type) {
		frame = startFrame;
		
	} else {
		frame = (frame + 1)%duration + startFrame;
	}
	
	PlaceModel(player, physGetPlayerX(), physGetPlayerY() - .7, physGetPlayerZ(), .25, .25, .25, -getYaw()*180/3.14 - 90, frame);
}
