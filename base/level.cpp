#include "physSystem.h"
#include "Mesh.h"
Mesh m;
void loadLevel(){
   m = LoadMesh("../Assets/ModMBasicBldg.obj");
   for(int strX=-100;strX<100;strX+=45){
      for(int strZ=-100;strZ<100;strZ+=45){
         physSetDisplayObj(createStaticBox(strX,0,strZ,1,1,1,btQuaternion(0,0,0,1),0/*INFINITE*/,0,0,0),&m); 
      }
   }
}
