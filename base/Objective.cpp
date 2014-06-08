#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define GLM_FORCE_RADIANS
#include "GLSL_helper.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include <list>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <time.h>
#include "Objective.h"
#include "physSystem.h"
#include "SoundPlayer.h"
#include "particle.h"
#include "Lab4.h"
#include "level.h"

int counter = 0;
int inProgress = 0;
int score = 0;
int seed = rand();
int getPoints(){
   return score;
}
//Level borders
//-42x, 67z
//230x, -379z


Objective::Objective(float minX, float minZ, float maxX, float maxZ) {
   active = false;
   time = 0;
   this->minX = minX;
   this->minZ = minZ;
   this->maxX = maxX;
   this->maxZ = maxZ;
}

Objective::~Objective() {

}

void Objective::Init() {
   float sX, sY, sZ, eX, eY, eZ;
   vector<glm::vec3> points = getObjectiveDestinationPositions();
   srand(seed);
   seed = rand();

/*   do {
      sX = rand() % (int)(maxX - minX) + minX;
      sZ = rand() % (int)(maxZ - minZ) + minZ;
   } while ((sY = physGetHeight(sX, sZ)) == -1);

   do {
      eX = rand() % (int)(maxX - minX) + minX;
      eZ = rand() % (int)(maxZ - minZ) + minZ;
   } while ((eY = physGetHeight(eX, eZ)) == -1);*/
   //sX = points[select].x;
   //sY = points[select].z;
   //eX = points[selectb].x;
   //eY = points[selectb].z;
   
   //start = glm::vec3(sX, sY, sZ);
   //end = glm::vec3(eX, eY, eZ);
   counter = (counter+1)%points.size();
   start = points[counter];
   counter = (counter+1)%points.size();
   end =  points[counter];

   //printf("objective starts as %f %f %f",sX,sY,sZ);
   //printf("%f %f %f \n",physGetPlayerX(),physGetPlayerY(),physGetPlayerZ());
}

void Objective::Update(glm::vec3 playerPos) {
   if ((sqrt((playerPos.x-end.x)*(playerPos.x-end.x) + (playerPos.z-end.z)*(playerPos.z-end.z)) < 7)&&(active)) {
      active = false;
      score += 20000-time;
      int deltaScore = 20000-time;
      time = 0;
      this->Init();
      PlayFX(SUCCESS_FX);
      //printf("score is %d\n",deltaScore);
      particleSpawner.push_front(createMoneyPart(deltaScore/50,1.0,playerPoss()));
      setCool(150);
      inProgress = 0;
//      particleSpawner.push_front(createDustPart(deltaScore/100,1.0,playerPoss()));
   }
   else if (sqrt((playerPos.x-start.x)*(playerPos.x-start.x) + (playerPos.z-start.z)*(playerPos.z-start.z)) < 7 && !active &&(inProgress==0)) {
      active = true;
      time = 0;
      inProgress = 1;
   }
   else {
      time++;
   }
   //printf("dist start %f\n",sqrt((playerPos.x-start.x)*(playerPos.x-start.x) + (playerPos.y-start.y)*(playerPos.y-start.y) + (playerPos.z-start.z)*(playerPos.z-start.z)));
   //printf("dist end   %f\n",sqrt((playerPos.x-end.x)*(playerPos.x-end.x) + (playerPos.y-end.y)*(playerPos.y-end.y) + (playerPos.z-end.z)*(playerPos.z-end.z)));
   //printf("is active %d\n",active);
}








