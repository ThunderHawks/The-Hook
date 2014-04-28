#include "GLSL_helper.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

#include "Camera.h"

glm::vec3 cameye, clookAtPoint;

/*Sends the view matrix to the shader*/
void SetView() {
	glm::vec3 up = glm::vec3(0, 1, 0);
//   clookAtPoint.y+=4;
   glm::mat4 view = glm::lookAt(cameye, clookAtPoint, up);
  // clookAtPoint.y-=4;
   //glm::mat4 view = glm::lookAt(clookAtPoint, eye, up);
   safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(view));
}

/*Returns the a vec3 of the Eye*/
glm::vec3 GetEye() {
	return cameye;
}
/*Sets the eye to a new vec3 and returns it's new location*/
glm::vec3 SetEye(glm::vec3 newEye) {
	cameye = newEye;
	return cameye;
}
/*returns a vec3 of the LookAt point*/
glm::vec3 GetLookAt() {
	return clookAtPoint;
}
/*Sets the clookAtPoint to a new location, and returns it*/
glm::vec3 SetLookAt(glm::vec3 newLookAt) {
	clookAtPoint = newLookAt;
	return clookAtPoint;
}
