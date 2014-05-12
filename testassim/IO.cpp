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

float startX, startY;
float endX, endY;
float eyeAtx = 0.0f;
float eyeAty = 0.0f;
float eyeAtz = 0.0f;
float alpha = 0.0;
float beta = -M_PI/2.0;

//Camera look at, eye position, up vector, and gaze, w, and u vectors
glm::vec3 lookAtPoint = glm::vec3(0, 0, -1);
glm::vec3 eye = glm::vec3(0, 0, 0);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 gaze;
glm::vec3 w, u;

//Change size to increase amount of keys input
int KeysPressed[255];

void glfwGetCursorPos(GLFWwindow *window, double xpos, double ypos) {
}

//the function that is called in the main loop that will act on the keys pressed
//that are kept track of inside of the array
void glfwKeyboard(void) {
   //GLFW_KEY_Q
   if(KeysPressed['Q']) {
      exit( EXIT_SUCCESS );
   }
}

//the keyboard callback that will alter the array based on key pressed/released
void glfwKeyPress(GLFWwindow *window, int key, int scan, int action, int mods) {
   if(action == GLFW_PRESS) {
      switch( key ) {
       case GLFW_KEY_Q:
         KeysPressed['Q'] = 1;
         break;
     }
   }   
   else if(action == GLFW_RELEASE) {
      switch( key ) {
       case GLFW_KEY_Q:
         KeysPressed['Q'] = 0;
         break;
     }
   }
}
