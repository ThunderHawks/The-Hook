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
#include "IOStartScreen.h"
#include "SoundPlayer.h"
#include "Gui.h"

//Last cursor position
glm::vec2 lastCurPosSS;

//Mouse press callback for Start Screen Mode
void glfwStartScreenMouse(GLFWwindow *window, int button, int action, int mods) {
   //If the left button is pressed
   if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
      //printf("Gui pressing\n");
      GuiPressing(STARTSCREEN_MODE, lastCurPosSS.x, lastCurPosSS.y);
      //printf("after gui pressed\n");
   }
}

//Mouse callback for keeping track of mouse position in Start Screen Mode
void glfwStartScreenGetCursorPos(GLFWwindow *window, double xpos, double ypos) {
   lastCurPosSS = glm::vec2(xpos, g_height - ypos - 1);
   //printf("%f %f\n", xpos, ypos);
}
