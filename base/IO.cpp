#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define GLM_FORCE_RADIANS
#include "GLSL_helper.h"
#include "MStackHelp.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include <list>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <time.h>

//#include "*.h"

#include "Helper.h"
#include "IO.h"
#include "physSystem.h"
#include "Camera.h"

float startX, startY;
float endX, endY;
float eyeAtx = 0.0f;
float eyeAty = 0.0f;
float eyeAtz = 0.0f;
float alpha = 0.0;
float beta = -M_PI/2.0;

//glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 gaze;
glm::vec3 w, u;

//Change size to increase amount of keys input
int KeysPressed[255];

void glfwGetCursorPos(GLFWwindow *window, double xpos, double ypos) {

   if(xpos > g_width || xpos < 0 || ypos < 0 || ypos > g_height) {
      return;
   }

   //Get rid of if unneeded
   gaze = GetLookAt() - GetEye();
   w = glm::vec3(-1.0 * w.x, -1.0 * w.y, -1.0 * w.z);
   w = glm::normalize(w);
   u = glm::cross(GetUp(), w)/magnitude(glm::cross(GetUp(), w));
   u = glm::normalize(u);

   endX = xpos;
   endY = g_height-ypos-1;

   float diff;

   //Calculate change in X
   if(startX < endX) {
      diff = endX - startX;
      beta += (diff * M_PI)/g_width;
   }
   else if(startX > endX){
      diff = startX - endX;
      beta -= (diff * M_PI)/g_width;
   }

   //Calculate change in Y
   if(startY < endY && alpha <= 0.8) {
      diff = endY - startY;
      alpha += (diff * M_PI)/g_width;
   }
   else if(startY > endY && alpha >= -0.1) {
      diff = startY - endY;
      alpha -= (diff * M_PI)/g_width;
   }
   //Update lookAt
   eyeAtx = 6.0 * cos(alpha) * cos(beta);
   eyeAty = 6.0 * sin(alpha);
   eyeAtz = 6.0 * cos(alpha) * cos(M_PI/2.0 - beta);

   eyeAtx += GetLookAt().x;
   eyeAty += GetLookAt().y;
   eyeAtz += GetLookAt().z;

   SetEye(glm::vec3(eyeAtx, eyeAty, eyeAtz));


   startX = g_width/2.0;// = endX;
   startY = g_height/2.0-1;// endY;
}
/*
*  returns bool of character pressed
*  takes the character for a given key
*/
int getPressed(char ch){
   return KeysPressed[ch];
}
/*
* the function that is called in the main loop that will act on the keys pressed
* that are kept track of inside of the array
*/
void glfwKeyboard(void) {
  w = gaze/magnitude(gaze);
  w = glm::vec3(-1.0 * w.x, -1.0 * w.y, -1.0 * w.z);
  u = glm::cross(GetUp(), w)/magnitude(glm::cross(GetUp(), w));
  //eye = glm::vec3(physGetPlayerX(),physGetPlayerY(),physGetPlayerZ());
   //GLFW_KEY_S
   if(KeysPressed['S']) {
       setPlayerSpeed(w.x*3,0,w.z*3);
       //newSpeed.x=newSpeed.x+w.x*3;
       //newSpeed.z=newSpeed.z+w.z*3;
       MoveEye(glm::vec3(0.1 * w.x, 0, 0.1 * w.z));
   }
   //GLFW_KEY_W
   if(KeysPressed['W']) {
   	 printf("%lf %lf %lf\n", GetEye().x, GetEye().y, GetEye().z);
       setPlayerSpeed(-w.x*3,0,-w.z*3);
       //newSpeed.x=newSpeed.x-w.x*3;
       //newSpeed.z=newSpeed.z-w.z*3;
       MoveEye(glm::vec3(-0.1 * w.x, 0, -0.1 * w.z));
       printf("%lf %lf %lf\n", GetEye().x, GetEye().y, GetEye().z);
   }
   //GLFW_KEY_D
   if(KeysPressed['D']) {
       setPlayerSpeed(u.x*3,0,u.z*3);
       //newSpeed.x=newSpeed.x+u.x*3;
       //newSpeed.z=newSpeed.z+u.z*3;
       MoveEye(glm::vec3(0.1 * u.x, 0, 0.1 * u.z));
   }
   //GLFW_KEY_A
   if(KeysPressed['A']) {
        setPlayerSpeed(-u.x*3,0,-u.z*3);
       //newSpeed.x=newSpeed.x-u.x*3;
       //newSpeed.z=newSpeed.z-u.z*3;
       MoveEye(glm::vec3(-0.1 * u.x, 0, -0.1 * u.z));
   }
   ///setPlayerSpeed(newSpeed.x,newSpeed.y,newSpeed.z);
   //GLFW_KEY_SPACE
   if(KeysPressed[' ']&&physGetPlayerY()<1.3) {
      setPlayerSpeed(0,4,0);
      KeysPressed[' ']=0;
      printf("Space is not implemented!\n");
   }
   //GLFW_KEY_Q
   if(KeysPressed['Q']) {
      exit( EXIT_SUCCESS );
   }
   if(KeysPressed['F']){
     // printf("box %f %f %f\n",-gaze.x*5,gaze.y*5,-gaze.z*5);
   //   createStaticBox(eye.x,eye.y,eye.z,1,1,1,btQuaternion(0,0,0,1),1,gaze.x*10,gaze.y*10,gaze.z*10);
      KeysPressed['F']=0;
   }
   if(KeysPressed['E']){
   
//      if(KeysPressed['E']==1){
      physGrapple(w.x,w.y,w.z);
         //KeysPressed['E']=2;
  //    }
      //else physGrapplePoint();
   }
}

//the keyboard callback that will alter the array based on key pressed/released
void glfwKeyPress(GLFWwindow *window, int key, int scan, int action, int mods) {
   if(action == GLFW_PRESS) {
      switch( key ) {
       case GLFW_KEY_S:
         KeysPressed['S'] = 1;
         break;
       case GLFW_KEY_W:
         KeysPressed['W'] = 1;
         break;
       case GLFW_KEY_D:
         KeysPressed['D'] = 1;
         break;
       case GLFW_KEY_A:
         KeysPressed['A'] = 1;
         break;
       case GLFW_KEY_SPACE:
         KeysPressed[' '] = 1;
         break;
       case GLFW_KEY_Q:
         KeysPressed['Q'] = 1;
         break;
       case GLFW_KEY_F:
         KeysPressed['F'] = 1;
         break;
      case GLFW_KEY_E:
         if(!KeysPressed['E'])
            KeysPressed['E'] = 1;
         break;
     }
   }   
   else if(action == GLFW_RELEASE) {
      switch( key ) {
       case GLFW_KEY_S:
         KeysPressed['S'] = 0;
         break;
       case GLFW_KEY_W:
         KeysPressed['W'] = 0;
         break;
       case GLFW_KEY_D:
         KeysPressed['D'] = 0;
         break;
       case GLFW_KEY_A:
         KeysPressed['A'] = 0;
         break;
       case GLFW_KEY_SPACE:
         KeysPressed[' '] = 0;
         break;
       case GLFW_KEY_Q:
         KeysPressed['Q'] = 0;
         break;
       case GLFW_KEY_F:
         KeysPressed['F'] = 0;
         break;
       case GLFW_KEY_E:
         KeysPressed['E'] = 0;
         break;
     }
   }
}
