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
//If entities are selected
bool entitiesSelected;
//Selected entities relative to the current lookAt 
vector<Entity> currentEntities;
//Number of selected entities relative to the current lookAt
int dupNum = 0;

//Name of the level loaded, else default
string currentLevel = "level1.wub";

//This method loads the level models and initializes the hotbar
void initLevelLoader() {
   printf("Initializing Level content...\n");

   //No entity selected
   entitiesSelected = false;

   //Load Meshes
   mesh[0] = LoadMesh("../Assets/ModMBasicBldg.obj");
   mesh[1] = LoadMesh("../Assets/shopBldg.obj");
   mesh[2] = LoadMesh("../Assets/cinderblock.obj");
   mesh[3] = LoadMesh("../Assets/Models/topHatChar.obj");
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
   entitiesSelected = false;
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
      //printf("%f,%f,%f,%f,%f,%f\n",tempEntity.position.x,tempEntity.position.y,tempEntity.position.z,tempEntity.scale.x,tempEntity.scale.y,tempEntity.scale.z);
      tempEntity.btPhys = createStaticBox(tempEntity.position.x,tempEntity.position.y,tempEntity.position.z,tempEntity.scale.x*3*7,tempEntity.scale.y*3*7,tempEntity.scale.z*3*7,btQuaternion(0,0,0,1),0,0,0,0);
      infile >> tempEntity.phyScale.x;
      infile >> tempEntity.phyScale.y;
      infile >> tempEntity.phyScale.z;
      
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
   //FIRE LORD EDIT HERE
   entity.phyScale = glm::vec3(0.0f, 0.0f, 0.0f);
   return entity;
}

//Use entity at hotbar index as the currently selected entity
void selectAtHotBarIndex(int index) {
   currentEntities.clear();
   dupNum = 1;
   currentEntities.push_back(hotBar[index]);
   //currentEntity = hotBar[index];
   entitiesSelected = true;

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

//Change the number of dups for selected entity
int changeDupNumBy(int toChange) {
   printf("toChange: %d\n", toChange);
   //No lower than 1
   if(dupNum + toChange < 1) {
      printf("dupNum (%d) + toChange (%d) == %d\n", dupNum, toChange, dupNum + toChange);
      return 1;
   }
   //If no entitiesSelected don't change
   if(entitiesSelected == false) {
      return 0;
   }
   //Else change
   else {
      //If less
      if(toChange == -1) {
         //Remove back object
         currentEntities.pop_back();
      }
      //Else if more
      else if(toChange == 1) {
         //Add toChange amount
         currentEntities.push_back(currentEntities.at(0));
      }
      //Adjust and return amount
      return dupNum += toChange;
   }
}

//Determines if entities are currently selected, if false DO NOT draw currentEntities
bool areEntitiesSelected() {
   return entitiesSelected;
}

//Unselect the selected Entity
void unselectEntity() {
   entitiesSelected = false;
}

//Changes the scale of the selected entity
void scaleSelectedEntity(glm::vec3 toScale) {
   Entity temp;

   //If change is within range
   //Currently using x as measure b/c everything is uniformly scaled right now
   if(SCALE_MIN < toScale.x + currentEntities.at(0).scale.x && toScale.x + currentEntities.at(0).scale.x <= SCALE_MAX) { 
      for(int i = 0; i < currentEntities.size(); i++) {
         //Get entity at index i
         temp = currentEntities.at(i);
         //Uniformly change scale
         temp.scale += toScale;
         //Replace
         currentEntities[i] = temp;
      }
   }
}

//Change the rotation of the selected entities by multiples of 90 degrees
void rotateSelectedEntities(float angle) {
   //Adjust the current angle by a multiple of 90 degrees
   Entity temp;
   //Rotate each object
   for(int i = 0; i < currentEntities.size(); i++) {
      //Get entitiy
      temp = currentEntities.at(i);
      //Change angle
      temp.angle += 90;
      //Replace
      currentEntities[i] = temp;  
   }
   //currentEntity.at(0).angle += angle * 90;
}

//Call to update positions of selected entities relative to lookAt
void updateCurrentEntitiesPos() {
   int angle = currentEntities.at(0).angle;
   glm::vec3 newPos;
   Entity tempEntity;

   if(entitiesSelected == true) {
      for(int i = 0; i < currentEntities.size(); i++) {
      //Determine if x or y depending on the angle
         if(angle%270 == 0) {
            //Store lookAt
            newPos = GetLookAt();
            //Adjust with x offset
            newPos = glm::vec3(newPos.x, newPos.y, newPos.z + i * currentEntities.at(i).scale.z);
         }
         else if(angle%180 == 0) {
            //Store lookAt
            newPos = GetLookAt();
            //Adjust with x offset
            newPos = glm::vec3(newPos.x - i * currentEntities.at(i).scale.x, newPos.y, newPos.z);
         }
         else if(angle%90 == 0) {
            //Store lookAt
            newPos = GetLookAt();
            //Adjust with z offset
            newPos = glm::vec3(newPos.x, newPos.y, newPos.z - i * currentEntities.at(i).scale.z);
         }
         else {
            //Store lookAt
            newPos = GetLookAt();
            //Adjust with x offset
            newPos = glm::vec3(newPos.x + i * currentEntities.at(i).scale.x, newPos.y, newPos.z);
         }
         //Get entity to add to world
         tempEntity = currentEntities.at(i);
         //Give new pos
         tempEntity.position = newPos;
         //Insert updated entity
         currentEntities[i] = tempEntity;
      }
   }
}

//Places the selected entity into the world at lookAtPoint
void placeSelectedEntity() {
   //If 
   if(entitiesSelected == true) {
      updateCurrentEntitiesPos();

      for(int i = 0; i < currentEntities.size(); i++) {
         entities.push_back(currentEntities.at(i));
      }
   }
   //No longer selecting entities
   entitiesSelected = false;
}

//Make selected entity the one last placed
void reselectLastEntity() {
   entitiesSelected = true;
}

//Return the entities that are currently selected
vector<Entity> getSelectedEntities(){
   return currentEntities;
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
      file << entityTemp.meshIndex << " ";
      //Write phyScale
      file << entityTemp.phyScale.x << " ";
      file << entityTemp.phyScale.y << " ";
      file << entityTemp.phyScale.z << "\n";
   }
   file.close();
   printf("%s saved\n", &fileName[0]);
}

