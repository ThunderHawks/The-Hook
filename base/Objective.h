#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

class Objective {
public:
   Objective(float minX, float minZ, float maxX, float maxZ);
   ~Objective();
   void Init();
   void Update(glm::vec3 playerPos);

   bool active;
   glm::vec3 start;
   glm::vec3 end;
   
private:
   float minX, minZ, maxX, maxZ;
   int time;
};
