/*
 *  CPE 471 lab 4 - draws a box without sides
 *  glut/OpenGL application 
 *  uses glm for matrices
 *  Has support for simple transforms - check the keyboard function
 *
 *  Created by zwood on 1/6/12 
 *  Copyright 2010 Cal Poly. All rights reserved.
 *
 *****************************************************************************/
#include "Lab4.h"
#include "Mesh.h"
#include "mesh_loader.h"
#include "InitHelpers.h"
#include "Helper.h"
#include "IO.h"
#include "physSystem.h"
#include "Shapes.h"
#include "level.h"
#include <vector>
#include "Camera.h"
#include "ViewFrustum.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

//position and color data handles
GLuint triBuffObj, colBuffObj;

int ShadeMode = 0;

//flag and ID to toggle on and off the shader
int ShadeProg;

//Handles to the shader data
GLint h_aPosition, h_aNormal, h_uViewMatrix, h_uProjMatrix;
GLuint CubeBuffObj, CIndxBuffObj, GrndBuffObj, GIndxBuffObj, GNBuffObj, GNIndxBuffObj;
GLuint ShadowCubeBuffObj, SCIndxBuffObj, ShadowNormalBuffObj, RampBuffObj, RIndxBuffObj, RampNormalBuffObj;
int g_CiboLen, g_GiboLen, g_RiboLen, g_SCiboLen;

float g_width, g_height;
GLint h_uLightVec;
GLint h_uLightColor;
GLint h_uCamPos, h_uShadeMode;
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;

//declare Matrix stack
RenderingHelper ModelTrans;

GLint h_uModelMatrix;
GLuint NormalBuffObj;
GLuint MeshBuffObj, MeshIndxBuffObj;

//The assimp mesh stuff
Mesh playerMesh;

/*Do something better with this later. I don't like that it is a global but oh well for now.*/
glm::mat4 curProj, curView;

/* projection matrix  - do not change */
glm::mat4 SetProjectionMatrix() {
   glm::mat4 Projection = glm::perspective(80.0f, (float)g_width/g_height, 0.1f, 100.f);	
   safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
   return Projection;
}

void DrawShadow(float x, float z, float Sx, float Sy, float Sz, float angle) {
   /*Shadow*/
   SetModel(x, -0.5, z, Sx, Sy, Sz, angle);
   SetMaterial(1);
   safe_glEnableVertexAttribArray(h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, ShadowCubeBuffObj);
   safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

   safe_glEnableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, ShadowNormalBuffObj);
   safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

   /* draw!*/
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SCIndxBuffObj);
   glDrawElements(GL_TRIANGLES, g_SCiboLen, GL_UNSIGNED_SHORT, 0);

   /* Disable the attributes used by our shader*/
   safe_glDisableVertexAttribArray(h_aPosition);
   safe_glDisableVertexAttribArray(h_aNormal);
}

void SetupCube(float x, float y, float z, int material, float angle, float scaleX, float scaleY, float scaleZ) {
   /*First Cube*/
   SetModel(x, y, z, scaleX, scaleY, scaleZ, angle);
   SetMaterial(material);
   safe_glEnableVertexAttribArray(h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, CubeBuffObj);
   safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

   safe_glEnableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, NormalBuffObj);
   safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

   /* draw!*/
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CIndxBuffObj);

   glDrawElements(GL_TRIANGLES, g_CiboLen, GL_UNSIGNED_SHORT, 0);

   /* Disable the attributes used by our shader*/
   safe_glDisableVertexAttribArray(h_aPosition);
   safe_glDisableVertexAttribArray(h_aNormal);
//   DrawShadow(x, z + 0.6, scaleX, scaleY, scaleZ + 0.4, angle);
}


/* Main display function */
void glfwDraw (GLFWwindow *window)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   /*Start our shader      */
   glUseProgram(ShadeProg);
   curProj = SetProjectionMatrix();
   curView = SetView();

   glUniform3f(h_uLightColor, 0.4, 0.4, 0.38);
   glUniform4f(h_uLightVec, 0.0, -1.0, 1.0, 0.0);
   glUniform3f(h_uCamPos, GetEye().x, GetEye().y, GetEye().z);

   ModelTrans.loadIdentity();
   SetModelStat();

   safe_glEnableVertexAttribArray(h_aPosition);

   glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
   safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);

   SetMaterial(0);

   safe_glEnableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, GNBuffObj);
   safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

   /* draw!*/
   glDrawElements(GL_TRIANGLES, g_GiboLen, GL_UNSIGNED_SHORT, 0);
   safe_glDisableVertexAttribArray(h_aPosition);

   //DRAW THE DANCING CYLINDER HERE!!
   btTransform pla;
   PlaceModel(playerMesh, GetLookAt().x, GetLookAt().y - 1, GetLookAt().z, .25, .1, .25, 1);
   //END OF DANCING CYLINDER CODE HERE!!

   //Draw Cubes
   for(float i=.05;i<1;i+=.0075){
      srand(physGetPlayerX());
      float x = physGetPlayerX()*(1-i)+grapplingHookLocation().x*i;
      float y = physGetPlayerY()*(1-i)*(1-i)+grapplingHookLocation().y*i*i;
      float z = physGetPlayerZ()*(1-i)+grapplingHookLocation().z*i;
      SetupCube(x,y,z,5,rand()/300.0,.15,.15,.15);
   }
   SetMaterial(2);
   //draw phys cubes
   vector<btRigidBody*> loopable = getVecList();
   srand(0);
   for(int i = 0;i<loopable.size();i++){
      btTransform trans;
      loopable[i]->getMotionState()->getWorldTransform(trans);
      //printf("actual is %f %f %f\n",trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ());
   
	if(!i)
		PlaceModel(*(Mesh*)(loopable[i]->getUserPointer()), trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ(),.15*SCALE,-.05*SCALE,.15*SCALE,1);
	else  
		PlaceModel(*(Mesh*)(loopable[i]->getUserPointer()), trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ(),.1*SCALE,.1*SCALE,.1*SCALE,rand()/300.0);
     // SetupCube(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ(),2,0,2,2,2);
   }

   ///render spherse
/*
   btTransform trans;
   fallRigidBody->getMotionState()->getWorldTransform(trans);
   std::cout << "sphere height: " << trans.getOrigin().getY() << " sphere x:"<<trans.getOrigin().getX()<< " sphere z:"<< trans.getOrigin().getZ() <<std::endl;
   SetupCube(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ(),2,60,2,2,2);
   fallRigidBodyb->getMotionState()->getWorldTransform(trans);
   SetupCube(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ(),2,60,2,2,2);
   FRBbuilding->getMotionState()->getWorldTransform(trans);
   SetupCube(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ(),3,0,2,2,2);
*/
   //Disable the shader
   glUseProgram(0);	
   glfwSwapBuffers(window);

}
//not in use
void initWorldEdit(GLFWwindow *window) {
   window = glfwCreateWindow(800, 800, "World Editor", NULL, NULL);
   if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
   }
   srand(time(0));
   glfwMakeContextCurrent(window);
   glfwSetWindowPos(window, 80, 80);
   glfwSetWindowSizeCallback(window, glfwWindowResize);
   glfwSetWindowSize(window,1600,800);
   g_height =800;
   g_width = 1600;

   glfwSetKeyCallback(window, glfwKeyPress);
   glfwSetCursorPosCallback( window, glfwEditGetCursorPos );

   glewInit();
   glInitialize(window);
   InitGeom();
}
//not in use
void initGamePlay(GLFWwindow *window) {
   window = glfwCreateWindow(800, 800, "Grapple", NULL, NULL);
   if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
   }
   srand(time(0));
   glfwMakeContextCurrent(window);
   glfwSetWindowPos(window, 80, 80);
   glfwSetWindowSizeCallback(window, glfwWindowResize);
   glfwSetWindowSize(window,1600,800);
   g_height =800;
   g_width = 1600;

   glfwSetKeyCallback(window, glfwKeyPress);
   glfwSetCursorPosCallback( window, glfwGameGetCursorPos );

   glewInit();
   glInitialize(window);
   physicsInit();
   InitGeom();

   loadLevel();
}

int main( int argc, char *argv[] )
{
   GLFWwindow *window;
   int Edit;

   glfwSetErrorCallback(glfwError);
   if (!glfwInit()) {
      exit(EXIT_FAILURE);
   }

   printf("Type 0 to play, any other int to edit\n");
   scanf("%i", &Edit);

   //If Edit Mode
   if(Edit) {
      //World Edit Init
      //initWorldEdit(window);
      window = glfwCreateWindow(800, 800, "World Editor", NULL, NULL);
      if (!window) {
         glfwTerminate();
         exit(EXIT_FAILURE);
      }
      srand(time(0));
      glfwMakeContextCurrent(window);
      glfwSetWindowPos(window, 80, 80);
      glfwSetWindowSizeCallback(window, glfwWindowResize);
      glfwSetWindowSize(window,1600,800);
      g_height =800;
      g_width = 1600;

      glfwSetKeyCallback(window, glfwKeyPress);
      glfwSetCursorPosCallback( window, glfwEditGetCursorPos );

      glewInit();
      glInitialize(window);
      InitGeom();
   }
   //If Play Mode
   else {
      //Game Play Init
      //initGamePlay(window);
      window = glfwCreateWindow(800, 800, "Grapple", NULL, NULL);
      if (!window) {
         glfwTerminate();
         exit(EXIT_FAILURE);
      }
      srand(time(0));
      SetEye(vec3(0, 0, 0));
      glfwMakeContextCurrent(window);
      glfwSetWindowPos(window, 80, 80);
      glfwSetWindowSizeCallback(window, glfwWindowResize);
      glfwSetWindowSize(window,1600,800);
      g_height =800;
      g_width = 1600;

      glfwSetKeyCallback(window, glfwKeyPress);
      glfwSetCursorPosCallback( window, glfwGameGetCursorPos );

      glewInit();
      glInitialize(window);
      physicsInit();
      InitGeom();
      loadLevel();
   }

	if (Edit)
		setDistance(.0001);
	else
		setDistance(10);
   // Start the main execution loop.
   while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      if(Edit) {
         //Keep the cursor centered
         glfwSetCursorPos(window,g_width/2,g_height/2);  
         glfwDraw(window);
         glfwEditGetCursorPos(NULL,g_width/2.0,g_height/2.0);
         //glfw Game Keyboard
         glfwEditKeyboard();
      }
      else {
         //player appy physics controls
         SetLookAt(glm::vec3(physGetPlayerX(),physGetPlayerY(),physGetPlayerZ()));
         SetSpeed(.05*magnitude(getPlayerSpeed()));
         //Keep the cursor centered
         glfwSetCursorPos(window,g_width/2,g_height/2);         
         physStep();
         //Draw stuff
         glfwDraw(window);
         glfwGameGetCursorPos(NULL,g_width/2.0,g_height/2.0);
         //glfw Game Keyboard
         glfwGameKeyboard();
      }
      usleep(15000);
   }

   // Clean up after GLFW.
   glfwDestroyWindow(window);
   glfwTerminate();
   exit(EXIT_SUCCESS);
}
