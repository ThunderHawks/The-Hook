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
#include "level.h"
#include "Lab4.h"
#include "SoundPlayer.h"

float startX, startY;
float endX, endY;
float alpha = 0.0;
float beta = -M_PI/2.0;

//Used to load the previous scroll distance for 'R' in edit mode
float previousLookAtDistance = 3.0;

//Used to increase speed when 'Left Shift' is pressed in edit
float speed = 1.0;

glm::vec3 gaze;
glm::vec3 w, u;

//Change size to increase amount of keys input
int KeysPressed[350];

//Mouse press callback for Edit Mode
void glfwEditMouse(GLFWwindow *window, int button, int action, int mods) {
   //If the left button is pressed
   if(button == GLFW_MOUSE_BUTTON_LEFT) {
      //If game is paused
      if(isPaused()) {
         pauseorUnpause();
      }
      //If an entity is selected, add it
      else if(areEntitiesSelected() == true) {
         placeSelectedEntity();
         //Reset lookAtDistance
         previousLookAtDistance = getDistance();
         setDistance(3.0);
      }
   }
   else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
      unselectEntity();
   }
}

//Mouse scroll callback for Edit Mode
void glfwEditScroll(GLFWwindow *window, double xOffset, double yOffset) {

   //Change dup num if N is pressed
   if(KeysPressed['N'] == 1) {
      printf("dupNum: %d\n", changeDupNumBy(yOffset));
   }
   //Change scale if F is being held along with scroll wheel
   if(KeysPressed['F'] == 1) {
      scaleSelectedEntity(glm::vec3(yOffset * 0.05, yOffset * 0.05, yOffset * 0.05));
   }
   //Change rotation if E is being held along with scroll wheel
   else if(KeysPressed['E'] == 1) {
      rotateSelectedEntities(yOffset);
   }
   //If the change will be in range
   else if(getDistance() + yOffset * 0.1 <= 30.0 && getDistance() + yOffset * 0.1 >= 1.0) {
      //If all other keys that involve scroll wheel weren't pressed
      if(!KeysPressed['N'] && !KeysPressed['F'] && !KeysPressed['E']) {
         addDistance(0.5 * yOffset);
      }
   }
}

void glfwGameGetCursorPos(GLFWwindow *window, double xpos, double ypos) {

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
      beta = incrementYaw((diff * M_PI)/g_width);
   }
   else if(startX > endX){
      diff = startX - endX;
      beta = incrementYaw(-(diff * M_PI)/g_width);
   }

   //Calculate change in Y
   if(endY < startY && alpha <= 0.8) {
      diff = startY - endY;
      alpha = incrementPitch((diff * M_PI)/g_width);
   }
   else if(endY > startY && alpha >= -0.5) {
      diff = endY - startY;
      alpha = incrementPitch(-(diff * M_PI)/g_width);
   }

   startX = g_width/2.0;// = endX;
   startY = g_height/2.0-1;// endY;
}

void glfwEditGetCursorPos(GLFWwindow *window, double xpos, double ypos) {

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
      beta = incrementYaw((diff * M_PI)/g_width);
   }
   else if(startX > endX){
      diff = startX - endX;
      beta = incrementYaw((-diff * M_PI)/g_width);
   }

   //Calculate change in Y
   if(endY > startY && alpha <= 0.98) {
      diff = endY - startY;
      alpha = incrementPitch((diff * M_PI)/g_width);
   }
   else if(endY < startY && alpha >= -0.98) {
      diff = startY - endY;
      alpha = incrementPitch(-(diff * M_PI)/g_width);
   }

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

void glfwGameKeyboard(void) {
  w = gaze/magnitude(gaze);
  w = glm::vec3(-1.0 * w.x, -1.0 * w.y, -1.0 * w.z);
  u = glm::cross(GetUp(), w)/magnitude(glm::cross(GetUp(), w));
   //GLFW_KEY_S
   if(KeysPressed['S']) {
       setPlayerSpeed(w.x*3,0,w.z*3);
       //newSpeed.x=newSpeed.x+w.x*3;
       //newSpeed.z=newSpeed.z+w.z*3;
   }
   //GLFW_KEY_W
   if(KeysPressed['W']) {
   	 printf("%lf %lf %lf\n", GetEye().x, GetEye().y, GetEye().z);
       setPlayerSpeed(-w.x*3,0,-w.z*3);
       //newSpeed.x=newSpeed.x-w.x*3;
       //newSpeed.z=newSpeed.z-w.z*3;
       printf("%lf %lf %lf\n", GetEye().x, GetEye().y, GetEye().z);
   }
   //GLFW_KEY_D
   if(KeysPressed['D']) {
       setPlayerSpeed(u.x*3,0,u.z*3);
       //newSpeed.x=newSpeed.x+u.x*3;
       //newSpeed.z=newSpeed.z+u.z*3;
   }
   //GLFW_KEY_A
   if(KeysPressed['A']) {
        setPlayerSpeed(-u.x*3,0,-u.z*3);
       //newSpeed.x=newSpeed.x-u.x*3;
       //newSpeed.z=newSpeed.z-u.z*3;
   }
   ///setPlayerSpeed(newSpeed.x,newSpeed.y,newSpeed.z);
   //GLFW_KEY_SPACE
   if(KeysPressed[' ']&&physGetPlayerY()<1.3) {
//      setPlayerSpeed(0,4,0);
      KeysPressed[' ']=0;
  ///    printf("Space is not implemented!\n");
      physJump();
      //PlayFX(JUMP_FX);
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
      //PlayFX(THROW_GRAP_FX);
//      if(KeysPressed['E']==1){
      physGrapple(w.x,w.y,w.z);
         //KeysPressed['E']=2;
  //    }
      //else physGrapplePoint();

   }
   if(KeysPressed['M']){
   	if (KeysPressed['M'] == 1)
			pauseBackground();
			
      KeysPressed['M']=2;
   }
}

/*
* the function that is called in the main loop that will act on the keys pressed
* that are kept track of inside of the array
*/
void glfwEditKeyboard(void) {
  w = gaze/magnitude(gaze);
  w = glm::vec3(-1.0 * w.x, -1.0 * w.y, -1.0 * w.z);
  u = glm::cross(GetUp(), w)/magnitude(glm::cross(GetUp(), w));
   //GLFW_KEY_LEFTCONTROL + 'S'
   if(KeysPressed[341] && KeysPressed['S']) {
      string toSave;
      pauseorUnpause();
      printf("Enter a string to have this world saved (don't include .wub, type 'none' to abort save):\n");
      scanf("%s", &toSave[0]);

      if(strcmp(&toSave[0], "abort") == 0) {
         printf("Save aborted\n");
         return;
      }

      printf("Going to save world as: %s\n", &toSave[0]);
      saveWorld(&toSave[0]);
   }
   //GLFW_KEY_S
   else if(KeysPressed['S']) {
       MoveEye(glm::vec3(speed * 0.15 * w.x, speed * 0.15 * w.y, speed * 0.15 * w.z));
   }
   //GLFW_KEY_W
   if(KeysPressed['W']) {
       MoveEye(glm::vec3(speed * -0.15 * w.x, speed * -0.15 * w.y, speed * -0.15 * w.z));
   }
   //GLFW_KEY_D
   if(KeysPressed['D']) {
       MoveEye(glm::vec3(speed * 0.15 * u.x, speed * 0.15 * u.y, speed * 0.15 * u.z));
   }
   //GLFW_KEY_A
   if(KeysPressed['A']) {
       MoveEye(glm::vec3(speed * -0.15 * u.x, speed * -0.15 * u.y, speed * -0.15 * u.z));
   }
   //GLFW_KEY_R
   if(KeysPressed['R']) {
      setDistance(previousLookAtDistance);
      reselectLastEntity();
   }
   //GLFW_KEY_BACKSPACE
   if(KeysPressed[8]) {
      undo();
      KeysPressed[8] = 0;
   }
   //GLFW_KEY_1
   if(KeysPressed['1']) {
      selectAtHotBarIndex(1);
   }
   //GLFW_KEY_2
   if(KeysPressed['2']) {
      selectAtHotBarIndex(2);
   }
   //GLFW_KEY_3
   if(KeysPressed['3']) {
      selectAtHotBarIndex(3);
   }
   //GLFW_KEY_4
   if(KeysPressed['4']) {
      selectAtHotBarIndex(4);
   }
   //GLFW_KEY_5
   if(KeysPressed['5']) {
      selectAtHotBarIndex(5);
   }
   //GLFW_KEY_6
   if(KeysPressed['6']) {
      selectAtHotBarIndex(6);
   }
   //GLFW_KEY_7
   if(KeysPressed['7']) {
      selectAtHotBarIndex(7);
   }
   //GLFW_KEY_8
   if(KeysPressed['8']) {
      selectAtHotBarIndex(8);
   }
   //GLFW_KEY_9
   if(KeysPressed['9']) {
      selectAtHotBarIndex(9);
   }
   //GLFW_KEY_0
   if(KeysPressed['0']) {
      selectAtHotBarIndex(0);
   }
   //GLFW_KEY_P
   if(KeysPressed['P']) {
      pauseorUnpause();
   }
   //Quit and save
   if(KeysPressed['Q'] && KeysPressed[341]) {
      saveWorld();
      exit( EXIT_SUCCESS );
   }
   //Quit and don't save
   else if(KeysPressed['Q']) {
      exit( EXIT_SUCCESS );
   }
}

//the keyboard callback that will alter the array based on key pressed/released
void glfwGameKeyPress(GLFWwindow *window, int key, int scan, int action, int mods) {
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
     case GLFW_KEY_M:
         if(!KeysPressed['M'])
            KeysPressed['M'] = 1;
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
     case GLFW_KEY_M:
         KeysPressed['M'] = 0;
         break;
     }
   }
}

//the keyboard callback that will alter the array based on key pressed/released
void glfwEditKeyPress(GLFWwindow *window, int key, int scan, int action, int mods) {
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
       case GLFW_KEY_R:
         KeysPressed['R'] = 1;
         break;
       case GLFW_KEY_E:
         KeysPressed['E'] = 1;
         break;
       //Number of feature
       case GLFW_KEY_N:
         KeysPressed['N'] = 1;
         break;
       //Hold and then scroll wheel to scale
       case GLFW_KEY_F:
         KeysPressed['F'] = 1;
         break;
       //Undo
       case GLFW_KEY_BACKSPACE:
         KeysPressed[8] = 1;
         break;
       //Left control + s = save
       case GLFW_KEY_LEFT_CONTROL:
         KeysPressed[341] = 1;
         break;
       //Pause/unpause
       case GLFW_KEY_P:
         KeysPressed['P'] = 1;
         break;
       //Allows for faster movement
       case GLFW_KEY_LEFT_SHIFT:
         KeysPressed[340] = 1;
         speed = 2.0;
         break;
       //Quit
       case GLFW_KEY_Q:
         KeysPressed['Q'] = 1;
         break;
       //Select object 1 in hotbar
       case GLFW_KEY_1:
         KeysPressed['1'] = 1;
         break;
       //Select object 2 in hotbar
       case GLFW_KEY_2:
         KeysPressed['2'] = 1;
         break;
       //Select object 3 in hotbar
       case GLFW_KEY_3:
         KeysPressed['3'] = 1;
         break;        
       case GLFW_KEY_4:
         KeysPressed['4'] = 1;
         break;
       case GLFW_KEY_5:
         KeysPressed['5'] = 1;
         break;
       case GLFW_KEY_6:
         KeysPressed['6'] = 1;
         break;
       case GLFW_KEY_7:
         KeysPressed['7'] = 1;
         break;
       case GLFW_KEY_8:
         KeysPressed['8'] = 1;
         break;
       case GLFW_KEY_9:
         KeysPressed['9'] = 1;
         break;
       case GLFW_KEY_0:
         KeysPressed['0'] = 1;
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
       case GLFW_KEY_R:
         KeysPressed['R'] = 0;
         break;
       //Number of feature
       case GLFW_KEY_N:
         KeysPressed['N'] = 0;
         break;
       case GLFW_KEY_F:
         KeysPressed['F'] = 0;
         break;
       case GLFW_KEY_E:
         KeysPressed['E'] = 0;
         break;
       //Undo
       case GLFW_KEY_BACKSPACE:
         KeysPressed[8] = 0;
         break;
       //Left control + s = save
       case GLFW_KEY_LEFT_CONTROL:
         KeysPressed[341] = 0;
         break;
       case GLFW_KEY_LEFT_SHIFT:
         KeysPressed[340] = 0;
         speed = 1.0;
         break;
       case GLFW_KEY_Q:
         KeysPressed['Q'] = 0;
         break;
       //Pause/unpause
       case GLFW_KEY_P:
         KeysPressed['P'] = 0;
         break;
       case GLFW_KEY_1:
         KeysPressed['1'] = 0;
         break;
       case GLFW_KEY_2:
         KeysPressed['2'] = 0;
         break;
       case GLFW_KEY_3:
         KeysPressed['3'] = 0;
         break; 
       case GLFW_KEY_4:
         KeysPressed['4'] = 0;
         break;
       case GLFW_KEY_5:
         KeysPressed['5'] = 0;
         break;
       case GLFW_KEY_6:
         KeysPressed['6'] = 0;
         break;
       case GLFW_KEY_7:
         KeysPressed['7'] = 0;
         break;
       case GLFW_KEY_8:
         KeysPressed['8'] = 0;
         break;
       case GLFW_KEY_9:
         KeysPressed['9'] = 0;
         break;
       case GLFW_KEY_0:
         KeysPressed['0'] = 0;
         break;

     }
   }
}
