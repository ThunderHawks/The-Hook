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

//Name of the level loaded
string currentLevel = "level1.wub";

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
   Entity entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.3, 0.3, 0.3), 0.0, 0);
   hotBar[0] = entity;
   //Shop Bldg
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.25, 0.25, 0.25), 0.0, 1);
   hotBar[1] = entity;
   //Cinderblock
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 2);
   hotBar[2] = entity;
   //Flower pot
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.1, 0.1, 0.1), 0.0, 3);
   hotBar[3] = entity;
   //Medium Blg
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.3, 0.3, 0.3), 0.0, 4);
   hotBar[4] = entity;
   //SideWalk
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 5);
   hotBar[5] = entity;
   //Street light
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.5, 0.5, 0.5), 0.0, 6);
   hotBar[6] = entity;
   //Table
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.3, 0.3, 0.3), 0.0, 7);
   hotBar[7] = entity;
   //Tall blg
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.4, 0.4, 0.4), 0.0, 8);
   hotBar[8] = entity;
   //Water tower
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, 0.2, 0.2), 0.0, 9);
   hotBar[9] = entity;

   //Load into current index to prevent segfault. Doesn't appear.
   selectAtHotBarIndex(0);
   entitySelected = false;
}

//Entity createEntity(glm::vec3 rotate, glm::vec3 scale,

//The entities are loaded into the physics engine
void loadLevel(string fileName){
   printf("Loading Level...\n");
   Entity tempEntity;
   int numOfEntities;
   ifstream infile;

   //If clean level, load nothing
   if(strcmp(&fileName[0], "clean") == 0) {
      currentLevel.clear();
      return;
   }
   //If default level, load default
   else if(strcmp(&fileName[0], "default") == 0) {
      infile.open(&currentLevel[0]);
   }
   //Else just load level
   else {
      infile.open(&fileName[0]);
      //Keep track of file name
      strcpy(&currentLevel[0], &fileName[0]);
   }

   int i;

   cout << "Opened file " << fileName << endl;
   infile >> numOfEntities;
   printf("NumOfEntites: %d\n", numOfEntities);
   while(0 < numOfEntities--) {
      //Read angle
      infile >> tempEntity.angle;
      //Read position
      infile >> tempEntity.position.x;
      infile >> tempEntity.position.y;
      infile >> tempEntity.position.z; 
      //Read scale
      infile >> tempEntity.scale.x;
      infile >> tempEntity.scale.y;
      infile >> tempEntity.scale.z;
      //Read BSRadius
      infile >> tempEntity.BSRadius;
      //Read meshIndex
      infile >> tempEntity.meshIndex;
      //SetMeshPointer
      tempEntity.mesh = &mesh[tempEntity.meshIndex];

      //Store entity into "entities" vector
      entities.push_back(tempEntity);
   }
}

//Creates an entity with the given arguments
Entity createEntity(glm::vec3 position, glm::vec3 scale, float angle, int meshIndex) {
   Entity entity;
   entity.position = position;
   entity.scale = scale;
   entity.angle = angle;
   entity.mesh = &mesh[meshIndex];
   entity.meshIndex = meshIndex;
   return entity;
}

//Use entity at hotbar index as the currently selected entity
void selectAtHotBarIndex(int index) {
   currentEntity = hotBar[index];
   entitySelected = true;

   //Set distance away from camera for convinience
   switch(index) {
      case 0:
         setDistance(20.0);
         break;
      case 1:
         setDistance(15.0);
         break;
      case 2:
         setDistance(3.0);
         break;
      case 3:
         setDistance(3.0);
         break;
      case 4:
         setDistance(18.0);
         break;
      case 5:
         setDistance(10.0);
         break;
      case 6:
         setDistance(10.0);
         break;
      case 7:
         setDistance(6.0);
         break;
      case 8:
         setDistance(400.0);
         break;
      case 9:
         setDistance(7.0);
         break;
    }
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
   if(entitySelected == true) {
      currentEntity.position = GetLookAt();
      entities.push_back(currentEntity);
      entitySelected = false;
   }
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

//Saves the world to the currentLevel
void saveWorld() {
   ofstream file;
   Entity entityTemp;
   //Is this is a previously unsaved level, we can't save without
   //a file name
   if(currentLevel.empty() == true) {
      return;
   }
   //Else save to file name
   else {
      file.open(&currentLevel[0], std::ifstream::out | std::ifstream::trunc);

      //Write number of entities
      file << entities.size() << "\n";
      printf("entities: %d\n", (int)entities.size());
      //For each entitys
      for(int i = 0; i < entities.size(); i++) {
         entityTemp = entities.at(i);
         printf("i: %d\n", i);
         //Write angle
         file << entityTemp.angle << " ";
         //Write position
         file << entityTemp.position.x << " " << entityTemp.position.y << " " << entityTemp.position.z << " ";
         //Write scale
         file << entityTemp.scale.x << " " << entityTemp.scale.y << " " << entityTemp.scale.z << " ";
         //Write BSRadius
         file << entityTemp.BSRadius << " ";
         //Write meshIndex
         file << entityTemp.meshIndex << "\n";
      }
      file.close();
      printf("%s saved\n", &currentLevel[0]);
   }
}

//Save the current world in .wub format
void saveWorld(string lvName) {
   ofstream file;
   Entity entityTemp;
   string fileName = lvName + ".wub";
   file.open(&fileName[0]);

   //Write number of entities
   file << entities.size() << "\n";

   //For each entitys
   for(int i = 0; i < entities.size(); i++) {
      entityTemp = entities.at(i);
   
      //Write angle
      file << entityTemp.angle << " ";
      //Write position
      file << entityTemp.position.x << " " << entityTemp.position.y << " " << entityTemp.position.z << " ";
      //Write scale
      file << entityTemp.scale.x << " " << entityTemp.scale.y << " " << entityTemp.scale.z << " ";
      //Write BSRadius
      file << entityTemp.BSRadius << " ";
      //Write meshIndex
      file << entityTemp.meshIndex << "\n";
   }
   file.close();
   printf("%s saved\n", &fileName[0]);
}

