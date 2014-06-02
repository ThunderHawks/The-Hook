#ifndef CAMBOX
#define CAMBOX
#include "glm/glm.hpp"
#include "level.h"
struct camBox{
  float x,y,z;//center
  float h,w,d;//"radius"
  glm::vec3 ax;//rotation axis
  float amt;//rotation amount
};
//vector<camBox*> boxes;
void* createCameraBox(float x,float y,float z,float h,float w,float d);
int pointBox(glm::vec3 vc, camBox* bx);
glm::vec3 rotatePoint(glm::vec3 pt, float amt, glm::vec3 ax);
int BoxIntoBox(camBox* a, camBox* b);
int BoxAndBox(camBox* a,camBox* b);

float distLineToPoint(glm::mat4 line,glm::vec3 pt);
float distLineToLine(glm::mat4 a,glm::mat4 b);
int LineToBox(glm::mat4 line,camBox* bx);
#endif
