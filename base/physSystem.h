#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

std::vector<btRigidBody*> getVecList();
void physicsInit();
btRigidBody* createStaticBox(float posX,float posY,float posZ,float scaleX,float scaleY,float scaleZ,btQuaternion rotation,float mass,float ix,float iy,float iz);
void physStep();
