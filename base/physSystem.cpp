#include "Lab4.h"
#include "InitHelpers.h"
#include "Helper.h"
#include "IO.h"
#include "Shapes.h"
#include "physSystem.h"
#include <vector>

//phys//
btRigidBody* groundRigidBody;
//btRigidBody* fallRigidBody;
//btRigidBody* fallRigidBodyb;
//btRigidBody* FRBbuilding;
btRigidBody* player;
btDiscreteDynamicsWorld* dynamicsWorld;
vector<btRigidBody*> btobjes;

vector<btRigidBody*> getVecList(){
   return btobjes;
}
void setPlayerSpeed(float x, float y, float z){
   printf("%f %f %f speedy\n",x,y,z);
   //player->setLinearVelocity(btVector3(x,y,z));
   btobjes[0]->setLinearVelocity(btVector3(0,1000,0));
}
void physicsInit() {
   //vector list of all non-ground and non-player objects
   btobjes = vector<btRigidBody*>();
   //bullet algorithm
   btBroadphaseInterface* broadphase = new btDbvtBroadphase();
   btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
   btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
   btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
   btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
   /*btDiscreteDynamicsWorld* */dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
   //end of bullet setup   
   dynamicsWorld->setGravity(btVector3(0,-10,0));

   //shapes
   btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);//1m up (y=1)
   //btCollisionShape* fallShape = new btSphereShape(1);
   //btCollisionShape* fallShapeb = new btSphereShape(1);
   //btCollisionShape* fallShapeBox = new btBoxShape(btVector3(1,1,1));

   //ground   
   //sets the ground up with a location
   btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));//-1m (y=1-1=0)

   
   btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));//zeros give infinite mass
   /*btRigidBody**/ groundRigidBody = new btRigidBody(groundRigidBodyCI);

   dynamicsWorld->addRigidBody(groundRigidBody);

   //player
//   printf("%d is pl point\n",player);
   player = createStaticBox(eye.x,eye.y,eye.z,1,1,1,btQuaternion(0,0,0,1),1,0,0,0);
  // printf("%d is pl point\n",player);
   
}
btRigidBody* createStaticBox(float posX,float posY,float posZ,float scaleX,float scaleY,float scaleZ,btQuaternion rotation,float mass,float ix,float iy,float iz){
   btCollisionShape* fallShapeBoxC = new btBoxShape(btVector3(scaleX,scaleY,scaleZ));
   //box
   btDefaultMotionState* fallMotionStateb = new btDefaultMotionState(btTransform(rotation,btVector3(posX,posY,posZ)));//50m up
   btScalar massb = mass;
   btVector3 fallInertiab(0,0,0);//inital velocity?
   fallShapeBoxC->calculateLocalInertia(massb,fallInertiab);//i duknow

   btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCIb(massb,fallMotionStateb,fallShapeBoxC,fallInertiab);
   btRigidBody* FRBbox = new btRigidBody(fallRigidBodyCIb);

   dynamicsWorld->addRigidBody(FRBbox);
   FRBbox->setLinearVelocity(btVector3(ix,iy,iz));
   btobjes.push_back(FRBbox);
   return FRBbox;
}
/*void physGetPlayerLocation(btTransform *trans){
   //trans = 
   btobjes[0]->getMotionState()->getWorldTransform(trans);
}*/
float physGetPlayerX(){
   btTransform trans;
   btobjes[0]->getMotionState()->getWorldTransform(trans);
   return trans.getOrigin().getX();
}
float physGetPlayerY(){
   btTransform trans;
   btobjes[0]->getMotionState()->getWorldTransform(trans);
   return trans.getOrigin().getY();
}
float physGetPlayerZ(){
   btTransform trans;
   btobjes[0]->getMotionState()->getWorldTransform(trans);
   return trans.getOrigin().getZ();
}
void physGrapple(float x,float y,float z,float lx,float ly,float lz){
   
}
void physStep(){
   //player->getX();
   dynamicsWorld->stepSimulation(1/60.f,10);
}
