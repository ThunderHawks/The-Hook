#include "GLSL_helper.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

#include "Camera.h"
#include "level.h"

#define RADIUS 1

glm::vec3 eye = glm::vec3(0, 1, 0), lookAtPoint = glm::vec3(0, 0, 1);
glm::vec3 up = glm::vec3(0, 1, 0);
glm::vec3 wVec, uVec, vVec;
float pitch = 0, yaw = -M_PI/2.0, dist = 10;
float inSpeed = 0;
int inEdit = 0;
glm::vec3 ideal;
float idealYaw = 0, idealPitch = 0;

void resetVecs() {
	if (inEdit == 0) {
		yaw += (idealYaw - yaw)/2 + (idealYaw - yaw)/20;
		pitch += (idealPitch - pitch)/2 + (idealYaw - yaw)/20;
		
		GetEye();
	}
	else {
		pitch = idealPitch;
		yaw = idealYaw;
		lookAtPoint.x = cos(pitch) * cos(yaw);
	   	lookAtPoint.y = sin(pitch);
	   	lookAtPoint.z = cos(pitch) * cos(M_PI/2.0 - yaw);
		lookAtPoint *= glm::vec3(dist, dist, dist);
		lookAtPoint += eye;

                //Ground Collision logic
                if(areEntitiesSelected() == true) {
                   Entity selected = getSelectedEntity();
                   if(lookAtPoint.y - (selected.scale.y * selected.phyScale.y)/2.0 < 0.0) {
                      lookAtPoint.y = (selected.scale.y * selected.phyScale.y)/2.0;
                   }
                }
	}
}

/*Sends the view matrix to the shader*/
glm::mat4 SetView() {
	resetVecs();
   glm::mat4 view = glm::lookAt(eye, lookAtPoint, up);
   safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(view));
   return view;
}

/* Set the view matrix and light's view matrix to the shader */
glm::mat4 SetShadowView() {
   glm::mat4 view = glm::lookAt(eye, lookAtPoint, up);
   safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(view));
   safe_glUniformMatrix4fv(h_uLightViewMatrix, glm::value_ptr(view));
   return view;
}

/*Returns the a vec3 of the Eye*/
glm::vec3 GetEye() {
	float tempDist;

	if (inEdit == 0) {
		eye.x = cos(pitch) * cos(yaw);
		eye.y = sin(pitch);
		eye.z = cos(pitch) * cos(M_PI/2.0 - yaw);

		eye *= glm::vec3(dist + inSpeed, dist + inSpeed, dist + inSpeed);
		
		while (eye.y + lookAtPoint.y < .1 && dist > 1) {
				eye.x -= .2*cos(pitch) * cos(yaw);
				eye.y -= .2*sin(pitch);
				eye.z -= .2*cos(pitch) * cos(M_PI/2.0 - yaw);
		}
		
		eye.x += lookAtPoint.x;
		eye.y += lookAtPoint.y;
		eye.z += lookAtPoint.z;
	}

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
	std::cout << "You are at " << lookAtPoint.x << " " << lookAtPoint.y << " " << lookAtPoint.z << std::endl;
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
/*Sets the pitch and returns it*/
float setPitch (float newPitch) {
	idealPitch = pitch = newPitch;
	return idealPitch;
}
/*Increment the pitch by the input and return its new location*/
float incrementPitch(float toAdd) {
	idealPitch += toAdd;
	return idealPitch;
}
/*Sets the yaw and returns it*/
float setYaw (float newYaw) {
	idealYaw = yaw = newYaw;
	return idealYaw;
}
/*Increment the yaw by the input and return its new location*/
float incrementYaw(float toAdd) {
	idealYaw += toAdd;
	return idealYaw;
}
/*returns the yaw*/
float getYaw() {
	return yaw;
}
/*Sets the distance and returns it*/
float setDistance(float newDist) {
	dist = newDist;
	return dist;
}
/*Increment the distance by the input and return its new location*/
float addDistance(float toAdd) {
	dist += toAdd;
	return dist;
}
/*gets the distance*/
float getDistance() {
	return dist;
}
/*Check to see if the camera has collided with a bounding sphere*/
bool checkCollision(glm::vec3 point, float rad) {
	bool ret = false;
	if ((point.x - eye.x)*(point.x - eye.x) + (point.y - eye.y)*(point.y - eye.y) + (point.z - eye.z)*(point.z - eye.z) < rad*rad + RADIUS*RADIUS)
		ret = true;
	
	return ret;
}
/*Adds a speed factor to the camera*/
void SetSpeed(float speed) {
	inSpeed = speed;
}
void SetEdit (int status) {
	inEdit = status;
}
