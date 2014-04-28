#include "glm/glm.hpp"

extern GLint h_uViewMatrix;

/*Sends the view matrix to the shader*/
void SetView();
/*Returns the a vec3 of the Eye*/
glm::vec3 GetEye();
/*Sets the eye to a new vec3 and returns it's new location*/
glm::vec3 SetEye(glm::vec3 newEye);
/*returns a vec3 of the LookAt point*/
glm::vec3 GetLookAt();
/*Sets the LookAtPoint to a new location, and returns it*/
glm::vec3 SetLookAt(glm::vec3 newLookAt);
