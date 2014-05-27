//#include "level.h"
#include "Gui.h"
#include "physSystem.h"
#include "Helper.h"
#include "camBox.h"
#include "octree.h"
#include <vector>
using namespace std;

//All of the meshes
Mesh mesh[100];

//All entities placed in world
vector<Entity> entities;

//If entities are selected
bool entitiesSelected;
//Selected entities relative to the current lookAt 
vector<Entity> currentEntities;
//spacing between each currentEntities
float CESpacing = 0.0;
//Line angle. Changing this changes rotates the line of currentEntities
int lineAngle = 0;
//Number of selected entities relative to the current lookAt
int dupNum = 0;
//Amount to undo if undo is done
int undoAmount = 0;
//Entities to redo
vector<Entity> redoEntities;

//Name of the level loaded, else default
string currentLevel = "level1.wub";

//This method loads the level models and initializes the hotbar
void initLevelLoader(int EditMode) {
   printf("Initializing Level content...\n");

   //Load Meshes
   mesh[0] = LoadMesh("../Assets/Models/ModMBasicBldg.obj");//60h 43d 49w
   mesh[1] = LoadMesh("../Assets/Models/shopBldg.obj");//25h 50d 50w
   mesh[2] = LoadMesh("../Assets/Models/cinderblock.obj");//0.5h,1d,0.5w
   mesh[3] = LoadMesh("../Assets/Models/mediumBasicBuilding.obj");//60h 43d 40w 
   mesh[4] = LoadMesh("../Assets/Models/sidewalkCorner.obj");//1h 5d 5w
   mesh[5] = LoadMesh("../Assets/Models/sidewalk.obj");//1h 5d 5w
   mesh[6] = LoadMesh("../Assets/Models/streetlight.obj");//25h 2d 12w
   mesh[7] = LoadMesh("../Assets/Models/table.obj");//5h 10d 10w
   mesh[8] = LoadMesh("../Assets/Models/tallBldg.obj");//1070h 200d 300w
   mesh[9] = LoadMesh("../Assets/Models/waterTower.obj");//32h 26d 26w
   mesh[10] = LoadMesh("../Assets/Models/simpleCRBldg.obj");//166h 71d 71w
   mesh[11] = LoadMesh("../Assets/Models/wall.obj"); //15h 1d 10w
   mesh[12] = LoadMesh("../Assets/Models/gasStation.obj"); //34h 50d 50w
   mesh[13] = LoadMesh("../Assets/Models/halfSlab.obj"); //1h 5d 5w
   mesh[14] = LoadMesh("../Assets/Models/mart.obj"); //1h 5d 5w
   mesh[15] = LoadMesh("../Assets/Models/flag.obj"); //10h 1d 7w
   mesh[16] = LoadMesh("../Assets/Models/asymBldg.obj"); //10h 1d 7w
   mesh[17] = LoadMesh("../Assets/Models/pointyBldg.obj"); //10h 1d 7w

   printf("Loaded meshes..\n");

   //Load into current index to prevent segfault. Doesn't appear.
   if(EditMode) {
      selectAtHotBarIndex(1);
      printf("selectedHotBarIndex\n");
      entitiesSelected = false;
   }
}

Octree* octLevel = new Octree(glm::vec3(-42,-100,67),glm::vec3(230,2000,379),3);
void loadOctTree(){
   for(int i = 0;i<entities.size();i++){
      octLevel->add(&entities[i]);
   }
}
vector<Entity*> pointLevelTest(glm::vec3 point){
   return octLevel->askPoint(point);//dont wana mess with .h files
}

//The entities are loaded into the physics engine
void loadLevel(string fileName){
   printf("Loading Level...\n");
   Entity tempEntity;
   int numOfEntities, i;
   ifstream infile;
   //-42x, 67z
   //230x, -379z

   //If clean level, load nothing
   if(strcmp(&fileName[0], "c.wub") == 0) {
      currentLevel.clear();
      return;
   }
   //If default level, load default
   else if(strcmp(&fileName[0], "d.wub") == 0) {
      infile.open(&currentLevel[0]);
   }
   //Else just load level
   else {
      infile.open(&fileName[0]);
      //Keep track of file name
      strcpy(&currentLevel[0], &fileName[0]);
   }

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
      //printf("the radius is %f\n",tempEntity.BSRadius);
      //Read meshIndex
      infile >> tempEntity.meshIndex;
      //SetMeshPointer
      tempEntity.mesh = &mesh[tempEntity.meshIndex];
      //phyScale!
      infile >> tempEntity.phyScale.x;
      infile >> tempEntity.phyScale.y;
      infile >> tempEntity.phyScale.z;

      switch(tempEntity.meshIndex){
         case 0:
            tempEntity.phyScale = glm::vec3(43,60,49);
            break;
         case 1:
            tempEntity.phyScale = glm::vec3(50,25,50);
            break;
         case 2:
            tempEntity.phyScale = glm::vec3(1, 0.5, 0.5);
            break;
         case 3:
            tempEntity.phyScale = glm::vec3(43,60,40);
            break;
         case 4:
            tempEntity.phyScale = glm::vec3(5,1,5);
            break;
        case 5:
            tempEntity.phyScale = glm::vec3(5,1,5);
            break;
         case 6:
            tempEntity.phyScale = glm::vec3(3, 25,2);
            break;
         case 7:
            tempEntity.phyScale = glm::vec3(10,5,10);
            break;
         case 8:
            tempEntity.phyScale = glm::vec3(200,1070,300);
            break;
         case 9:
            tempEntity.phyScale = glm::vec3(26,32,26);
            break;
         case 10:
            tempEntity.phyScale = glm::vec3(71,166,71);
            break;
         case 11:
            tempEntity.phyScale = glm::vec3(10, 15, 1);
            break;
         case 12:
            tempEntity.phyScale = glm::vec3(50, 34, 50);
            break;
         case 13:
            tempEntity.phyScale = glm::vec3(5,1,5);
            break;
         case 14:
            tempEntity.phyScale = glm::vec3(60, 40, 60);
            break;
         case 15:
            tempEntity.phyScale = glm::vec3(1, 10, 7);
            break;
         case 16:
            tempEntity.phyScale = glm::vec3(140, 500, 140);
            break;  
         case 17:
            tempEntity.phyScale = glm::vec3(150, 825, 150);
            break;
        }

      if(!(tempEntity.angle>-10&&tempEntity.angle<10||tempEntity.angle>170&&tempEntity.angle<190)){
         float tem = tempEntity.phyScale.x;
         tempEntity.phyScale.x = tempEntity.phyScale.z;
         tempEntity.phyScale.z = tem;
         tem = tempEntity.scale.x;
         tempEntity.scale.x = tempEntity.scale.z;
         tempEntity.scale.z = tem;
      }
      tempEntity.btPhys = createStaticBox(tempEntity.position.x,tempEntity.position.y,tempEntity.position.z,
       tempEntity.scale.x*tempEntity.phyScale.x*.5,tempEntity.scale.y*tempEntity.phyScale.y*.5,tempEntity.scale.z*tempEntity.phyScale.z*.5,
       btQuaternion(0,0,0,1),0,0,0,0);
      tempEntity.physics = createCameraBox(tempEntity.position.x,tempEntity.position.y,tempEntity.position.z,
       tempEntity.scale.y*tempEntity.phyScale.y*.6,tempEntity.scale.x*tempEntity.phyScale.x*.6,tempEntity.scale.z*tempEntity.phyScale.z*.6);

      if(!(tempEntity.angle>-10&&tempEntity.angle<10||tempEntity.angle>170&&tempEntity.angle<190)){
         float tem = tempEntity.phyScale.x;
         tempEntity.phyScale.x = tempEntity.phyScale.z;
         tempEntity.phyScale.z = tem;
         tem = tempEntity.scale.x;
         tempEntity.scale.x = tempEntity.scale.z;
         tempEntity.scale.z = tem;
      }
      
      //Store entity into "entities" vector
      entities.push_back(tempEntity);
   }
   loadOctTree();
}


//Creates an entity with the given arguments
Entity createEntity(glm::vec3 position, glm::vec3 scale, float angle, int meshIndex) {
   Entity entity;
   entity.position = position;
   entity.scale = scale;
   entity.angle = angle;
   entity.mesh = &mesh[meshIndex];
   entity.meshIndex = meshIndex;
   entity.phyScale = glm::vec3(0.0f, 0.0f, 0.0f);
      switch(meshIndex){
         case 0:
            entity.phyScale = glm::vec3(43,60,49);
            break;
         case 1:
            entity.phyScale = glm::vec3(50,25,50);
            break;
         case 2:
            entity.phyScale = glm::vec3(1, 0.5, 0.5);
            break;
         case 3:
            entity.phyScale = glm::vec3(43,60,40);
            break;
         case 4:
            entity.phyScale = glm::vec3(5,1,5);
            break;
        case 5:
            entity.phyScale = glm::vec3(5,1,5);
            break;
         case 6:
            entity.phyScale = glm::vec3(3, 25,2);
            break;
         case 7:
            entity.phyScale = glm::vec3(10,5,10);
            break;
         case 8:
            entity.phyScale = glm::vec3(200,1070,300);
            break;
         case 9:
            entity.phyScale = glm::vec3(26,32,26);
            break;
         case 10:
            entity.phyScale = glm::vec3(71,166,71);
            break;
         case 11:
            entity.phyScale = glm::vec3(10, 15, 1);
            break;
         case 12:
            entity.phyScale = glm::vec3(50, 34, 50);
            break;
         case 13:
            entity.phyScale = glm::vec3(5,1,5);
            break;
         case 14:
            entity.phyScale = glm::vec3(60, 40, 60);
            break;
         case 15:
            entity.phyScale = glm::vec3(1, 10, 7);
            break;
         case 16:
            entity.phyScale = glm::vec3(140, 500, 140);
            break;
         case 17:
            entity.phyScale = glm::vec3(150, 825, 150);
            break;
        }

       entity.BSRadius = sqrt( pow(entity.scale.x * entity.phyScale.x, 2.0) + pow(entity.scale.y * entity.phyScale.y, 2.0) + pow(entity.scale.z * entity.phyScale.z, 2.0));

   return entity;
}

//Update the radius of each object
void UpdateBSRadius() {
   Entity entityTemp;

   for(int i = 0; i < entities.size(); i++) {
      entityTemp = entities[i];
      entityTemp.BSRadius = sqrt( pow(entityTemp.scale.x * entityTemp.phyScale.x, 2.0) + pow(entityTemp.scale.y * entityTemp.phyScale.y, 2.0) + pow(entityTemp.scale.z * entityTemp.phyScale.z, 2.0));
      entities[i] = entityTemp;
   }
}

//Access Entity at index from entire pool of entities
Entity GetEntityPool(int index) {
   return entities[index];
}

//Use entity at hotbar index as the currently selected entity
void selectAtHotBarIndex(int index) {
   Icon iconTemp = getHBIcon(index);

   currentEntities.clear();
   dupNum = 1;
   currentEntities.push_back(iconTemp.entity);
   //currentEntity = hotBar[index];
   entitiesSelected = true;

   //Set distance away from camera for convinience
   setDistance(iconTemp.lookAtDistance);
}

//Change the number of dups for selected entity that is going to be placed
int changeDupNumBy(int toChange) {
   //No lower than 1
   if(dupNum + toChange < 1) {
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

//Changes the x scale of the selected entity
void scaleSelectedEntityX(float scaleX) {
   Entity temp;

   //If change is within range
   if(SCALE_MIN < scaleX + currentEntities.at(0).scale.x && scaleX) { 
      for(int i = 0; i < currentEntities.size(); i++) {
         //Get entity at index i
         temp = currentEntities.at(i);
         //Uniformly change scale
         temp.scale += glm::vec3(scaleX, 0.0f, 0.0f);
         //Replace
         currentEntities[i] = temp;
      }
   }
}

//Grab selected entity at index 0
Entity getSelectedEntity() {
   return currentEntities.at(0);
}

//Changes the y scale of the selected entity
void scaleSelectedEntityY(float scaleY) {
   Entity temp;

   //If change is within range
   if(SCALE_MIN < scaleY + currentEntities.at(0).scale.y) { 
      for(int i = 0; i < currentEntities.size(); i++) {
         //Get entity at index i
         temp = currentEntities.at(i);
         //Uniformly change scale
         temp.scale += glm::vec3(0.0f, scaleY, 0.0f);
         //Replace
         currentEntities[i] = temp;
      }
   }
}

//Changes the z scale of the selected entity
void scaleSelectedEntityZ(float scaleZ) {
   Entity temp;

   //If change is within range
   if(SCALE_MIN < scaleZ + currentEntities.at(0).scale.z) { 
      for(int i = 0; i < currentEntities.size(); i++) {
         //Get entity at index i
         temp = currentEntities.at(i);
         //Uniformly change scale
         temp.scale += glm::vec3(0.0f, 0.0f, scaleZ);
         //Replace
         currentEntities[i] = temp;
      }
   }
}


//Changes the uniform scale of the selected entity
void scaleSelectedEntity(glm::vec3 toScale) {
   Entity temp;

   //If change is within range
   //Currently using x as measure b/c everything is uniformly scaled right now
   if(SCALE_MIN < toScale.x + currentEntities.at(0).scale.x) { 
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

//Change the space inbetween each selected entity by the amount put in
void changeCESpacing(float space) 
{
   //Check if in range
   if(0 <= space + CESpacing) { 
      CESpacing += space;
   }
}

//Change the rotation of the selected entities by multiples of 90 degrees
void rotateSelectedEntities(float angle) {
   //Adjust the current angle by a multiple of 90 degrees
   Entity temp;

   //If within range of 0 to 270
   if(currentEntities.at(0).angle + 90 * angle >= 0 && currentEntities.at(0).angle + 90 * angle <= 270) {
      //Rotate each object
      for(int i = 0; i < currentEntities.size(); i++) {
         //Get entitiy
         temp = currentEntities.at(i);
         //Change angle
         temp.angle += 90 * angle;
         //Replace
         currentEntities[i] = temp;  
      }
   }
   //currentEntity.at(0).angle += angle * 90;
}

//Change the lineAngle of the selected entities by a multiple of 90 degrees
void rotateLineAngle(float angle) {
   //If within range of 0 to 270
   if(angle * 90 + lineAngle >= 0 && angle * 90 + lineAngle <= 270) {
      lineAngle += 90 * angle;
   } 
}

//Call to update positions of selected entities relative to lookAt
void updateCurrentEntitiesPos() {
   glm::vec3 newPos;
   Entity tempEntity;

   if(entitiesSelected == true) {
      for(int i = 0; i < currentEntities.size(); i++) {
      //Determine if x or y depending on the lineAngle
         if(lineAngle/270 == 1) {
            //Store lookAt
            newPos = GetLookAt();
            //Adjust with x offset
            newPos = glm::vec3(newPos.x - ((i * CESpacing) + i * currentEntities.at(i).scale.x * currentEntities.at(i).phyScale.x), newPos.y, newPos.z);
         }
         else if(lineAngle/180 == 1) {
            //Store lookAt
            newPos = GetLookAt();
            //Adjust with x offset
            newPos = glm::vec3(newPos.x, newPos.y, newPos.z - ((i * CESpacing) + i * currentEntities.at(i).scale.z * currentEntities.at(i).phyScale.z));
         }
         else if(lineAngle/90 == 1) {
            //Store lookAt
            newPos = GetLookAt();
            //Adjust with x offset
            newPos = glm::vec3(newPos.x + ((i * CESpacing) + i * currentEntities.at(i).scale.x * currentEntities.at(i).phyScale.x), newPos.y, newPos.z);

         }
         else {
            //Store lookAt
            newPos = GetLookAt();
            //Adjust with z offset
            newPos = glm::vec3(newPos.x, newPos.y, newPos.z + ((i * CESpacing) + i * currentEntities.at(i).scale.z * currentEntities.at(i).phyScale.z));
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
   
   //Change spacing back to zero
   CESpacing = 0.0;
   undoAmount = currentEntities.size();
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

//Delete the closest object to the eye
void deleteClosest() {
   float closestDistance = FLT_MAX;
   int closestIndex;
   glm::vec3 editorEye = GetEye();

   //If nothing to delete
   if(entities.size() == 0) {
      return;
   }

   //Find the closest entity
   for(int i = 0; i < entities.size(); i++) {
      if(glm::distance(entities[i].position, editorEye) < closestDistance) {
         closestIndex = i;
         closestDistance = glm::distance(entities[i].position, editorEye);
      }
   }
   //Add to redo and then remove
   redoEntities.push_back(entities[closestIndex]);
   entities.erase(entities.begin() + closestIndex);
}

//Undo last placement
void undo() {
   //If there are entities to actually undo
   if(entities.size() > 0) {
      //Undo amount last placed
      while(undoAmount--) {
         //Put the entity into redo
         redoEntities.push_back(entities[entities.size() - 1]);
         //Then pop off
         entities.pop_back();
      }
      undoAmount = 1;
   }
}

//Put the entity last removed back into the world
void redo() {
   //If there are entities to actually redo
   if(redoEntities.size() > 0) {
      //Push the recently removed element back into entities
      entities.push_back(redoEntities.back());
      //Then remove from redo
      redoEntities.pop_back();  
   }
}

//Saves the world to the currentLevel
void saveWorld() {
   UpdateBSRadius();

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
      printf("%s saved\n", &currentLevel[0]);
   }
}

//Save the current world in .wub format
void saveWorld(string lvName) {
   UpdateBSRadius();

   ofstream file;
   Entity entityTemp;
   string fileName = lvName + ".wub";
   file.open(&fileName[0]);

   if(strcmp(&fileName[0], "none.wub") == 0) {
      printf("Save aborted\n");
      return;
   }

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

