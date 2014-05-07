#ifndef ent
#define ent
#include <vector>
#include "Mesh.h"
#include <bullet/btBulletDynamicsCommon.h>
#include "glm/glm.hpp"
struct StaticEnt{
   public:
   void* m;
   void* ourPhys;
   void* btPhys;
   
   glm::mat4 physOff;
   glm::mat4 rotOff;
   glm::mat4 scale;
};
std::vector<StaticEnt*> entityVec;
#endif
