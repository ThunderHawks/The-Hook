#include "GLSL_helper.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

#include "Camera.h"

/* camera controls - do not change */
void SetView(glm::vec3 eye, glm::vec3 lookAtPoint) {
	glm::vec3 up = glm::vec3(0, 1, 0);
//   lookAtPoint.y+=4;
   glm::mat4 view = glm::lookAt(eye, lookAtPoint, up);
  // lookAtPoint.y-=4;
   //glm::mat4 view = glm::lookAt(lookAtPoint, eye, up);
   safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(view));
}
