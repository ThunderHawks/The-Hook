#include "physSystem.h"
#include "Mesh.h"
#include "level.h"
Mesh m;
void loadLevel(){
   m = LoadMesh("../Assets/ModMBasicBldg.obj");
   for(int strX=-100;strX<100;strX+=30){
      for(int strZ=-100;strZ<100;strZ+=30){
       //btRigidBody* createStaticBox(float posX,float posY,float posZ,float scaleX,float scaleY,float scaleZ,btQuaternion rotation,float mass,float ix,float iy,float iz);
         physSetDisplayObj(createStaticBox(strX,SCALE*3,strZ,SCALE*2,SCALE*3,SCALE*2,btQuaternion(0,0,0,1),0/*INFINITE*/,0,0,0),&m); 
      }
   }
}
