#ifndef LEVEL_H_
#define LEVEL_H_

#define SCALE 3
#include "Mesh.h"
#include "Camera.h"
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

#define SCALE_MIN 0.0
#define SCALE_MAX 2.0

//Structure used to represent models in the world
struct Entity {
   Mesh *mesh;
   float angle;
   glm::vec3 position;
   glm::vec3 scale;
   //Bounding Sphere Radius
   float BSRadius;
   //Mesh index
   int meshIndex;
   //"Special Custom Physics Pointer"
   void *physics;
};

//This method loads the level models and initializes the hotbar
void initLevelLoader();
//The entities are loaded into the physics engine
void loadLevel(string fileName);
//Creates an entity with the given arguments
Entity createEntity(glm::vec3 position, glm::vec3 scale, float angle, int meshIndex);
//Use entity at hotbar index as the currently selected entity
void selectAtHotBarIndex(int index);
//Determines if an entity is currently selected, if false DO NOT draw currentEntity
bool isEntitySelected();
//Changes the scale of the selected entity. The object's scale will always be
//0 < scale <= 2.0
void scaleSelectedEntity(glm::vec3 toScale);
//Change the rotation of the selected entity by multiples of 90 degrees
void rotateSelectedEntity(float angle);
//Places the selected entity into the world at lookAtPoint
void placeSelectedEntity();
//Make selected entity the one last placed
void reselectLastEntity();
//Return the entity that is currently selected
Entity getSelectedEntity();
//Return the entity inside of the world at index
Entity getEntityAt(int index);
//Return the number of entities in the world
int getEntityNum();
//Add this entity to the world
void placeEntity(Entity entity);
//Undo last placement
void undo();
//Saves the world to the currentLevel
void saveWorld();
//Save the current world in .wub format
void saveWorld(string lvName);

#endif
