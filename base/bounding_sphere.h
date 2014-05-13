#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

//Stores information regarding a ball
struct BS {
	glm::vec3 v; //Velocity
	glm::vec3 pos; //Position
	float radius; //Radius
	glm::vec3 color;
};