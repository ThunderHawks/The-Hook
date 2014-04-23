#include "physSystem.h"
#include "Mesh.h"
void loadLevel(Mesh *obj){

   for(int strX=-5;strX<6;strX+=3){
      for(int strZ=-5;strZ<6;strZ+=3){
         physSetDisplayObj(createStaticBox(strX,5,strZ,4,10,4,btQuaternion(0,0,0,1),0/*INFINITE*/,0,0,0),obj); 
      }
   }
}
