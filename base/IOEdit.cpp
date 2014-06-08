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
#include "IOEdit.h"
#include "physSystem.h"
#include "Camera.h"
#include "level.h"
#include "Lab4.h"
#include "SoundPlayer.h"
#include "Gui.h"

//e = edit
float estartX, estartY;
float eendX, eendY;
float ealpha = 0.0;
float ebeta = -M_PI/2.0;
int erTgl = 0;
//Used to load the previous scroll distance for 'R' in edit mode
float previousLookAtDistance = 3.0;

//Used to increase speed when 'Left Shift' is pressed in edit
float speed = 1.0;

glm::vec3 egaze;
glm::vec3 ew, eu;

//Last cursor position
glm::vec2 lastCurPos;

//Change size to increase amount of keys input
int eKeysPressed[350];

/*
*  returns bool of character pressed
*  takes the character for a given key
*/
int getEPressed(char ch){
   return eKeysPressed[ch];
}

//Mouse press callback for Edit Mode
void glfwEditMouse(GLFWwindow *window, int button, int action, int mods) {
   
   //If the left button is pressed
   if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
      //If game is paused
      if(isPaused()) {
         pauseorUnpause();
      }
      //If in gui mode, call button presses
      if(eKeysPressed['G']) {
         GuiPressing(EDIT_MODE, lastCurPos.x, lastCurPos.y);
      }
      //If an entity is not selected and shift is held
      else if(areEntitiesSelected() == false && eKeysPressed[340] && eKeysPressed['G'] == 0) {
         deleteClosest();
      }
      //If an entity is selected, add it
      else if(areEntitiesSelected() == true && eKeysPressed['G'] == 0) {
         placeSelectedEntity();
         //Reset lookAtDistance
         previousLookAtDistance = getDistance();
         setDistance(3.0);
         //Set shift to false to avoid undoing object just placed
         eKeysPressed[340] = 0;
      }
   }
   else if(button == GLFW_MOUSE_BUTTON_RIGHT && eKeysPressed['G'] == 0) {
      unselectEntity();
   }
}

//Mouse scroll callback for Edit Mode
void glfwEditScroll(GLFWwindow *window, double xOffset, double yOffset) {
   //Change dup num if N is pressed
   if(eKeysPressed['N'] == 1) {
      changeDupNumBy(yOffset);
   }
   //Change scale if F is being held along with scroll wheel
   if(eKeysPressed['F'] == 1) {
      //Scale in Z
      if(eKeysPressed['Z'] == 1) {
         if(eKeysPressed[341]) {
            scaleSelectedEntityZ(yOffset * 0.5);
         }
         else {
            scaleSelectedEntityZ(yOffset * 0.05);
         }
      }
      //Scale in X
      if(eKeysPressed['X'] == 1) {
         if(eKeysPressed[341]) {
            scaleSelectedEntityX(yOffset * 0.5);
         }
         else {
            scaleSelectedEntityX(yOffset * 0.05);
         }
      }
      //Scale in Y
      if(eKeysPressed['C'] == 1) {
         if(eKeysPressed[341]) {
            scaleSelectedEntityY(yOffset * 0.5);
         }
         else {
            scaleSelectedEntityY(yOffset * 0.05);
         }
      }
      //Else uniformly scaled
      else if(!eKeysPressed['Z'] && !eKeysPressed['X'] && !eKeysPressed['C']){
         scaleSelectedEntity(glm::vec3(yOffset * 0.05, yOffset * 0.05, yOffset * 0.05));
      }
   }
   //Change rotation if E is being held along with scroll wheel
   else if(eKeysPressed['E'] == 1) {
      //If left shift, change line rotation
      if(eKeysPressed[340]) {
         rotateLineAngle(yOffset);
      }
      //If left control, change spacing inbetween each selected entity
      else if(eKeysPressed[341]) {
         changeCESpacing(yOffset * 0.1);
      }
      //else rotate the model selected
      else {
         rotateSelectedEntities(yOffset);
      }
   }
   //If the change will be in range
   else if(getDistance() + yOffset * 0.1 <= 70.0 && getDistance() + yOffset * 0.1 >= 1.0) {
      //printf("getDistance %f\n", getDistance());
      //If l-shift increment by a tiny amount
      if(eKeysPressed[340] == 1) {
         addDistance(0.1 * yOffset);
      }
      //else if all other keys that involve scroll wheel weren't pressed
      else if(!eKeysPressed['N'] && !eKeysPressed['F'] && !eKeysPressed['E']) {
         addDistance(0.5 * yOffset);
      }
   }
}

void glfwEditGetCursorPos(GLFWwindow *window, double xpos, double ypos) {

   //printf("%f %f\n", xpos, g_height - ypos);
   lastCurPos = glm::vec2(xpos, g_height - ypos - 1);

   //If in GUI Selection, disable pitch and yaw
   if(eKeysPressed['G'] == 0) {

      if(xpos > g_width || xpos < 0 || ypos < 0 || ypos > g_height) {
         return;
      }

      //Get rid of if unneeded
      egaze = GetLookAt() - GetEye();
      ew = glm::vec3(-1.0 * ew.x, -1.0 * ew.y, -1.0 * ew.z);
      ew = glm::normalize(ew);
      eu = glm::cross(GetUp(), ew)/magnitude(glm::cross(GetUp(), ew));
      eu = glm::normalize(eu);

      eendX = xpos;
      eendY = g_height-ypos-1;

      float diff;

      //Calculate change in X
      if(estartX < eendX) {
         diff = eendX - estartX;
         ebeta = incrementYaw((diff * M_PI)/g_width);
      }
      else if(estartX > eendX){
         diff = estartX - eendX;
         ebeta = incrementYaw((-diff * M_PI)/g_width);
      }

      //Calculate change in Y
      if(eendY > estartY && ealpha <= 0.98) {
         diff = eendY - estartY;
         ealpha = incrementPitch((diff * M_PI)/g_width);
      }
      else if(eendY < estartY && ealpha >= -0.98) {
         diff = estartY - eendY;
         ealpha = incrementPitch(-(diff * M_PI)/g_width);
      }

      estartX = g_width/2.0;// = endX;
      estartY = g_height/2.0-1;// endY;
   }

}

/*
* the function that is called in the main loop that will act on the keys pressed
* that are kept track of inside of the array
*/
void glfwEditKeyboard(void) {
   ew = egaze/magnitude(egaze);
   ew = glm::vec3(-1.0 * ew.x, -1.0 * ew.y, -1.0 * ew.z);
   eu = glm::cross(GetUp(), ew)/magnitude(glm::cross(GetUp(), ew));

   //If in GUI Selection, disable all other actions
   if(eKeysPressed['G'] == 0) {
      //GLFW_KEY_S
      if(eKeysPressed['S']) {
          MoveEye(glm::vec3(speed * 0.15 * ew.x, speed * 0.15 * ew.y, speed * 0.15 * ew.z));
      }
      //GLFW_KEY_W
      if(eKeysPressed['W']) {
          MoveEye(glm::vec3(speed * -0.15 * ew.x, speed * -0.15 * ew.y, speed * -0.15 * ew.z));
      }
      //GLFW_KEY_D
      if(eKeysPressed['D']) {
          MoveEye(glm::vec3(speed * 0.15 * eu.x, speed * 0.15 * eu.y, speed * 0.15 * eu.z));
      }
      //GLFW_KEY_A
      if(eKeysPressed['A']) {
          MoveEye(glm::vec3(speed * -0.15 * eu.x, speed * -0.15 * eu.y, speed * -0.15 * eu.z));
      }
      //GLFW_KEY_R
      if(eKeysPressed['R']) {
         setDistance(previousLookAtDistance);
         reselectLastEntity();
      }
      //GLFW_KEY_BACKSPACE
       if(eKeysPressed[8]) {
         //l-shift, redo
         if(eKeysPressed[340]) {
            redo();
         }
         //else undo
         else
         {
            undo();
         }
         eKeysPressed[8] = 0;
      }
      //GLFW_KEY_1
      if(eKeysPressed['1']) {
         selectAtHotBarIndex(1);
      }
      //GLFW_KEY_2
      if(eKeysPressed['2']) {
         selectAtHotBarIndex(2);
      }
      //GLFW_KEY_3
      if(eKeysPressed['3']) {
         selectAtHotBarIndex(3);
      }
      //GLFW_KEY_4
      if(eKeysPressed['4']) {
         selectAtHotBarIndex(4);
      }
      //GLFW_KEY_5
      if(eKeysPressed['5']) {
         selectAtHotBarIndex(5);
      }
      //GLFW_KEY_6
      if(eKeysPressed['6']) {
         selectAtHotBarIndex(6);
      }
      //GLFW_KEY_7
      if(eKeysPressed['7']) {
         selectAtHotBarIndex(7);
      }
      //GLFW_KEY_8
      if(eKeysPressed['8']) {
         selectAtHotBarIndex(8);
      }
      //GLFW_KEY_9
      if(eKeysPressed['9']) {
         selectAtHotBarIndex(9);
      }
   }
   //GLFW_KEY_P
   if(eKeysPressed['P']) {
      pauseorUnpause();
   }
   //Quit and don't save if 'Q' + lShift/lctrl
   if(eKeysPressed['Q'] && eKeysPressed[340] || eKeysPressed['Q'] && eKeysPressed[341]) {
      toStartScreen();
   }
   //Quit and Save
   else if(eKeysPressed['Q']) {
      saveWorld();
      toStartScreen();
   }
}

//the keyboard callback that will alter the array based on key pressed/released
void glfwEditKeyPress(GLFWwindow *window, int key, int scan, int action, int mods) {
   if(action == GLFW_PRESS) {
      switch( key ) {
       case GLFW_KEY_S:
         eKeysPressed['S'] = 1;
         break;
       case GLFW_KEY_W:
         eKeysPressed['W'] = 1;
         break;
       case GLFW_KEY_D:
         eKeysPressed['D'] = 1;
         break;
       case GLFW_KEY_A:
         eKeysPressed['A'] = 1;
         break;
       case GLFW_KEY_R:
         eKeysPressed['R'] = 1;
         break;
       case GLFW_KEY_E:
         eKeysPressed['E'] = 1;
         break;
       //Scale in X
       case GLFW_KEY_X:
         eKeysPressed['X'] = 1;
         break;
       //Scale in Y
       case GLFW_KEY_C:
         eKeysPressed['C'] = 1;
         break;
       //Scale in Z
       case GLFW_KEY_Z:
         eKeysPressed['Z'] = 1;
         break;
       //Number of feature
       case GLFW_KEY_N:
         eKeysPressed['N'] = 1;
         break;
       //Hold and then scroll wheel to scale
       case GLFW_KEY_F:
         eKeysPressed['F'] = 1;
         break;
       //Undo
       case GLFW_KEY_BACKSPACE:
         eKeysPressed[8] = 1;
         break;
       //Left control + s = save
       case GLFW_KEY_LEFT_CONTROL:
         eKeysPressed[341] = 1;
         break;
       //Pause/unpause
       case GLFW_KEY_P:
         eKeysPressed['P'] = 1;
         break;
       //Allows for faster movement
       case GLFW_KEY_LEFT_SHIFT:
         eKeysPressed[340] = 1;
         speed = 10.0;
         break;
       //Gui
       case GLFW_KEY_G:
         if(eKeysPressed['G'] == 1) {
            eKeysPressed['G'] = 0;
         }
         else if(eKeysPressed['G'] == 0) {
            eKeysPressed['G'] = 1;
         }
         break;
       //Quit
       case GLFW_KEY_Q:
         eKeysPressed['Q'] = 1;
         break;
       //Select object 1 in hotbar
       case GLFW_KEY_1:
         eKeysPressed['1'] = 1;
         break;
       //Select object 2 in hotbar
       case GLFW_KEY_2:
         eKeysPressed['2'] = 1;
         break;
       //Select object 3 in hotbar
       case GLFW_KEY_3:
         eKeysPressed['3'] = 1;
         break;        
       case GLFW_KEY_4:
         eKeysPressed['4'] = 1;
         break;
       case GLFW_KEY_5:
         eKeysPressed['5'] = 1;
         break;
       case GLFW_KEY_6:
         eKeysPressed['6'] = 1;
         break;
       case GLFW_KEY_7:
         eKeysPressed['7'] = 1;
         break;
       case GLFW_KEY_8:
         eKeysPressed['8'] = 1;
         break;
       case GLFW_KEY_9:
         eKeysPressed['9'] = 1;
         break;
       case GLFW_KEY_0:
         eKeysPressed['0'] = 1;
         break;
     }
   }   
   else if(action == GLFW_RELEASE) {
      switch( key ) {
       case GLFW_KEY_S:
         eKeysPressed['S'] = 0;
         break;
       case GLFW_KEY_W:
         eKeysPressed['W'] = 0;
         break;
       case GLFW_KEY_D:
         eKeysPressed['D'] = 0;
         break;
       case GLFW_KEY_A:
         eKeysPressed['A'] = 0;
         break;
       case GLFW_KEY_R:
         eKeysPressed['R'] = 0;
         break;
       //Number of feature
       case GLFW_KEY_N:
         eKeysPressed['N'] = 0;
         break;
       case GLFW_KEY_F:
         eKeysPressed['F'] = 0;
         break;
       //Scale in X
       case GLFW_KEY_X:
         eKeysPressed['X'] = 0;
         break;
       //Scale in Y
       case GLFW_KEY_C:
         eKeysPressed['C'] = 0;
         break;
       //Scale in Z
       case GLFW_KEY_Z:
         eKeysPressed['Z'] = 0;
         break;
       case GLFW_KEY_E:
         eKeysPressed['E'] = 0;
         break;
       case GLFW_KEY_B:
         eKeysPressed['B'] = 0;
         break;
       //Undo
       case GLFW_KEY_BACKSPACE:
         eKeysPressed[8] = 0;
         break;
       //Left control + s = save
       case GLFW_KEY_LEFT_CONTROL:
         eKeysPressed[341] = 0;
         break;
       case GLFW_KEY_LEFT_SHIFT:
         eKeysPressed[340] = 0;
         speed = 1.0;
         break;
       case GLFW_KEY_Q:
         eKeysPressed['Q'] = 0;
         break;
       //Pause/unpause
       case GLFW_KEY_P:
         eKeysPressed['P'] = 0;
         break;
       case GLFW_KEY_1:
         eKeysPressed['1'] = 0;
         break;
       case GLFW_KEY_2:
         eKeysPressed['2'] = 0;
         break;
       case GLFW_KEY_3:
         eKeysPressed['3'] = 0;
         break; 
       case GLFW_KEY_4:
         eKeysPressed['4'] = 0;
         break;
       case GLFW_KEY_5:
         eKeysPressed['5'] = 0;
         break;
       case GLFW_KEY_6:
         eKeysPressed['6'] = 0;
         break;
       case GLFW_KEY_7:
         eKeysPressed['7'] = 0;
         break;
       case GLFW_KEY_8:
         eKeysPressed['8'] = 0;
         break;
       case GLFW_KEY_9:
         eKeysPressed['9'] = 0;
         break;
       case GLFW_KEY_0:
         eKeysPressed['0'] = 0;
         break;

     }
   }
}


