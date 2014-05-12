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
#include <stdio.h>
#include <vector>
#include <string>
#include "Camera.h"
#include "Shadows.h"
#include "SoundPlayer.h"
#include "Objective.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

//Paused/unpause
bool paused;

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
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine, h_uMatAlpha;
GLint h_uTexUnit;
GLint h_uLightViewMatrix, h_uLightProjMatrix;

//declare Matrix stack
RenderingHelper ModelTrans;

GLint h_uModelMatrix;
GLuint NormalBuffObj;
GLuint MeshBuffObj, MeshIndxBuffObj;

//The assimp mesh stuff
Mesh playerMesh;
Mesh flag;

/*Do something better with this later. I don't like that it is a global but oh well for now.*/
glm::mat4 curProj, curView;

vector<Objective*> objectives;

/* projection matrix  - do not change */
glm::mat4 SetProjectionMatrix() {
   glm::mat4 Projection = glm::perspective(80.0f, (float)g_width/g_height, 0.1f, 500.f);	
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

//Draws the currently selected entity
void drawSelectedObjects() {

   updateCurrentEntitiesPos();
   //Only draw if entitie(s) are selected
   if(areEntitiesSelected() == true) {
      //Get selected entities
      vector<Entity> tempEntities = getSelectedEntities();

      //Place each selected objects
      for(int i = 0; i < tempEntities.size(); i++) {
         PlaceModel(*tempEntities.at(i).mesh, tempEntities.at(i).position.x, tempEntities.at(i).position.y, tempEntities.at(i).position.z, tempEntities.at(i).scale.x, tempEntities.at(i).scale.y, tempEntities.at(i).scale.z, tempEntities.at(i).angle, tempEntities.at(i).BSRadius);
      }
   }
}
float sizer = 45;
float cool = 0;
//Draws the entities into the world
void drawEntities() {
   Entity entityTemp;
   srand(sizer);
   for(int i = 0; i < getEntityNum(); i++) {
      entityTemp = getEntityAt(i);
      int mat = rand()%13;
      while(!(mat = rand()%13));
      SetMaterial(mat);
      PlaceModel(*entityTemp.mesh, entityTemp.position.x, entityTemp.position.y, entityTemp.position.z, entityTemp.scale.x*(sin(sizer)*.3+1), entityTemp.scale.y*(sin(sizer)*.3+1), entityTemp.scale.z*(sin(sizer)*.3+1), entityTemp.angle+sin(sizer)*10, entityTemp.BSRadius);
   }
   //printf("cool? %d %d %f\n",cool,getPressed('B'),cos(sizer));
   if(getPressed('B')) cool = 1;
   else{
    cool = 0;
    sizer=0;
   }
   if(cool) sizer+=.16;
}

//Bool that returns true if game is paused
bool isPaused() {
   if(paused == true) {
      return paused;
   }  
   return false;
}

//Pauses/unpasses game
void pauseorUnpause() {
   if(paused == true) {
      paused = false;
   }
   else {
      paused = true;
   }
}

/* Main display function */
void glfwDraw (GLFWwindow *window)
{
   //Enable transparency
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Disable backface culling for skybox
   glCullFace(GL_BACK);
   glDisable(GL_CULL_FACE);

   // Draw skybox
   ModelTrans.loadIdentity();
   DrawSkyBox();
   SetModelStat();

   // Enable backface culling
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);

   safe_glEnableVertexAttribArray(h_aPosition);
   safe_glEnableVertexAttribArray(h_aNormal);

   glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
   safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);

   SetMaterial(0);

   glBindBuffer(GL_ARRAY_BUFFER, GNBuffObj);
   safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

   /* draw!*/
   glDrawElements(GL_TRIANGLES, g_GiboLen, GL_UNSIGNED_SHORT, 0);

   // Disable attributes
   safe_glDisableVertexAttribArray(h_aPosition);
   safe_glDisableVertexAttribArray(h_aNormal);

   //DRAW THE DANCING CYLINDER HERE!!
   btTransform pla;
   //PlaceModel(playerMesh, GetLookAt().x, GetLookAt().y - 1, GetLookAt().z, .25, .1, .25, 1, 1.7);
   //END OF DANCING CYLINDER CODE HERE!!
   SetMaterial(2);
   drawSelectedObjects();
   drawEntities();

   // Disable backface culling for skybox
   glCullFace(GL_BACK);
   glDisable(GL_CULL_FACE);

   //Draw Cubes, ??????????
   SetupCube(plsRndr().getX(),plsRndr().getY(),plsRndr().getZ(),5,60,1,1,1);
   for(float i=.05;i<1;i+=.0075){
      srand(physGetPlayerX());
      float x = physGetPlayerX()*(1-i)+grapplingHookLocation().x*i;
      float y = physGetPlayerY()*(1-i)*(1-i)+grapplingHookLocation().y*i*i;
      float z = physGetPlayerZ()*(1-i)+grapplingHookLocation().z*i;
      SetupCube(x,y,z,5,rand()/300.0,.15,.15,.15);
   }

   SetMaterial(2);

   vector<btRigidBody*> loopable = getVecList();
   srand(0);
   for(int i = 0;i<loopable.size();i++){
      btTransform trans;
      loopable[i]->getMotionState()->getWorldTransform(trans);
      //printf("actual is %f %f %f\n",trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ());
   if(loopable[i]->getUserPointer()){
	   if(!i)
		   PlaceModel(*(Mesh*)(loopable[i]->getUserPointer()), trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ(),.1*SCALE,.1*SCALE,.1*SCALE,1, 1.7);
	   else  
		   PlaceModel(*(Mesh*)(loopable[i]->getUserPointer()), trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ(),.1*SCALE,.1*SCALE,.1*SCALE, 0, 1);
        // SetupCube(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ(),2,0,2,2,2);
      }
   }

   //draw phys cubes
   for(int i = 0; i < objectives.size();i++){
      if(objectives[i]->active){
         PlaceModel(flag,objectives[i]->end.x, objectives[i]->end.y, objectives[i]->end.z, 50, 50, 50, 1, 1.7);
         SetupCube(objectives[i]->end.x, objectives[i]->end.y, objectives[i]->end.z, 16, 60, 10, 5000, 10);
      }
      else{
         PlaceModel(flag,objectives[i]->start.x, objectives[i]->start.y, objectives[i]->start.z, 50, 50, 50, 1, 1.7);
         SetupCube(objectives[i]->start.x, objectives[i]->start.y, objectives[i]->start.z, 15, 60, 10, 5000, 10);
      }
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
}

void renderScene(GLFWwindow *window, ShadowMap *shadowMap) {
   glm::vec3 origEye = GetEye();
   glm::vec3 origLookAt = GetLookAt();
   
   glUseProgram(ShadeProg);

   glUniform1i(h_uShadeMode, ShadeMode);

   // Specify texture unit
   safe_glUniform1i(h_uTexUnit, 0);
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   
   // Set light uniforms
   glUniform3f(h_uLightColor, 0.4, 0.4, 0.38);
   glUniform4f(h_uLightVec, 0.0, 3.0, 4.0, 0.0);

   // Render depth info from light's perspective
   shadowMap->BindFBO();
   glClear(GL_DEPTH_BUFFER_BIT);
   SetEye(glm::vec3(origLookAt.x, origLookAt.y + 6.0, origLookAt.z + 8.0));
   SetLookAt(origLookAt);
   curView = SetShadowView();
   curProj = SetOrthoProjectionMatrix(10.0);
   glUniform3f(h_uCamPos, 0.0, 3.0, 4.0);
   glfwDraw(window);
   shadowMap->UnbindFBO();

   // Render scene normally and draw
   shadowMap->BindDepthTex();
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   SetEye(origEye);
   SetLookAt(origLookAt);
   curView = SetView();
   curProj = SetProjectionMatrix();
   glUniform3f(h_uCamPos, GetEye().x, GetEye().y, GetEye().z);
   glfwDraw(window);
   shadowMap->UnbindDepthTex();

   // Disable textures
   glDisable(GL_TEXTURE_2D);

   // Disable the shaders
   glUseProgram(0);	
   glfwSwapBuffers(window);
}

int main( int argc, char *argv[] )
{
   GLFWwindow *window;
   int Edit;
   //File name to load
   string fileName;
   //whether name inputtted is valid
   bool validFile = false;

   //Try to determine file input
   while(validFile == false) {
      printf("Type the file to load (Special options: 'd' = default, 'c' = clean):\n");
      scanf("%s", &fileName[0]);
      strcat(&fileName[0], ".wub");

      ifstream toLoad(&fileName[0]);
      validFile = toLoad.good();

      //If 'c' was entered, then load a clean level
      if(strcmp(&fileName[0], "c.wub") == 0) {
         printf("Loading clean level...\n");
         validFile = true;
      }
      //If 'd' was entered, then deafult level
      else if(strcmp(&fileName[0], "d.wub") == 0) {
         printf("Loading default level...\n");
         validFile = true;
      }
      else if(validFile == false){
         printf("Bad file, please type another file to load.\n");
      }
      else if(validFile == true) {
         toLoad.close();
      }
   }

   //Determine mode
   printf("Type 0 to play, any other int to edit\n");
   scanf("%i", &Edit);

   glfwSetErrorCallback(glfwError);
   if (!glfwInit()) {
      exit(EXIT_FAILURE);
   }

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
      setDistance(7);
      SetEdit(1);
      paused = false;

      glfwSetKeyCallback( window, glfwEditKeyPress);
      glfwSetCursorPosCallback( window, glfwEditGetCursorPos );
      glfwSetMouseButtonCallback( window, glfwEditMouse );
      glfwSetScrollCallback( window, glfwEditScroll );

      glewInit();
      glInitialize(window);
      physicsInit();
      InitGeom();
      initLevelLoader();
      loadLevel(fileName);
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
      setDistance(10);
      paused = false;

      glfwSetKeyCallback(window, glfwGameKeyPress);
      glfwSetCursorPosCallback( window, glfwGameGetCursorPos );
      glfwSetMouseButtonCallback( window, glfwPlayMouse );

      glewInit();
      glInitialize(window);
      physicsInit();
      InitGeom();
      initLevelLoader();
      loadLevel(fileName);

      //music
      SetBackground("../Assets/Sounds/cityMain.mp3");
   }
   LoadMesh("../Assets/Models/topHatChar.obj");
   ShadowMap *shadowMap = new ShadowMap();
   if (shadowMap->MakeShadowMap(g_width, g_height) == -1) {
      printf("SHADOW MAP FAILED\n");
      exit(EXIT_FAILURE);  
   }

   objectives.push_back(new Objective(-42.0, -379.0, 230.0, 67.0));
   objectives[objectives.size()-1]->Init();

   // Start the main execution loop.
   while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      if(Edit) {
         if(paused == false) {
            //Keep the cursor centered
            glfwSetCursorPos(window,g_width/2,g_height/2);  
            renderScene(window, shadowMap);
            glfwEditGetCursorPos(NULL,g_width/2.0,g_height/2.0);
            //glfw Game Keyboard
            glfwEditKeyboard();
         }
      }
      else {
         if(paused == false) {
            //player appy physics controls
            SetLookAt(glm::vec3(physGetPlayerX(),physGetPlayerY(),physGetPlayerZ()));
            SetSpeed(.05*magnitude(getPlayerSpeed()));
            //Keep the cursor centered
            glfwSetCursorPos(window,g_width/2,g_height/2);         
            physStep();
            for(int i=0;i<objectives.size();i++) objectives[i]->Update(glm::vec3(physGetPlayerX(),physGetPlayerY(),physGetPlayerZ()));
            //printf("oop%d\n",      getVecList().size());
            //Draw stuff
            renderScene(window, shadowMap);
            glfwGameGetCursorPos(NULL,g_width/2.0,g_height/2.0);
            //glfw Game Keyboard
            glfwGameKeyboard();
         }
      }
      usleep(15000);
   }

   // Clean up after GLFW.
   glfwDestroyWindow(window);
   glfwTerminate();
   exit(EXIT_SUCCESS);
}
