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
btDiscreteDynamicsWorld* dynamicsWorld;
vector<btRigidBody*> btobjes;

vector<btRigidBody*> getVecList(){
   return btobjes;
}
void physicsInit() {
   btobjes = vector<btRigidBody*>();
   btBroadphaseInterface* broadphase = new btDbvtBroadphase();
   btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
   btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
   btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
   btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
   /*btDiscreteDynamicsWorld* */dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
   dynamicsWorld->setGravity(btVector3(0,-10,0));

   //shapes
   btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);//1m up (y=1)
   //btCollisionShape* fallShape = new btSphereShape(1);
   //btCollisionShape* fallShapeb = new btSphereShape(1);
   //btCollisionShape* fallShapeBox = new btBoxShape(btVector3(1,1,1));

   //ground   
   btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));//-1m (y=1-1=0)

   btRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));//zeros give infinite mass
   /*btRigidBody**/ groundRigidBody = new btRigidBody(groundRigidBodyCI);

   dynamicsWorld->addRigidBody(groundRigidBody);
   /*
   //sphere
   btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));//50m up
   btScalar mass = 1;
   btVector3 fallInertia(0,0,0);//inital velocity?
   fallShape->calculateLocalInertia(mass,fallInertia);//i duknow

   btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
/*   btRigidBody** / fallRigidBody = new btRigidBody(fallRigidBodyCI);

   dynamicsWorld->addRigidBody(fallRigidBody);

   //second
   btDefaultMotionState* fallMotionStateb = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(.5,55,0)));//50m up
   btScalar massb = 1;
   btVector3 fallInertiab(0,0,0);//inital velocity?
   fallShapeb->calculateLocalInertia(massb,fallInertiab);//i duknow

   btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCIb(massb,fallMotionStateb,fallShapeb,fallInertiab);
  /* btRigidBody** / fallRigidBodyb = new btRigidBody(fallRigidBodyCIb);

   dynamicsWorld->addRigidBody(fallRigidBodyb);

   //box
   btDefaultMotionState* fallMotionStateBox = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,1,0)));//50m up
   btVector3 fallInertiaBox(0,0,0);//inital velocity?
   fallShapeBox->calculateLocalInertia(massb,fallInertiaBox);//i duknow

   btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCIBox(0,fallMotionStateBox,fallShapeBox,btVector3(0,0,0));
//                                           groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
  /* btRigidBody** / FRBbuilding = new btRigidBody(fallRigidBodyCIBox);

   dynamicsWorld->addRigidBody(FRBbuilding);
*/
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
void physStep(){
   dynamicsWorld->stepSimulation(1/60.f,10);
}
