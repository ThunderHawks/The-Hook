#ifndef PLAYER_ANIM_H_
#define PLAYER_ANIM_H_

#include "Lab4.h"
#include "mesh_loader.h"
#include "Mesh.h"
#include "physSystem.h"
#include "Camera.h"

/***************
 GLOBALS
***************/
extern int animType;
extern int frame;
extern Mesh mainMesh;

/***************
 PLAYER ANIMATION CLASS
***************/
/*class PlayerAnim {

	public:
		Mesh mainMesh;

		/**************************
		 CONSTRUCTORS
		 *************************/
//		PlayerAnim();

		/***************
		 ANIMATION FUNCTIONS
		***************/
		void Animate(int type, int startFrame, int endFrame);
		void SetPlayer(char *path);
		/* int type 		- the animation type
		 *						  (Only here to check if animation has changed)
		 * int startFrame - the frame that the animation starts on
		 * int duration	- the duration (in frames(!)) the animation lasts for
		 */

//};


#endif
