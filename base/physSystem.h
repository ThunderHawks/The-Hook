#ifndef PHYS_H_
#define PHYS_H_

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>
#include <vector>

#ifndef GLM_HPP_
#define GLM_HPP_
#include "glm/glm.hpp"
#endif 

bool physGrapple(float lx, float ly, float lz);
std::vector<btRigidBody*> getVecList();
void physicsInit();
void physGetPlayerLocation(btTransform);
void setPlayerSpeed(float x, float y, float z);
btRigidBody* createStaticBox(float posX,float posY,float posZ,float scaleX,float scaleY,float scaleZ,btQuaternion rotation,float mass,float ix,float iy,float iz);
btRigidBody* createStaticSphere(float posX,float posY,float posZ,float scaleX,float scaleY,float scaleZ,btQuaternion rotation,float mass,float ix,float iy,float iz);
void physStep(float time);
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
void physJump();
int canMove();
float physGetHeight(float x, float y);
void holdGrapple(int i);
glm::vec3 playerPoss();

int grappleInRange(float lx, float ly, float lz);
//Made by Andrew. This allows the player to let go of the grapple.
void ReleaseGrapple();

#endif
