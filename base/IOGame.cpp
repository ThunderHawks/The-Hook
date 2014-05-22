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
#include "IOGame.h"
#include "physSystem.h"
#include "Camera.h"
#include "level.h"
#include "Lab4.h"
#include "SoundPlayer.h"

//g == game
float gstartX, gstartY;
float gendX, gendY;
float galpha = 0.0;
float gbeta = -M_PI/2.0;
int zoomed = 0;

glm::vec3 ggaze;
glm::vec3 gw, gu;

//Change size to increase amount of keys input
int gKeysPressed[255];

void glfwPlayMouse(GLFWwindow *window, int button, int action, int mods) {
   //If the left button is pressed
   if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
   	//find the direction to throw the grappling hook
      glm::vec3 dir = GetLookAt()-GetEye();
      //check to see if the grappling hook hit. If it did, play a sound
      if (physGrapple(-dir.x,-dir.y,-dir.z))
      	PlayFX(THROW_GRAP_FX);
      
      //pull on the grapple
      holdGrapple(1);
   }
   if(button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE){
      holdGrapple(0);
      physJump();
   }
   else if(button == GLFW_MOUSE_BUTTON_RIGHT) {
   	//zoom in
      zoomed = !zoomed;
   }
}

void glfwGameGetCursorPos(GLFWwindow *window, double xpos, double ypos) {

   if(xpos > g_width || xpos < 0 || ypos < 0 || ypos > g_height) {
      return;
   }

   //Get rid of if unneeded
   ggaze = GetLookAt() - GetEye();
   gw = glm::vec3(-1.0 * gw.x, -1.0 * gw.y, -1.0 * gw.z);
   gw = glm::normalize(gw);
   gu = glm::cross(GetUp(), gw)/magnitude(glm::cross(GetUp(), gw));
   gu = glm::normalize(gu);

   gendX = xpos;
   gendY = g_height-ypos-1;

   float diff;

   //Calculate change in X
   if(gstartX < gendX) {
      diff = gendX - gstartX;
      gbeta = incrementYaw(.5*(diff * M_PI)/g_width);
   }
   else if(gstartX > gendX){
      diff = gstartX - gendX;
      gbeta = incrementYaw(-.5*(diff * M_PI)/g_width);
   }

   //Calculate change in Y
   if(gendY < gstartY && galpha <= 1.3) {
      diff = gstartY - gendY;
      galpha = incrementPitch((diff * M_PI)/g_width);
   }
   else if(gendY > gstartY && galpha >= -0.5) {
      diff = gendY - gstartY;
      galpha = incrementPitch(-(diff * M_PI)/g_width);
   }

   gstartX = g_width/2.0;// = endX;
   gstartY = g_height/2.0-1;// endY;
}

/*
*  returns bool of character pressed
*  takes the character for a given key
*/
int getGPressed(char ch){
   return gKeysPressed[ch];
}

void glfwGameKeyboard(void) {
   if(zoomed) setDistance(2);

  gw = ggaze/magnitude(ggaze);
  gw = glm::vec3(-1.0 * gw.x, -1.0 * gw.y, -1.0 * gw.z);
  gu = glm::cross(GetUp(), gw)/magnitude(glm::cross(GetUp(), gw));
   //GLFW_KEY_S
   if(gKeysPressed['S']) {
       if(canMove()==1) setPlayerSpeed(gw.x*3,0,gw.z*3);
       else setPlayerSpeed(gw.x*1,0,gw.z*1);
       //newSpeed.x=newSpeed.x+w.x*3;
       //newSpeed.z=newSpeed.z+w.z*3;
   }
   //GLFW_KEY_W
   if(gKeysPressed['W']) {
   	 //printf("%lf %lf %lf\n", GetEye().x, GetEye().y, GetEye().z);
       //printf("%f player X %f player Y\n",physGetPlayerX(),physGetPlayerZ());
       //if(canMove()==1) setPlayerSpeed(-w.x*3,0,-w.z*3);
       /*else */setPlayerSpeed(-gw.x*3,0,-gw.z*3);
       //newSpeed.x=newSpeed.x-w.x*3;
       //newSpeed.z=newSpeed.z-w.z*3;
       //printf("%lf %lf %lf\n", GetEye().x, GetEye().y, GetEye().z);
   }
   //GLFW_KEY_D
   if(gKeysPressed['D']) {
       if(canMove()==1) setPlayerSpeed(gu.x*3,0,gu.z*3);
        else setPlayerSpeed(gu.x*3,0,gu.z*3);

       //newSpeed.x=newSpeed.x+u.x*3;
       //newSpeed.z=newSpeed.z+u.z*3;
   }
   //GLFW_KEY_A
   if(gKeysPressed['A']) {
        if(canMove()==1) 
        		setPlayerSpeed(-gu.x*3,0,-gu.z*3);
       else 
 				setPlayerSpeed(-gu.x*3,0,-gu.z*3);
       //newSpeed.x=newSpeed.x-u.x*3;
       //newSpeed.z=newSpeed.z-u.z*3;
   }
   ///setPlayerSpeed(newSpeed.x,newSpeed.y,newSpeed.z);
   //GLFW_KEY_SPACE
   if(gKeysPressed[' ']) {
//      setPlayerSpeed(0,4,0);
      //KeysPressed[' ']=0;
      //printf("Space is not implemented!\n");
      physJump();
      //PlayFX(JUMP_FX);
   }
   //GLFW_KEY_Q
   if(gKeysPressed['Q']) {
      exit( EXIT_SUCCESS );
   }
   if(gKeysPressed['F']){
     // printf("box %f %f %f\n",-gaze.x*5,gaze.y*5,-gaze.z*5);
   //   createStaticBox(eye.x,eye.y,eye.z,1,1,1,btQuaternion(0,0,0,1),1,gaze.x*10,gaze.y*10,gaze.z*10);
      gKeysPressed['F']=0;
   }
   if(gKeysPressed['E']){
      
         //KeysPressed['E']=2;
  //    }
      //else physGrapplePoint();

   }
	if(gKeysPressed['K']){
   	if (gKeysPressed['K'] == 1)
			musicPlayer.prevSong();
      gKeysPressed['K'] = 2;
   }
	if(gKeysPressed['L']){
   	if (gKeysPressed['L'] == 1)
			musicPlayer.nextSong();
      gKeysPressed['L'] = 2;
   }
   if(gKeysPressed['M']){
   	if (gKeysPressed['M'] == 1)
			musicPlayer.nextSong();
			
      gKeysPressed['M'] = 2;
   }

   if (gKeysPressed['N']) {
      if (++ShadeMode == 2)
         ShadeMode = 0;
      gKeysPressed['N'] = 0;
   }
}


//the keyboard callback that will alter the array based on key pressed/released
void glfwGameKeyPress(GLFWwindow *window, int key, int scan, int action, int mods) {
   if(action == GLFW_PRESS) {
      switch( key ) {
       case GLFW_KEY_S:
         gKeysPressed['S'] = 1;
         break;
       case GLFW_KEY_W:
         gKeysPressed['W'] = 1;
         break;
       case GLFW_KEY_D:
         gKeysPressed['D'] = 1;
         break;
       case GLFW_KEY_A:
         gKeysPressed['A'] = 1;
         break;
       case GLFW_KEY_SPACE:
         gKeysPressed[' '] = 1;
         break;
       case GLFW_KEY_Q:
         gKeysPressed['Q'] = 1;
         break;
       case GLFW_KEY_F:
         gKeysPressed['F'] = 1;
         break;
		 case GLFW_KEY_E:
         if(!gKeysPressed['E'])
            gKeysPressed['E'] = 1;
				break;
		 case GLFW_KEY_K:
         if(!gKeysPressed['K'])
            gKeysPressed['K'] = 1;
				break;
		 case GLFW_KEY_L:
         if(!gKeysPressed['L'])
            gKeysPressed['L'] = 1;
				break;
		 case GLFW_KEY_M:
         if(!gKeysPressed['M'])
            gKeysPressed['M'] = 1;
				break;
		 case GLFW_KEY_N:
         if(!gKeysPressed['N'])
            gKeysPressed['N'] = 1;
         	break;
		 case GLFW_KEY_B:
      	gKeysPressed['B'] = 1;
     			break;
   	 case GLFW_KEY_V:
      	gKeysPressed['V'] = 1;
     			break;
     }
   }   
   else if(action == GLFW_RELEASE) {
      switch( key ) {
       case GLFW_KEY_S:
         gKeysPressed['S'] = 0;
         break;
       case GLFW_KEY_W:
         gKeysPressed['W'] = 0;
         break;
       case GLFW_KEY_D:
         gKeysPressed['D'] = 0;
         break;
       case GLFW_KEY_A:
         gKeysPressed['A'] = 0;
         break;
       case GLFW_KEY_SPACE:
         gKeysPressed[' '] = 0;
         break;
       case GLFW_KEY_Q:
         gKeysPressed['Q'] = 0;
         break;
       case GLFW_KEY_F:
         gKeysPressed['F'] = 0;
         break;
       case GLFW_KEY_E:
         gKeysPressed['E'] = 0;
         break;
		 case GLFW_KEY_K:
         gKeysPressed['K'] = 0;
         break;
		 case GLFW_KEY_L:
         gKeysPressed['L'] = 0;
         break;
		 case GLFW_KEY_M:
         gKeysPressed['M'] = 0;
         break;
		 case GLFW_KEY_N:
         gKeysPressed['N'] = 0;
         break;
		 case GLFW_KEY_B:
			gKeysPressed['B'] = 0;
			break;
		 case GLFW_KEY_V:
			gKeysPressed['V'] = 0;
			break;
     }
   }
}

