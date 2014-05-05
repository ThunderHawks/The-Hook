#include "GLSL_helper.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

#include "Camera.h"

glm::vec3 eye = glm::vec3(0, 0, 0), lookAtPoint;
glm::vec3 up = glm::vec3(0, 1, 0);
glm::vec3 wVec, uVec, vVec;
float pitch = 0, yaw = -M_PI/2.0, distance = 10;



void resetVecs() {
	//Update lookAt
   eye.x = cos(pitch) * cos(yaw);
   eye.y = sin(pitch);
   eye.z = cos(pitch) * cos(M_PI/2.0 - yaw);

	eye *= glm::vec3(distance, distance, distance);
   
   while (eye.y + lookAtPoint.y < .1) {
   	eye.x -= .2*cos(pitch) * cos(yaw);
   	eye.y -= .2*sin(pitch);
   	eye.z -= .2*cos(pitch) * cos(M_PI/2.0 - yaw);
   }
   
   eye.x += lookAtPoint.x;
   eye.y += lookAtPoint.y;
   eye.z += lookAtPoint.z;
   
   
}

/*Sends the view matrix to the shader*/
glm::mat4 SetView() {
	resetVecs();
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
/*Increment the lookAt by this vec3 and return its new location*/
glm::vec3 MoveLookAt(glm::vec3 toAdd) {
	lookAtPoint += toAdd;
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

float setPitch (float newPitch) {
	pitch = newPitch;
	return pitch;
}

float incrementPitch(float toAdd) {
	pitch += toAdd;
	return pitch;
}
float setYaw (float newYaw) {
	yaw = newYaw;
	return yaw;
}
float incrementYaw(float toAdd) {
	yaw += toAdd;
	return yaw;
}
float setDistance(float newDist) {
	distance = newDist;
	return distance;
}
float addDistance(float toAdd) {
	distance += toAdd;
	return distance;
}
