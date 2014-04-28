#include "ent.h"
class PRSentity{
   glm::mat4 position;
   glm::mat4 rotation
   glm::mat4 scale;
   btRigidBody btPhys;
   void *ourPhys
   Mesh m;
   float time = 0;
}
entity::draw(vector<int> shprogs){
   for(int i=0;i<shprogs.size();i++){
      m.draw(shprogs[i]);
   }
}
entity::draw(){
   m.draw(1);
}
entity::startInterpolatePos(glm::mat4 start, glm::mat4 end){
   
}
