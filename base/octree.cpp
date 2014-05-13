#include "octree.h"

//Adds a bounding sphere to or removes one from the children of
//current octree
void Octree::handleOctree(Entity* bs, glm::vec3 pos, bool addBS) {
	//Figure out in which child(ren) the bs belongs
	for(int x = 0; x < 2; x++) {
		if (x == 0) {
			if (pos[0] - bs->BSRadius > center[0]) {
				continue;
			}
		}
		else if (pos[0] + bs->BSRadius < center[0]) {
			continue;
		}
		
		for(int y = 0; y < 2; y++) {
			if (y == 0) {
				if (pos[1] - bs->BSRadius > center[1]) {
					continue;
				}
			}
			else if (pos[1] + bs->BSRadius < center[1]) {
				continue;
			}
			
			for(int z = 0; z < 2; z++) {
				if (z == 0) {
					if (pos[2] - bs->BSRadius > center[2]) {
						continue;
					}
				}
				else if (pos[2] + bs->BSRadius < center[2]) {
					continue;
				}
						
				//Add or remove the bounding sphere
				if (addBS) {
					children[x][y][z]->add(bs);
				}
				else {
					children[x][y][z]->removeBS(bs, pos);
				}
			}
		}
	}
}

//Creates children of current octree, and moves the
//bounding spheres in this octree to the children tree
void Octree::makeChildren() {
	for(int x = 0; x < 2; x++) {
		float minX;
		float maxX;
		if (x == 0) {
			minX = corner1[0];
			maxX = center[0];
		}
		else {
			minX = center[0];
			maxX = corner2[0];
		}
				
		for(int y = 0; y < 2; y++) {
			float minY;
			float maxY;
			if (y == 0) {
				minY = corner1[1];
				maxY = center[1];
			}
			else {
				minY = center[1];
				maxY = corner2[1];
			}
					
			for(int z = 0; z < 2; z++) {
				float minZ;
				float maxZ;
				if (z == 0) {
					minZ = corner1[2];
					maxZ = center[2];
				}
				else {
					minZ = center[2];
					maxZ = corner2[2];
				}
						
				children[x][y][z] = new Octree(glm::vec3(minX, minY, minZ),
											   glm::vec3(maxX, maxY, maxZ),
											   depth + 1);
			}
		}
	}
			
	//Remove all bses from "bsSet" and add them to the new children
	for(set<Entity*>::iterator it = bsSet.begin(); it != bsSet.end();
			it++) {
		Entity* bs = *it;
		handleOctree(bs, bs->position, true);
	}
	bsSet.clear();
	
	hasChildren = true;
}
		
//Adds all bses in this or one of its descendants to the specified set
void Octree::collectBS(set<Entity*> &bsSet) {
	if (hasChildren) {
		for(int x = 0; x < 2; x++) {
			for(int y = 0; y < 2; y++) {
				for(int z = 0; z < 2; z++) {
					children[x][y][z]->collectBS(bsSet);
				}
			}
		}
	}
	else {
		for(set<Entity*>::iterator it = bsSet.begin(); it != bsSet.end();
				it++) {
			Entity* bs = *it;
			bsSet.insert(bs);
		}
	}
}
		
//Destroys the children of current octree, and moves all bsess in its descendants
//to the "bsSet"
void Octree::destroyChildren() {
	//Move all balls in descendants of this to the "balls" set
	collectBS(this->bsSet);
	
	for(int x = 0; x < 2; x++) {
		for(int y = 0; y < 2; y++) {
			for(int z = 0; z < 2; z++) {
				delete children[x][y][z];
			}
		}
	}
	
	hasChildren = false;
}
	
//Removes the specified bounding sphere at the indicated position
void Octree::removeBS(Entity* bs, glm::vec3 pos) {
	numBS--;
	
	if (hasChildren && numBS < MIN_BS_PER_OCTREE) {
		destroyChildren();
	}
	
	if (hasChildren) {
		handleOctree(bs, pos, false);
	}
	else {
		bsSet.erase(bs);
	}
}
	
/* Function to check potential collision between a bounding sphere
	 * and a wall.
	 * - cs: Stores possible collision to cs.
	 * - w : the side of wall that is going to collide
	 * - coord : relevant coordinate of the wall in axis
	 * - direct: direction of the wall from bounding sphere. 
	 *           if wall in negative direction -> 0
	 *           if wall in positive direction -> 1
	 */ 
void Octree::BSWallCollision(vector<BS_Wall> &cs, Wall w, 
					 char coord, int direct) {
	if (hasChildren) {
		//Recursively call potentialBallWallCollisions on the correct
		//half of the children (e.g. if w is WALL_TOP, call it on
		//children above centerY)
		for(int direct2 = 0; direct2 < 2; direct2++) {
			for(int direct3 = 0; direct3 < 2; direct3++) {
				Octree *child;
				switch (coord) {
					case 'x':
						child = children[direct][direct2][direct3];
						break;
					case 'y':
						child = children[direct2][direct][direct3];
						break;
					case 'z':
						child = children[direct2][direct3][direct];
						break;
				}
				
				child->BSWallCollision(cs, w, coord, direct);
			}
		}
	}
	else {
		//Add (bs, w) for all bounding spheres in current octree
		for(set<Entity*>::iterator it = bsSet.begin(); it != bsSet.end();
				it++) {
			Entity* bs = *it;
			BS_Wall bwp;
			bwp.bs = bs;
			bwp.wall = w;
			cs.push_back(bwp);
		}
	}
}

//Constructs a new Octree.  c1 is (minX, minY, minZ), c2 is (maxX, maxY,
//maxZ), and d is the depth, which starts at 1.
Octree::Octree(glm::vec3 c1, glm::vec3 c2, int d) {
	corner1 = c1;
	corner2 = c2;
	center = glm::vec3(c1[0] + c2[0] / 2, c1[1] + c2[1] / 2, c1[2] + c2[2] / 2);
	depth = d;
	numBS = 0;
	hasChildren = false;
}

//Destructor
Octree::~Octree() {
	if (hasChildren) {
		destroyChildren();
	}
}
		
//Adds a bouding sphere
void Octree::add(Entity* bs) {
	numBS++;
	if (!hasChildren && depth < MAX_OCTREE_DEPTH &&
		numBS > MAX_BS_PER_OCTREE) {
		makeChildren();
	}
	
	if (hasChildren) {
		handleOctree(bs, bs->position, true);
	}
	else {
		bsSet.insert(bs);
	}
}
		
//Removes a bounding sphere
void Octree::remove(Entity* bs) {
	removeBS(bs, bs->position);
}

//Changes the position of a bs in this from oldPos to bs->pos
void Octree::bsMoved(Entity* bs, glm::vec3 oldPos) {
	removeBS(bs, oldPos);
	add(bs);
}
		
//Adds potential bs-bs collisions to the specified set
void Octree::BSBSCollision(vector<BS_BS> &col) {
	if (hasChildren) {
		for(int x = 0; x < 2; x++) {
			for(int y = 0; y < 2; y++) {
				for(int z = 0; z < 2; z++) {
					children[x][y][z]->BSBSCollision(col);
				}
			}
		}
	}
	else {
		//Add all pairs (bs1, bs2) from bsSet
		for(set<Entity*>::iterator it = bsSet.begin(); it != bsSet.end(); it++) {
			Entity* bs1 = *it;
			for(set<Entity*>::iterator it2 = bsSet.begin();	it2 != bsSet.end(); it2++) {
				Entity* bs2 = *it2;
				//This test makes sure that we only add each pair once
				if (bs1 < bs2) {
					BS_BS bp;
					bp.bs1 = bs1;
					bp.bs2 = bs2;
					col.push_back(bp);
				}
			}
		}
	}
}

//Adds potential bs-wall collisions to the specified set
void Octree::BSWallCollision(vector<BS_Wall> &col) {
	BSWallCollision(col, WALL_LEFT, 'x', 0);
	BSWallCollision(col, WALL_RIGHT, 'x', 1);
	BSWallCollision(col, WALL_BOTTOM, 'y', 0);
	BSWallCollision(col, WALL_TOP, 'y', 1);
	BSWallCollision(col, WALL_FAR, 'z', 0);
	BSWallCollision(col, WALL_NEAR, 'z', 1);
}

