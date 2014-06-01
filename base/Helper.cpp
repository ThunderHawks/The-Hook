
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
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <time.h>

#include "Helper.h"

/* Input:  a vec3 vector
*  output: the magnitude of the vec3
*	Summary: finds the magnitude of a vec3 
*/ 
float magnitude(glm::vec3 vector) {
   return sqrt(vector.x*vector.x + vector.y*vector.y + vector.z*vector.z);
}

// GLFW callback for any detected errors.
void glfwError(int error, const char *description)
{
   printf("gllfw error occured!\n");
   std::cerr << description << std::endl;
}

// GLFW callback for when the window is resized.
void glfwWindowResize(GLFWwindow *window, int width, int height)
{
   g_width = (float)width;
   g_height = (float)height;
   glViewport(0, 0, (GLsizei)(width), (GLsizei)(height));
}

/*The next 4 functions do the followng with a pixel

	convert the x if the x of the pixel to 
	convert the y if the y of the pixel to 
	convert the x if the x of the pixel to world
	convert the y if the y of the pixel to world
*/
float p2i_x(int p_x) {
  float x_i = ( (float)p_x - ((g_width-1.0)/2.0) )*2.0/g_width;
  return(x_i);
}

float p2i_y(int p_y) {
  return( ( (float)p_y - ((g_height-1.0)/2.0) )*2.0/g_height);
}

float p2w_x(int p_x) {
  float x_i = ( (float)p_x - ((g_width-1.0)/2.0) )*2.0/g_width;
  return(((float)g_width/(float)g_height)*x_i);
}

float p2w_y(int p_y) {
  return( ( (float)p_y - ((g_height-1.0)/2.0) )*2.0/g_height);
}

/* Input:  two floats that determine the bounds of the random number
	Output: a random float in between the two numbers
	Summary: get a random float in between two numbers
*/
float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}
