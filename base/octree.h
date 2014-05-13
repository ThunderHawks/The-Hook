/* Octree by Richard

   Simple implementation of octree.
   I am assuming that we are using bounding spheres for all
   the object used in our game including the camera.

   If we need to add cube objects, I need to add things for
   cubes.

   Only works with spheres and balls :)

 */

#ifndef OCTREE_H
#define OCTREE_H

#include <set>
#include <vector>
#include <cstdlib>
#include <iostream>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#include "level.h"

using namespace std;

enum Wall {WALL_LEFT, WALL_RIGHT, WALL_FAR, WALL_NEAR, WALL_TOP, WALL_BOTTOM};

//Stores a pair of balls
struct BS_BS {
	Entity* bs1;
	Entity* bs2;
};

//Stores a ball and a wall
struct BS_Wall {
	Entity* bs;
	Wall wall;
};

const int MAX_OCTREE_DEPTH = 6;
const int MIN_BS_PER_OCTREE = 3;
const int MAX_BS_PER_OCTREE = 6;

class Octree{
private:
	glm::vec3 corner1;
	glm::vec3 corner2;
	glm::vec3 center;

	//Children of current cube. 
	Octree* children[2][2][2];

	bool hasChildren;  //check to see if current cube has children

	set<Entity*> bsSet;   //'bs' stands for bounding sphere

	int depth;
	int numBS;

	//Adds a bounding sphere to or removes one from the children of
	//current octree
	void handleOctree(Entity* bs, glm::vec3 pos, bool addBS);

	//Creates children of current octree, and moves the
	//bounding spheres in this octree to the children tree
	void makeChildren();

	//Adds all bses in this or one of its descendants to the specified set
	void collectBS(set<Entity*> &bsSet);

	//Destroys the children of current octree, and moves all bsess in its descendants
	//to the "bsSet"
	void destroyChildren();
	
	//Removes the specified bounding sphere at the indicated position
	void removeBS(Entity* bs, glm::vec3 pos);

	/* Function to check potential collision between a bounding sphere
	 * and a wall.
	 * - cs: Stores possible collision to cs.
	 * - w : the side of wall that is going to collide
	 * - coord : relevant coordinate of the wall in axis
	 * - direct: direction of the wall from bounding sphere. 
	 *           if wall in negative direction -> 0
	 *           if wall in positive direction -> 1
	 */ 
	void BSWallCollision(vector<BS_Wall> &cs, Wall w, 
						 char coord, int direct);

public:
	Octree(glm::vec3 c1, glm::vec3 c2, int d);
	~Octree();

	//Adds a bouding sphere
	void add(Entity* bs);

	//Removes a bounding sphere
	void remove(Entity* bs);

	//Changes the position of a ball in this from oldPos to bs->pos
	void bsMoved(Entity* bs, glm::vec3 oldPos);

	//Adds potential bs-bs collisions to the specified set
	void BSBSCollision(vector<BS_BS> &col);

	//Adds potential bs-wall collisions to the specified set
	void BSWallCollision(vector<BS_Wall> &col);
	
};

#endif //end of OCTREE_H