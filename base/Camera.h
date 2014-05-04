#include "glm/glm.hpp"

extern GLint h_uViewMatrix;

/*Sends the view matrix to the shader and returns it*/
glm::mat4 SetView();
/*Returns the a vec3 of the Eye*/
glm::vec3 GetEye();
/*Sets the eye to a new vec3 and returns it's new location*/
glm::vec3 SetEye(glm::vec3 newEye);
/*Increment the eye by this vec3, and returns its new location*/
glm::vec3 MoveEye(glm::vec3 toAdd);
/*returns a vec3 of the LookAt point*/
glm::vec3 GetLookAt();
/*Sets the LookAtPoint to a new location, and returns it*/
glm::vec3 SetLookAt(glm::vec3 newLookAt);
/*Increment the lookAt by this vec3 and return its new location*/
glm::vec3 MoveLookAt(glm::vec3 toAdd);
/*returns the up vector*/
glm::vec3 GetUp();
/*Sets the up vector to a new direction, and returns it*/
glm::vec3 SetUp(glm::vec3 newUp);
