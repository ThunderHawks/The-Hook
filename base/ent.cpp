#include "ent.h"
//#include "Mesh.h"
//#include <bullet/btBulletDynamicsCommon.h>
class PRSentity{
   glm::mat4 position;
   glm::mat4 rotation;
   glm::mat4 scale;

   glm::mat4 innerPos;
   glm::mat4 innerRot;
   glm::mat4 innerScale;

   glm::mat4 endPos;
   glm::mat4 endRot;
   glm::mat4 endScale;

   //btRigidBody *btPhys;
   void *ourPhys;
   Mesh *m;
   float time;

   void draw();
   void startInterpolatePos(glm::mat4 pos,glm::mat4 rot,glm::mat4 sca, glm::mat4 ePos,glm::mat4 eRot,glm::mat4 eSca);
   void stepAnimation(float a);
};

void PRSentity::draw(){
   drawAtM4(m, innerPos*innerRot*innerScale, 1);
}
void PRSentity::startInterpolatePos(glm::mat4 pos,glm::mat4 rot,glm::mat4 sca, glm::mat4 ePos,glm::mat4 eRot,glm::mat4 eSca){
   time=0;

   position = pos;
   rotation = rot;
   scale = sca;

   endPos = ePos;
   endRot = eRot;
   endScale = eSca;
}
void PRSentity::stepAnimation(float a){
   time+=a;
   a=time;
   innerPos = position*(1-a) + endPos*a;
   innerRot = rotation*(1-a) + endRot*a;
   innerScale = scale*(1-a) + endScale*a;
}

/*class StaticEnt{
   public:
   Mesh* m;
   void* ourPhys;
   btRigidBody* btPhys;
   
   glm::mat4 physOff;
   glm::mat4 rotOff;
   glm::mat4 scale;
};*/
