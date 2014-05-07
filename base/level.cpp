#include "level.h"
#include "physSystem.h"
#include "Mesh.h"
#include "Helper.h"
#include <vector>
using namespace std;

//All of the meshes
Mesh mesh[100];
//All entities placed in world
vector<Entity> entities;
//Hotbar of entities
Entity hotBar[10];
//Entity at currentLookAt
bool entitySelected;
Entity currentEntity;

//This method loads the level models and initializes the hotbar
void initLevelLoader() {
   printf("Initializing Level content...\n");

   //No entity selected
   entitySelected = false;

   //Load Meshes
   mesh[0] = LoadMesh("../Assets/ModMBasicBldg.obj");
   mesh[1] = LoadMesh("../Assets/shopBldg.obj");
   mesh[2] = LoadMesh("../Assets/cinderblock.obj");
   mesh[3] = LoadMesh("../Assets/flowerpot1.obj");
   mesh[4] = LoadMesh("../Assets/mediumBasicBuilding.obj");
   mesh[5] = LoadMesh("../Assets/sidewalk.obj");
   mesh[6] = LoadMesh("../Assets/streetlight.obj");
   mesh[7] = LoadMesh("../Assets/table.obj");
   mesh[8] = LoadMesh("../Assets/tallBldg.obj");
   mesh[9] = LoadMesh("../Assets/waterTower.obj");

   //Load hotbar options
   //Basic Blg
   Entity entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 0);
   hotBar[0] = entity;
   //Shop Bldg
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 1);
   hotBar[1] = entity;
   //Cinderblock
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 2);
   hotBar[2] = entity;
   //Flower pot
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 3);
   hotBar[3] = entity;
   //Medium Blg
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 4);
   hotBar[4] = entity;
   //SideWalk
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 5);
   hotBar[5] = entity;
   //Street light
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 6);
   hotBar[6] = entity;
   //Table
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 7);
   hotBar[7] = entity;
   //Tall blg
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.1, 0.1, 0.1), 0.0, 8);
   hotBar[8] = entity;
   //Water tower
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.8, 0.8, 0.8), 0.0, 9);
   hotBar[9] = entity;
}

//Entity createEntity(glm::vec3 rotate, glm::vec3 scale,

//The entities are loaded into the physics engine
void loadLevel(){
   printf("Loading Level...\n");
   glm::vec3 tempScale, tempPosition;
   float tempAngle;
   Entity tempEntity;

   int tf = 0;
   for(int strX=-100;strX<100;strX+=30){
      for(int strZ=-500;strZ<500;strZ+=30){
       //btRigidBody* createStaticBox(float posX,float posY,float posZ,float scaleX,float scaleY,float scaleZ,btQuaternion rotation,float mass,float ix,float iy,float iz);
         tf = !tf;
         if(tf){
            entities.push_back(createEntity(glm::vec3(strX, 4.5, strZ), glm::vec3(0.3, 0.3, 0.3), 0.0, 9));
            /*SET COLLISION BOX APPROPRIATELY*/
            //physSetDisplayObj(createStaticBox(strX,SCALE*3,strZ,SCALE*2,SCALE*3,SCALE*2,btQuaternion(0,0,0,1),0/*INFINITE*/,0,0,0),&mesh[0]);
         }
         else{
            entities.push_back(createEntity(glm::vec3(strX, 3.0, strZ), glm::vec3(0.3, 0.3, 0.3), 0.0, 1));
           // physSetDisplayObj(createStaticBox(strX,SCALE*1,strZ,SCALE*2.25,SCALE*1.25,SCALE*2,btQuaternion(0,0,0,1),0/*INFINITE*/,0,0,0),&mesh[1]);
         }
      }
   }
 
#if 0
Hard coded physics boxes
   int tf = 0;
   for(int strX=-100;strX<100;strX+=30){
      for(int strZ=-500;strZ<500;strZ+=30){
       //btRigidBody* createStaticBox(float posX,float posY,float posZ,float scaleX,float scaleY,float scaleZ,btQuaternion rotation,float mass,float ix,float iy,float iz);
         tf = !tf;
         if(tf){
            physSetDisplayObj(createStaticBox(strX,SCALE*3,strZ,SCALE*2,SCALE*3,SCALE*2,btQuaternion(0,0,0,1),0/*INFINITE*/,0,0,0),&mesh[0]);
         }
         else{
            physSetDisplayObj(createStaticBox(strX,SCALE*1,strZ,SCALE*2.25,SCALE*1.25,SCALE*2,btQuaternion(0,0,0,1),0/*INFINITE*/,0,0,0),&mesh[1]);
         }
      }
   }
#endif

#if 0
   Code to add all entities to physics code, used after loading from .txt files
   //Draw each entity in the world
   for(int i = 0; i < entities.size(); i++) {

      tempEntity = entities.at(i);

      tempPosition = tempEntity.position;
      tempAngle= tempEntity.angle;
      tempScale = tempEntity.scale;

      physSetDisplayObj(createStaticBox(tempPosition.x, tempPosition.y, tempPosition.z, tempScale.x, tempScale.y, tempScale.z, btQuaternion(0,0,0,1),0/*INFINITE*/,0,0,0), tempEntity.mesh);
   }
#endif
}

//Creates an entity with the given arguments
Entity createEntity(glm::vec3 position, glm::vec3 scale, float angle, int meshIndex) {
   Entity entity;
   entity.position = position;
   entity.scale = scale;
   entity.angle = angle;
   entity.mesh = &mesh[meshIndex];
   return entity;
}

//Use entity at hotbar index as the currently selected entity
void selectAtHotBarIndex(int index) {
   currentEntity = hotBar[index];
   entitySelected = true;
}
 
//Determines if an entity is currently selected, if false DO NOT draw currentEntity
bool isEntitySelected() {
   return entitySelected;
}

//Changes the scale of the selected entity
void scaleSelectedEntity(glm::vec3 toScale) {
   //If change is within range
   //Currently using x as measure b/c everything is uniformly scaled right now
   if(SCALE_MIN < toScale.x + currentEntity.scale.x && toScale.x + currentEntity.scale.x <= SCALE_MAX) { 
      currentEntity.scale += toScale;
   }
}

//Change the rotation of the selected entity by multiples of 90 degrees
void rotateSelectedEntity(float angle) {
   //adjust the current angle by a multiple of 90 degrees, M_PI/2.0 is the radian equivalent
   currentEntity.angle += angle * 90;
}

//Places the selected entity into the world at lookAtPoint
void placeSelectedEntity() {
   currentEntity.position = GetLookAt();
   entities.push_back(currentEntity);
   entitySelected = false;
}

//Make selected entity the one last placed
void reselectLastEntity() {
   entitySelected = true;
}

//Return the entity that is currently selected
Entity getSelectedEntity(){
   return currentEntity;
}

//Return the entity inside of the world at index
Entity getEntityAt(int index) {
   return entities.at(index);
}

//Return the number of entities in the world
int getEntityNum() {
   return entities.size();
}

//Add this entity to the world
void placeEntity(Entity entity){
   entities.push_back(entity);
}

//Undo last placement
void undo() {
   //If there are entities to actually undo
   if(entities.size() > 0) {
      //Undo one
      entities.pop_back();
   }
}

/*
void writeWubFile(String fileName) {
   if(entities.size() == 0) {
      printf("No can do... you need to make something first!\n");
   }
}

void readWubFile(String fileName) {

}*/

