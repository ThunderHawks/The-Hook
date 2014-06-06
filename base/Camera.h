#ifndef CAMERA_H_
#define CAMERA_H_

#ifndef GLM_HPP_
#define GLM_HPP_
#include "glm/glm.hpp"
#endif

extern GLint h_uViewMatrix;
extern GLint h_uLightViewMatrix;

/*Sends the view matrix to the shader and returns it*/
glm::mat4 SetView();
/*get the view matrix from a position*/
glm::mat4 GetView(glm::vec3 NewEye);
/*Set the view matrix and light's view matrix to the shader and returns it*/
glm::mat4 SetShadowView();
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
/*Sets the pitch and returns it*/
float setPitch (float newPitch);
/*Increment the pitch by the input and return its new location*/
float incrementPitch(float toAdd);
/*Sets the yaw and returns it*/
float setYaw (float newYaw);
/*Increment the yaw by the input and return its new location*/
float incrementYaw(float toAdd);
/*Gets the yaw and returns in*/
float getYaw();
/*Sets the distance and returns it*/
float setDistance(float newDist);
/*Increment the distance by the input and return its new location*/
float addDistance(float toAdd);
/*returns the distance*/
float getDistance();
/*Check to see if the camera has collided with a bounding sphere*/
bool checkCollision(glm::vec3 point, float rad);
/*Adds a speed factor to the camera*/
void SetSpeed(float speed);
/*check if you are in edit mode*/
void SetEdit (int status);
void resetVecs();
#endif
