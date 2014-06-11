#ifndef PLAYER_ANIM_H_
#define PLAYER_ANIM_H_

#include "mesh_loader.h"
#include "Mesh.h"
#include "physSystem.h"
#include "Camera.h"

/***************
 GLOBALS
***************/
extern int animType;
extern int frame;

/***************
 PLAYER ANIMATION CLASS
***************/
class Player {

	public:
		Mesh player;
		
		/***************
		 CONSTRUCTORS
		***************/
		Player(); 				//Default constructor
		Player(char *path);	//Custom constructor
		
		/***************
		 ANIMATION FUNCTIONS
		***************/
		void Animate(int type, int startFrame, int duration);
		/* int type 		- the animation type
		 *						  (Only here to check if animation has changed)
		 * int startFrame - the frame that the animation starts on
		 * int duration	- the duration (in frames(!)) the animation lasts for
		 */
}


#endif
