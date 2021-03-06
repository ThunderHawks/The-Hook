#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

#include "Lab4.h"
#include "InitHelpers.h"
#include "Helper.h"
#include "Shapes.h"
#include "IOGame.h"
#include "IOEdit.h"
#include "Mesh.h"
#include "physSystem.h"
#include "Camera.h"
//#include <bullet/btCollisionWorld.h>
#include "sound.h"
#include <vector>
#include "particle.h"
#include "camBox.h"
#include "IOGame.h"

//phys//
btRigidBody* groundRigidBody;
btRigidBody* player;
btDiscreteDynamicsWorld* dynamicsWorld;
//btCollisionShape* ground;
vector<btRigidBody*> btobjes;
//Mesh chara;
glm::vec3 lookAt;
float curXsp,curYsp,curZsp;
int playerGrappleActive = 0;
int playerFall = 1;
int playerJump = 1;
int hold = 0;

vector<btRigidBody*> getVecList(){
   return btobjes;
}

void setPlayerSpeed(float x, float y, float z){
   curXsp += x;
   curYsp += y;
   curZsp += z;
}
void AsetPlayerSpeed(float x, float y, float z){
//   printf("%f %f %f speedy\n",30*x,30*y,30*z);
   if(!playerGrappleActive)
      player->setLinearVelocity(btVector3(1*x+player->getLinearVelocity().getX()*(playerJump?.8:.8),1*y+player->getLinearVelocity().getY(),1*z+player->getLinearVelocity().getZ()*(playerJump?.8:.8)));
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
//   dynamicsWorld->setGravity(btVector3(0,-15,0));
//   dynamicsWorld->setGravity(btVector3(0,-30,0));
   dynamicsWorld->setGravity(btVector3(0,-60,0));

   //shapes
   btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);//1m up (y=1)
//   ground = groundShape;
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
   player = createStaticSphere(1,1,1,1.7,1,1,btQuaternion(0,0,0,1),1,0,0,0);
   
   player->setSleepingThresholds (0, 0);
   //chara = LoadMesh("../Assets/Models/topHatChar.obj");
   //physSetDisplayObj(player,&chara);

   float x,y,z;
   x = physGetPlayerX();
   y = physGetPlayerY();
   z = physGetPlayerZ();
   lookAt = glm::vec3(x,y,z);
//   lookAt = glm::vec3(physGetPlayerX(),physGetPlayerY,physGetPlayerZ);
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
   //printf("%d is num\n",btobjes.size());
   return FRBbox;
}
btRigidBody* createStaticSphere(float posX,float posY,float posZ,float scaleX,float scaleY,float scaleZ,btQuaternion rotation,float mass,float ix,float iy,float iz){
   btCollisionShape* fallShapeBoxC = new btSphereShape(scaleX);
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
   //printf("%d is num\n",btobjes.size());
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
int flip;


int grappleInRange(float lx,float ly,float lz){
   //glm::vec3 tempLookAt;

   dir = glm::normalize(glm::vec3(-lx,ly,-lz));

   glm:: vec3 ggaze = GetLookAt() - GetEye();
   glm::vec3 gw = ggaze/magnitude(ggaze);
   gw = glm::vec3(-1.0 * gw.x, -1.0 * gw.y, -1.0 * gw.z);
   glm::vec3 gu = glm::cross(GetUp(), gw)/magnitude(glm::cross(GetUp(), gw));
   gu *= 2;

   lookAt += glm::vec3(gu.x, 0, gu.z);
   
   btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(lookAt.x+1.6*dir.x,lookAt.y-1.6*dir.y,lookAt.z+1.6*dir.z), btVector3(lookAt.x+75*dir.x,lookAt.y-75*dir.y,lookAt.z+75*dir.z));
   dynamicsWorld->rayTest(btVector3(lookAt.x+1.6*dir.x,lookAt.y-1.6*dir.y,lookAt.z+1.6*dir.z), btVector3(lookAt.x+75*dir.x,lookAt.y-75*dir.y,lookAt.z+75*dir.z), RayCallback);

   lookAt -= glm::vec3(gu.x, 0, gu.z);
//   int ret = //inRange(50,75,glm::lookAt(playerPoss(),glm::vec3(tempLookAt.x+75*dir.x,tempLookAt.y+75*dir.y,tempLookAt.z+75*dir.z),glm::vec3(0,1,0)));
   //printf("grapple is %d range\n",RayCallback.hasHit());
   //if(RayCallback.hasHit())   printf("%f %f %f",RayCallback.m_hitPointWorld.getX(),RayCallback.m_hitPointWorld.getY(),RayCallback.m_hitPointWorld.getZ());
   return    RayCallback.hasHit();
}


bool physGrapple(float lx,float ly,float lz){
	bool ret = false;
   flip = 0;
   dir = glm::normalize(glm::vec3(-lx,ly,-lz));



   //printf("grapple in dir %f %f %f\n",dir.x,dir.y,dir.z);
   //printf("looks at is %f %f %f\n",lookAt.x,lookAt.y,lookAt.z);
   /*   btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(lookAt.x+.0*dir.x,lookAt.y-.0*dir.y,lookAt.z+.0*dir.z), btVector3(lookAt.x+50*dir.x,lookAt.y-50*dir.y,lookAt.z+50*dir.z));
   dynamicsWorld->rayTest(btVector3(lookAt.x+.0*dir.x,lookAt.y-.0*dir.y,lookAt.z+.0*dir.z), btVector3(lookAt.x+50*dir.x,lookAt.y-50*dir.y,lookAt.z+50*dir.z), RayCallback);
*/

//////////////////////////////
   glm:: vec3 ggaze = GetLookAt() - GetEye();
   glm::vec3 gw = ggaze/magnitude(ggaze);
   gw = glm::vec3(-1.0 * gw.x, -1.0 * gw.y, -1.0 * gw.z);
   glm::vec3 gu = glm::cross(GetUp(), gw)/magnitude(glm::cross(GetUp(), gw));
   gu *= 2;

   lookAt += glm::vec3(gu.x, 0, gu.z);
   ///////////////////////////////


   btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(lookAt.x+1.6*dir.x,lookAt.y-1.6*dir.y,lookAt.z+1.6*dir.z), btVector3(lookAt.x+75*dir.x,lookAt.y-75*dir.y,lookAt.z+75*dir.z));
   dynamicsWorld->rayTest(btVector3(lookAt.x+1.6*dir.x,lookAt.y-1.6*dir.y,lookAt.z+1.6*dir.z), btVector3(lookAt.x+75*dir.x,lookAt.y-75*dir.y,lookAt.z+75*dir.z), RayCallback);
   //player->setLinearVelocity(btVector3(dir.x*50,dir.y*50,dir.z*50));
   btTransform plPos;
   player->getMotionState()->getWorldTransform(plPos);
   if(RayCallback.hasHit()&& !playerGrappleActive && RayCallback.m_hitPointWorld.distance(plPos.getOrigin())>1.8) {
      playerJump=0;
    //End = RayCallback.m_hitPointWorld;
    //Normal = RayCallback.m_hitNormalWorld;
      //printf("hit!\n");
      btVector3 go=RayCallback.m_hitNormalWorld*-5+player->getLinearVelocity();
      if (go.length()>10)(go/go.length())*10;
      //player->setLinearVelocity(go);
      //printf("%f %f %f on hit norm",RayCallback.m_hitNormalWorld.getX(),RayCallback.m_hitNormalWorld.getY(),RayCallback.m_hitNormalWorld.getZ());
      tmp = RayCallback.m_hitPointWorld;
    // Do some clever stuff here
      if(tmp.getY()>1){
         playerGrappleActive =1;
         //SFX HERE
         //Sound s = Sound();
         //s.playSFX("../Assets/Sounds/HookShot.mp3");
         //printf("%f %f %f - player loc; %f %f %f grapple loc\n", physGetPlayerX(), physGetPlayerY(), physGetPlayerZ(), grapplingHookLocation().x, grapplingHookLocation().y, grapplingHookLocation().z);
         //printf("%f %f %f grapple sfx distance\n", abs(grapplingHookLocation().x - physGetPlayerX()), abs(grapplingHookLocation().y - physGetPlayerY()), abs(grapplingHookLocation().z - physGetPlayerZ()));
         musicPlayer.BGM.play3DSFX("../Assets/Sounds/HookShot.mp3", physGetPlayerX(), physGetPlayerY(), physGetPlayerZ(), grapplingHookLocation().x, grapplingHookLocation().y, grapplingHookLocation().z);
         
         ret = true;
      }
   }
   


   lookAt -= glm::vec3(gu.x, 0, gu.z);

   return ret;
}
void physJump(){
   if(playerJump==1){
      setPlayerSpeed(0,30,0);
      playerJump=0;
      playerFall = 0;
      anim = JUMP;
      musicPlayer.BGM.playSFX("../Assets/Sounds/Jump.wav");
   }
}
float physGetHeight(float x, float y){
   btCollisionWorld::ClosestRayResultCallback RayCallback(btVector3(x,1050,y), btVector3(x,y,1100));
   dynamicsWorld->rayTest(btVector3(x,1050,y), btVector3(x,y,1100), RayCallback);
   if(RayCallback.hasHit()){
      return RayCallback.m_hitPointWorld.getY();
   }
   return -1.0;
}

int isGrappleActive(){
   return playerGrappleActive;
}
void physGrapplePoint(){
	lookAt = GetLookAt();

   

   glm::vec3 at = glm::vec3(lookAt.x,lookAt.y,lookAt.z);
   glm::vec3 targ = glm::vec3(tmp.getX(),tmp.getY(),tmp.getZ());
   glm::vec3 loc = targ-at;
   float dist = sqrt(loc.x*loc.x+loc.y*loc.y+loc.z*loc.z);
   targ-=at;
   targ*=10;
   //
   dist/=3;
   dist = dist>1?dist:1;
   dist = targ.y>0?dist:dist/1.4;
   //
   player->setLinearVelocity(btVector3(targ.x,targ.y/dist,targ.z));
   if(dist<1.1 && !hold){
      if(flip==0){
         flip = 1;
         //printf("pls rotate camera to normal of building\n");
//         rotateCamera(pi);
      }
      playerJump = 1;
      playerGrappleActive=0;
      //setPlayerSpeed(0,10,0);
      //printf("reset\n");
   }
}
void holdGrapple(int h){
   hold = h;
}
void physSetDisplayObj(btRigidBody* phys, void *obj){
   phys->setUserPointer(obj);
}
btVector3 plsRndr(){
   return tmp;
   //return btVector3(lookAt.x+3*dir.x,lookAt.y-3*dir.y,lookAt.z+3*dir.z);
}

glm::vec3 grapplingHookLocation(){
   return glm::vec3(tmp.getX(),tmp.getY(),tmp.getZ());
   //return btVector3(lookAt.x+3*dir.x,lookAt.y-3*dir.y,lookAt.z+3*dir.z);
}
int canMove(){
   return playerJump;
}
glm::vec3 playerPoss(){
   return glm::vec3(physGetPlayerX(),physGetPlayerY(),physGetPlayerZ());
}
void physStep(float time){
   //player->getX();
   //setPlayerSpeed(2,2,2);
   AsetPlayerSpeed(curXsp,curYsp,curZsp);
   //printf("%f %f %f speeds\n",curXsp,curYsp,curZsp);
   curXsp=curYsp=curZsp=0;
   btVector3 pVec = player->getLinearVelocity();
   float fspeed = pVec.getY();
   float x,y,z;
   x = physGetPlayerX();
   y = physGetPlayerY();
   z = physGetPlayerZ();
   //printf("%f %f %f pos\n",x,y,z);
   lookAt = glm::vec3(x,y,z);
   //printf("the h is %d\n",getPressed('E'));
   if(playerGrappleActive) physGrapplePoint();
   //printf("a\n");
   dynamicsWorld->stepSimulation(1/time,10);
   //printf("fall %f %f\n",fspeed,player->getLinearVelocity().getY());
   if(fspeed<-8 && player->getLinearVelocity().getY()<-8){
      playerFall = 1;
   }
   if(player->getLinearVelocity().getY()*player->getLinearVelocity().getY()<.0001 &&
      fspeed*fspeed>.0001 && playerFall==1){
      if(!playerJump)      particleSpawner.push_front(createDustPart(250,1.0,playerPoss()-glm::vec3(0,1,0)));
      playerJump = 1;
      anim = LAND;
      //printf("can boing\n");
   }
   
   //NOT SURE IF NEEDED
   //ColCallback.resetDetector();
   //printf("b\n");

}
/*returns a vector that is the speed of the player as a vec3*/
glm::vec3 getPlayerSpeed() {
	btVector3 pVec = player->getLinearVelocity();
	return glm::vec3(pVec.getX(), pVec.getY(), pVec.getZ());
}

void ReleaseGrapple() {
   playerGrappleActive = 0;
}
