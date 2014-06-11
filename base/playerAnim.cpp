#include "playerAnim.h"

#include <stdio.h>

int animType = 0;
int frame = 0;
Mesh mainMesh;

/***************
 CONSTRUCTORS
***************/
/*PlayerAnim::PlayerAnim() {
	mainMesh = Mesh();
}


/***************
 ANIMATION FUNCTIONS
***************/
void Animate(int type, int startFrame, int endFrame) {
	static unsigned int increment = 0;
	glm::vec3 holdSpeed;
	//printf("%d frame %d animType\n", frame, animType);
	if (increment%3 == 0) {
		if (animType != type) {
			animType = type;
			frame = startFrame;
		} else {
			if (type == START_RUN && frame == endFrames[START_RUN]) {
				animType = RUN;
				anim = RUN;
				frame = startFrames[RUN];
			} else if (type == RUN && frame == endFrames[RUN]) {
				holdSpeed = getPlayerSpeed();
				if (holdSpeed.x < .01f && holdSpeed.y < .01f && holdSpeed.z < .01f) {
					animType = IDLE;
					anim = IDLE;
					frame = startFrames[IDLE];
				} else
					frame = startFrames[RUN];
			} else if (type == FIRE && frame == endFrames[FIRE]) {
				animType = FLAIL;
				anim = FLAIL;
				frame = startFrames[FLAIL];
			} else if (type == JUMP && frame == endFrames[JUMP]) {
				animType = LAND;
				anim = LAND;
				frame = startFrames[LAND];
			}
			else if (type == LAND && frame == endFrames[LAND]) {
				animType = IDLE;
				anim = IDLE;
				frame = startFrames[IDLE];
			}
			else if (type == IDLE && frame == endFrames[IDLE]) {
				frame = startFrames[IDLE];
			}
			else if (type == FLAIL && frame == endFrames[FLAIL]) {
				if (isGrappleActive())
					frame = startFrames[FLAIL];
				else{
					animType = CRASH;
					anim = CRASH;
					frame = startFrames[CRASH];
				}
			}
			else if (type == CRASH && frame == endFrames[CRASH]) {
				animType = START_RUN;
				anim = START_RUN;
				frame = startFrames[START_RUN];
			}
			else {
				if (animType != IDLE || (animType == IDLE && increment%2))
					frame++;
			}
		
	}
	}
	++increment;
	

	PlaceModel(mainMesh, physGetPlayerX(), physGetPlayerY() - 1.5, physGetPlayerZ(), .25, .25, .25, -getYaw()*180/3.14 - 90, frame);
}

void SetPlayer(char *path) {
	mainMesh = LoadMesh(path);
}
