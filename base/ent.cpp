#include "ent.h"
class PRSentity{
   glm::mat4 position;
   glm::mat4 rotation
   glm::mat4 scale;

   glm::mat4 innerPos;
   glm::mat4 innerRot;
   glm::mat4 innerScale;

   glm::mat4 endPos;
   glm::mat4 endRot;
   glm::mat4 endScale;

   btRigidBody *btPhys;
   void *ourPhys
   Mesh *m;
   float time = 0;
}
entity entity::entity()
void entity::draw(vector<int> shprogs){
   for(int i=0;i<shprogs.size();i++){
      //m.draw(shprogs[i]);
   }
}
void entity::draw(){
   m.draw(innerPos*innerRot*innerScale);
   PlaceModel();
}
void entity::startInterpolatePos(glm::mat4 start, glm::mat4 end){
   
}
