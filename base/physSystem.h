#ifndef PHYS_H_
#define PHYS_H_

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <vector>
#include "glm/glm.hpp"

void physGrapple(float lx, float ly, float lz);
std::vector<btRigidBody*> getVecList();
void physicsInit();
void physGetPlayerLocation(btTransform);
void setPlayerSpeed(float x, float y, float z);
btRigidBody* createStaticBox(float posX,float posY,float posZ,float scaleX,float scaleY,float scaleZ,btQuaternion rotation,float mass,float ix,float iy,float iz);
void physStep();
float physGetPlayerX();
float physGetPlayerY();
float physGetPlayerZ();
void physSetDisplayObj(btRigidBody* phys, void* obj);
void setPlayerSpeed(float x,float y,float z);
btVector3 plsRndr();
void physGrapplePoint();
/*returns a vector that is the speed of the player as a vec3*/
glm::vec3 getPlayerSpeed();
glm::vec3 grapplingHookLocation();
int isGrappleActive();

#endif
