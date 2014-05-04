#include "GLSL_helper.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

#include "Camera.h"

glm::vec3 eye = glm::vec3(0, 0, 0), lookAtPoint;
glm::vec3 up = glm::vec3(0, 1, 0);

/*Sends the view matrix to the shader*/
glm::mat4 SetView() {
   glm::mat4 view = glm::lookAt(eye, lookAtPoint, up);
   safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(view));
   return view;
}

/*Returns the a vec3 of the Eye*/
glm::vec3 GetEye() {
	return eye;
}
/*Sets the eye to a new vec3 and returns it's new location*/
glm::vec3 SetEye(glm::vec3 newEye) {
	eye = newEye;
	return eye;
}
/*Increment the eye by this vec3 and returns its new location*/
glm::vec3 MoveEye(glm::vec3 toAdd) {
	eye += toAdd;
	return eye;
}
/*returns a vec3 of the LookAt point*/
glm::vec3 GetLookAt() {
	return lookAtPoint;
}
/*Sets the lookAtPoint to a new location, and returns it*/
glm::vec3 SetLookAt(glm::vec3 newLookAt) {
	lookAtPoint = newLookAt;
	return lookAtPoint;
}
/*gets the up vector and returns it*/
glm::vec3 GetUp() {
	return up;
}
/*Sets the up vector to a new direction, and returns it*/
glm::vec3 SetUp(glm::vec3 newUp) {
	up = newUp;
	return up;
}
