#include "physSystem.h"
#include "Mesh.h"
void loadLevel(Mesh *obj){

   for(int strX=-36;strX<36;strX+=12){
      for(int strZ=-36;strZ<36;strZ+=12){
         physSetDisplayObj(createStaticBox(strX,5,strZ,1,1,1,btQuaternion(0,0,0,1),0/*INFINITE*/,0,0,0),obj); 
      }
   }
}
