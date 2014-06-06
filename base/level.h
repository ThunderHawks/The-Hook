#ifndef LEVEL_H_
#define LEVEL_H_

#define SCALE 3
#include "Mesh.h"
#include "Camera.h"
//#include "Gui.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include <bullet/btBulletDynamicsCommon.h>
using namespace std;

#define SCALE_MIN 0.0

//Structure used to represent models in the world
struct Entity {
   Mesh *mesh;
   int material;
   float angle;
   glm::vec3 position;
   glm::vec3 scale;
   //Bounding Sphere Radius
   float BSRadius;
   //Mesh index
   int meshIndex;
   //"Special Custom Physics Pointer"
   void *physics;
   btRigidBody *btPhys;
   //Scale of physics Boxes
   glm::vec3 phyScale;
};

//This method loads the level models and initializes the hotbar
void initLevelLoader(int EditMode);
//The entities are loaded into the physics engine
void loadLevel(string fileName);
//Creates an entity with the given arguments
Entity createEntity(glm::vec3 position, glm::vec3 scale, float angle, int meshIndex);
//Use entity at hotbar index as the currently selected entity
void selectAtHotBarIndex(int index);
//Access Entity at index from entire pool of entities
Entity GetEntityPool(int index);
//Unselect the selected Entity
void unselectEntity();
//Change the number of dups for selected entity
int changeDupNumBy(int toChange);
//Determines if entities are currently selected, if false DO NOT draw currentEntities
bool areEntitiesSelected();
//Grab selected entity at index 0
Entity getSelectedEntity();
//Returns a vector of the objective destination positions
vector<glm::vec3> getObjectiveDestinationPositions();

//Changes the x scale of the selected entity
void scaleSelectedEntityX(float scaleX);
//Changes the y scale of the selected entity
void scaleSelectedEntityY(float scaleY);
//Changes the z scale of the selected entity
void scaleSelectedEntityZ(float scaleZ);
//Changes the scale of the selected entity. The object's scale will always be
//0 < scale <= 2.0
void scaleSelectedEntity(glm::vec3 toScale);
//Change the space inbetween each selected entity by the amount put in

void changeCESpacing(float space);
//Call to update positions of selected entities relative to lookAt
void updateCurrentEntitiesPos();
//Change the rotation of the selected entities by multiples of 90 degrees
void rotateSelectedEntities(float angle);
//Change the lineAngle of the selected entities by a multiple of 90 degrees
void rotateLineAngle(float angle);
//Places the selected entity into the world at lookAtPoint
void placeSelectedEntity();
//Make selected entity the one last placed
void reselectLastEntity();
//Return the entities that are currently selected
vector<Entity> getSelectedEntities();
//Return the entity inside of the world at index
Entity getEntityAt(int index);
//Return the number of entities in the world
int getEntityNum();
//Add this entity to the world
void placeEntity(Entity entity);
//Delete the closest object to the eye
void deleteClosest();
//Undo last placement
void undo();
//Put the entity last removed back into the world
void redo();
//Saves the world to the currentLevel
void saveWorld();
//Frees all data
void freeLevelData();
//askPoint translation
vector<Entity*> pointLevelTest(glm::vec3 point);
vector<Entity*> GetNearby(int range);

#endif
