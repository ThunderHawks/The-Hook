#include "physSystem.h"
#include "Mesh.h"
#include "level.h"

Mesh mb;
Mesh ms;

/*Loads all of the meshes and places all of the objects*/
void loadLevel(){
   mb = LoadMesh("../Assets/ModMBasicBldg.obj");
   ms = LoadMesh("../Assets/shopBldg.obj");
   int tf = 0;
   for(int strX=-1000;strX<1000;strX+=30){
      for(int strZ=-500;strZ<500;strZ+=30){
       //btRigidBody* createStaticBox(float posX,float posY,float posZ,float scaleX,float scaleY,float scaleZ,btQuaternion rotation,float mass,float ix,float iy,float iz);
         tf = !tf;
         if(tf){
            physSetDisplayObj(createStaticBox(strX,SCALE*3,strZ,SCALE*2,SCALE*3,SCALE*2,btQuaternion(0,0,0,1),0/*INFINITE*/,0,0,0),&mb);
         }
         else{
            physSetDisplayObj(createStaticBox(strX,SCALE*1,strZ,SCALE*2.25,SCALE*1.25,SCALE*2,btQuaternion(0,0,0,1),0/*INFINITE*/,0,0,0),&ms); 
         }
      }
   }
}
