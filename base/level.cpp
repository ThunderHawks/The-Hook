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
Entity hotBar[20];
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
void initLevelLoader() {
   printf("Initializing Level content...\n");

   //No entity selected
   entitiesSelected = false;

   //Load Meshes
   mesh[0] = LoadMesh("../Assets/ModMBasicBldg.obj");//60h 43d 49w
   mesh[1] = LoadMesh("../Assets/shopBldg.obj");//25h 50d 50w
   mesh[2] = LoadMesh("../Assets/cinderblock.obj");//2h,4d,2w
   mesh[3] = LoadMesh("../Assets/mediumBasicBuilding.obj");//60h 43d 40w 
   mesh[4] = LoadMesh("../Assets/Models/sidewalkCorner.obj");//1h 5d 5w
   mesh[5] = LoadMesh("../Assets/sidewalk.obj");//1h 5d 5w
   mesh[6] = LoadMesh("../Assets/streetlight.obj");//25h 2d 12w
   mesh[7] = LoadMesh("../Assets/table.obj");//5h 10d 10w
   mesh[8] = LoadMesh("../Assets/tallBldg.obj");//1070h 200d 300w
   mesh[9] = LoadMesh("../Assets/waterTower.obj");//32h 26d 26w
   mesh[10] = LoadMesh("../Assets/Models/simpleCRBldg.obj");//166h 71d 71w
   mesh[11] = LoadMesh("../Assets/Models/wall.obj"); //15h 1d 10w
   mesh[12] = LoadMesh("../Assets/Models/gasStation.obj"); //34h 50d 50w
   mesh[13] = LoadMesh("../Assets/Models/halfSlab.obj"); //1h 5d 5w
   mesh[14] = LoadMesh("../Assets/Models/mart.obj"); //1h 5d 5w
   mesh[15] = LoadMesh("../Assets/Models/flag.obj"); //10h 1d 7w
   mesh[16] = LoadMesh("../Assets/Models/asymBldg.obj"); //10h 1d 7w
   mesh[17] = LoadMesh("../Assets/Models/pointyBldg.obj"); //10h 1d 7w

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
   //Medium Blg
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.1, 0.1, 0.1), 0.0, 3);
   hotBar[3] = entity;
   //Side walk corner
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 4);
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
   //Record Building
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, 0.2, 0.2), 0.0, 10);
   hotBar[10] = entity;
   //Wall
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, 0.2, 0.2), 0.0, 11);
   hotBar[11] = entity;
   //Gas Station
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.3, 0.3, 0.3), 0.0, 12);
   hotBar[12] = entity;
   //Half slab
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(1.0, 1.0, 1.0), 0.0, 13);
   hotBar[13] = entity;
   //Mart
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.3, 0.3, 0.3), 0.0, 14);
   hotBar[14] = entity;
   //Flag
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.2, 0.2, 0.2), 0.0, 15);
   hotBar[15] = entity;
   //AsymbBldg
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.1, 0.1, 0.1), 0.0, 16);
   hotBar[16] = entity;
   //PointyBldg
   entity = createEntity(glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.1, 0.1, 0.1), 0.0, 17);
   hotBar[17] = entity;

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
      printf("the radius is %f\n",tempEntity.BSRadius);
      //Read meshIndex
      infile >> tempEntity.meshIndex;
      //SetMeshPointer
      tempEntity.mesh = &mesh[tempEntity.meshIndex];
      //phyScale!
      infile >> tempEntity.phyScale.x;
      infile >> tempEntity.phyScale.y;
      infile >> tempEntity.phyScale.z;
      //printf("%f,%f,%f,%f,%f,%f\n",tempEntity.position.x,tempEntity.position.y,tempEntity.position.z,tempEntity.scale.x,tempEntity.scale.y,tempEntity.scale.z);
//    createStaticBox(float posX,float posY,float posZ,
               //     float scaleX,float scaleY,float scaleZ,
            //        btQuaternion rotation,float mass,float ix,float iy,float iz)
      if(!(tempEntity.angle>-10&&tempEntity.angle<10||tempEntity.angle>170&&tempEntity.angle<190)){
         float tem = tempEntity.phyScale.x;
         tempEntity.phyScale.x = tempEntity.phyScale.z;
         tempEntity.phyScale.z = tem;
         tem = tempEntity.scale.x;
         tempEntity.scale.x = tempEntity.scale.z;
         tempEntity.scale.z = tem;
      }
      tempEntity.btPhys = createStaticBox(tempEntity.position.x,tempEntity.position.y,tempEntity.position.z,
                                          tempEntity.scale.x*tempEntity.phyScale.x*.5,tempEntity.scale.y*tempEntity.phyScale.y*.55,tempEntity.scale.z*tempEntity.phyScale.z*.5,
                                          btQuaternion(0,0,0,1),0,0,0,0);
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
   switch(meshIndex){
      case 0:
         entity.phyScale = glm::vec3(60,43,49);
         break;
      case 1:
         entity.phyScale = glm::vec3(25,50,50);
         break;
      case 2:
         entity.phyScale = glm::vec3(2,4,2);
         break;
      case 3:
         entity.phyScale = glm::vec3(60,43,40);
         break;
      case 4:
         entity.phyScale = glm::vec3(1,5,5);
         break;
      case 5:
         entity.phyScale = glm::vec3(1,5,5);
         break;
      case 6:
         entity.phyScale = glm::vec3(25,2,12);
         break;
      case 7:
         entity.phyScale = glm::vec3(5,10,10);
         break;
      case 8:
         entity.phyScale = glm::vec3(1070,200,300);
         break;
      case 9:
         entity.phyScale = glm::vec3(32,26,26);
         break;
      case 10:
         entity.phyScale = glm::vec3(166,71,71);
         break;
      case 11:
         entity.phyScale = glm::vec3(15, 1, 10);
         break;
      case 12:
         entity.phyScale = glm::vec3(34, 50, 50);
         break;
      case 13:
         entity.phyScale = glm::vec3(1,5,5);
         break;
      case 14:
         entity.phyScale = glm::vec3(40, 60, 60);
         break;
      case 15:
         entity.phyScale = glm::vec3(10, 1, 7);
         break;
      case 16:
         entity.phyScale = glm::vec3(500, 140, 140);
         break;
      case 17:
         entity.phyScale = glm::vec3(825, 150, 150);
         break;
      //h w d
      //x y z
   }
   float temp = entity.phyScale.y;
   entity.phyScale.y = entity.phyScale.x;
   entity.phyScale.x = temp;
   entity.BSRadius = 2*sqrt(entity.phyScale.x*entity.phyScale.x+entity.phyScale.y*entity.phyScale.y+entity.phyScale.z*entity.phyScale.z);
   printf("this is radius %f\n",entity.BSRadius);
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
         setDistance(7.0);
         break;
      case 4:
         setDistance(10.0);
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
      case 10:
         setDistance(30.0);
         break;
      case 11:
         setDistance(7.0);
         break;
      case 12:
         setDistance(20.0);
         break;
      case 13:
         setDistance(10.0);
         break;
      case 14:
         setDistance(20.0);
         break;
      case 15:
         setDistance(10.0);
         break;
      case 16:
         setDistance(50.0);
         break;
      case 17:
         setDistance(50.0);
         break;
    }
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
   if(SCALE_MIN < scaleX + currentEntities.at(0).scale.x && scaleX + currentEntities.at(0).scale.x <= SCALE_MAX) { 
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

//Changes the y scale of the selected entity
void scaleSelectedEntityY(float scaleY) {
   Entity temp;

   //If change is within range
   if(SCALE_MIN < scaleY + currentEntities.at(0).scale.y && scaleY + currentEntities.at(0).scale.y <= SCALE_MAX) { 
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
   if(SCALE_MIN < scaleZ + currentEntities.at(0).scale.z && scaleZ + currentEntities.at(0).scale.z <= SCALE_MAX) { 
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

