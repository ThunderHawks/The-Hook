#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

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
//   printf("%f %f %f speedy\n",30*x,30*y,30*z);
   player->setLinearVelocity(btVector3(3*x,3*y+player->getLinearVelocity().getY(),3*z));
  // player->clearForces();
//   player->applyCentralForce(btVector3(x*300,200*y,300*z));
   //btobjes[0]->setLinearVelocity(btVector3(0,1000,0));
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
   dynamicsWorld->setGravity(btVector3(0,-15,0));

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
   player = createStaticBox(1,1,1,1,1,1,btQuaternion(0,0,0,1),1,0,0,0);
   player->setSleepingThresholds (0, 0);
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
glm::vec3 dir;
btVector3 tmp;
void physGrapple(float lx,float ly,float lz){
   dir = glm::normalize(glm::vec3(-lx,ly,-lz));
   printf("grapple in dir %f %f %f\n",dir.x,dir.y,dir.z);
   printf("looks at is %f %f %f\n",lookAtPoint.x,lookAtPoint.y,lookAtPoint.z);
   /*   btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(lookAtPoint.x+.0*dir.x,lookAtPoint.y-.0*dir.y,lookAtPoint.z+.0*dir.z), btVector3(lookAtPoint.x+50*dir.x,lookAtPoint.y-50*dir.y,lookAtPoint.z+50*dir.z));
   dynamicsWorld->rayTest(btVector3(lookAtPoint.x+.0*dir.x,lookAtPoint.y-.0*dir.y,lookAtPoint.z+.0*dir.z), btVector3(lookAtPoint.x+50*dir.x,lookAtPoint.y-50*dir.y,lookAtPoint.z+50*dir.z), RayCallback);
*/
   btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(lookAtPoint.x+1*dir.x,lookAtPoint.y+1*dir.y+1,lookAtPoint.z+1*dir.z), btVector3(lookAtPoint.x+50*dir.x,lookAtPoint.y+50*dir.y,lookAtPoint.z+50*dir.z));
   dynamicsWorld->rayTest(btVector3(lookAtPoint.x+1*dir.x,lookAtPoint.y+1*dir.y+1,lookAtPoint.z+1*dir.z), btVector3(lookAtPoint.x+50*dir.x,lookAtPoint.y+50*dir.y,lookAtPoint.z+50*dir.z), RayCallback);
   //player->setLinearVelocity(btVector3(dir.x*50,dir.y*50,dir.z*50));
   if(RayCallback.hasHit()) {
    //End = RayCallback.m_hitPointWorld;
    //Normal = RayCallback.m_hitNormalWorld;
      printf("hit!\n");
      btVector3 go=RayCallback.m_hitNormalWorld*-5+player->getLinearVelocity();
      if (go.length()>10)(go/go.length())*10;
      player->setLinearVelocity(go);
      printf("%f %f %f on hit norm",RayCallback.m_hitNormalWorld.getX(),RayCallback.m_hitNormalWorld.getY(),RayCallback.m_hitNormalWorld.getZ());
      tmp = RayCallback.m_hitPointWorld;
    // Do some clever stuff here
   }
}
void physSetDisplayObj(btRigidBody* phys, void *obj){
   phys->setUserPointer(obj);
}
btVector3 plsRndr(){
   return tmp;
   //return btVector3(lookAtPoint.x+3*dir.x,lookAtPoint.y-3*dir.y,lookAtPoint.z+3*dir.z);
}
void physStep(){
   //player->getX();
   //setPlayerSpeed(2,2,2);

   dynamicsWorld->stepSimulation(1/60.f,10);
   lookAtPoint = glm::vec3(physGetPlayerX(),physGetPlayerY(),physGetPlayerZ());
   glfwGetCursorPos(NULL,g_width/2.0,g_height/2.0);
}
