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

#include "Helper.h"
#include "IO.h"

float startX, startY;
float endX, endY;
float eyeAtx = 0.0f;
float eyeAty = 0.0f;
float eyeAtz = 0.0f;
float alpha = 0.0;
float beta = -M_PI/2.0;

//Camera look at, eye position, up vector, and gaze, w, and u vectors
glm::vec3 lookAtPoint = glm::vec3(4.0, 0.3, 4.0);
glm::vec3 eye;
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 gaze;
glm::vec3 w, u;

//Change size to increase amount of keys input
int KeysPressed[5] = {0, 0, 0, 0, 0};

void glfwGetCursorPos(GLFWwindow *window, double xpos, double ypos) {

   if(xpos > g_width || xpos < 0 || ypos < 0 || ypos > g_height) {
      return;
   }

   //Get rid of if unneeded
   gaze = lookAtPoint - eye;
   w = glm::vec3(-1.0 * w.x, -1.0 * w.y, -1.0 * w.z);
   u = glm::cross(up, w)/magnitude(glm::cross(up, w));

   endX = xpos;
   endY = g_height-ypos-1;

   float startWX = p2w_x(startX);
   float startWY = p2w_y(startY);
   float endWX = p2w_x(endX);
   float endWY = p2w_y(endY);
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
   if(startY < endY && alpha <= 0.4) {
      diff = endY - startY;
      alpha += (diff * M_PI)/g_width;
   }
   else if(startY > endY && alpha >= -0.1) {
      diff = startY - endY;
      alpha -= (diff * M_PI)/g_width;
   }
   //Update lookAt
   eyeAtx = 1.0 * cos(alpha) * cos(beta);
   eyeAty = 1.0 * sin(alpha);
   eyeAtz = 1.0 * cos(alpha) * cos(M_PI/2.0 - beta);

   eyeAtx += lookAtPoint.x;
   eyeAty += lookAtPoint.y;
   eyeAtz += lookAtPoint.z;

   eye = glm::vec3(eyeAtx, eyeAty, eyeAtz);


   startX = g_width/2.0;// = endX;
   startY = g_height/2.0-1;// endY;
}

//the function that is called in the main loop that will act on the keys pressed
//that are kept track of inside of the array
void glfwKeyboard(void) {
  //gaze = lookAtPoint - eye;

  w = gaze/magnitude(gaze);
  w = glm::vec3(-1.0 * w.x, -1.0 * w.y, -1.0 * w.z);
  u = glm::cross(up, w)/magnitude(glm::cross(up, w));

   //GLFW_KEY_S
   if(KeysPressed[0]) {
       eye = glm::vec3(eye.x + 0.1 * w.x, eye.y, eye.z + 0.1 * w.z);
       lookAtPoint = glm::vec3(lookAtPoint.x + 0.1 * w.x, lookAtPoint.y, lookAtPoint.z + 0.1 * w.z);
   }
   //GLFW_KEY_W
   if(KeysPressed[1]) {
       eye = glm::vec3(eye.x - 0.1 * w.x, eye.y, eye.z - 0.1 * w.z);
       lookAtPoint = glm::vec3(lookAtPoint.x - 0.1 * w.x, lookAtPoint.y, lookAtPoint.z - 0.1 * w.z);
   }
   //GLFW_KEY_D
   if(KeysPressed[2]) {
       eye = glm::vec3(eye.x + 0.1 * u.x, eye.y, eye.z + 0.1 * u.z);
       lookAtPoint = glm::vec3(lookAtPoint.x + 0.1 * u.x, lookAtPoint.y, lookAtPoint.z + 0.1 * u.z);
   }
   //GLFW_KEY_A
   if(KeysPressed[3]) {
       eye = glm::vec3(eye.x - 0.1 * u.x, eye.y, eye.z - 0.1 * u.z);
       lookAtPoint = glm::vec3(lookAtPoint.x - 0.1 * u.x, lookAtPoint.y, lookAtPoint.z - 0.1 * u.z);
   }
   //GLFW_KEY_Q
   if(KeysPressed[4]) {
      printf("Space is not implemented!\n");
   }
   //GLFW_KEY_SPACE
   if(KeysPressed[5]) {
      exit( EXIT_SUCCESS );
   }
}

//the keyboard callback that will alter the array based on key pressed/released
void glfwKeyPress(GLFWwindow *window, int key, int scan, int action, int mods) {
   if(action == GLFW_PRESS) {
      switch( key ) {
       case GLFW_KEY_S:
         KeysPressed[0] = 1;
         break;
       case GLFW_KEY_W:
         KeysPressed[1] = 1;
         break;
       case GLFW_KEY_D:
         KeysPressed[2] = 1;
         break;
       case GLFW_KEY_A:
         KeysPressed[3] = 1;
         break;
       case GLFW_KEY_SPACE:
         KeysPressed[4] = 1;
         break;
       case GLFW_KEY_Q:
         KeysPressed[5] = 1;
         break;
     }
   }   
   else if(action == GLFW_RELEASE) {
      switch( key ) {
       case GLFW_KEY_S:
         KeysPressed[0] = 0;
         break;
       case GLFW_KEY_W:
         KeysPressed[1] = 0;
         break;
       case GLFW_KEY_D:
         KeysPressed[2] = 0;
         break;
       case GLFW_KEY_A:
         KeysPressed[3] = 0;
         break;
       case GLFW_KEY_SPACE:
         KeysPressed[4] = 0;
         break;
       case GLFW_KEY_Q:
         KeysPressed[5] = 0;
         break;
     }
   }
}