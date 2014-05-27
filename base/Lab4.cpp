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
#include "IOGame.h"
#include "IOEdit.h"
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
#include "camBox.h"
#include "Gui.h"
//#include "particle.h"

#include "types.h"
#include "Image.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>


SoundPlayer musicPlayer;
//Paused/unpause
bool paused;
   int Edit;
//position and color data handles
GLuint triBuffObj, colBuffObj;

int ShadeMode = 0;

//flag and ID to toggle on and off the shader
int ShadeProg;

//Handles to the shader data
GLint h_aPointSize, h_aPosition, h_aNormal, h_uViewMatrix, h_uProjMatrix;
GLuint CubeBuffObj, CIndxBuffObj, GrndBuffObj, GIndxBuffObj, GNBuffObj, GNIndxBuffObj, SqIndxBuffObj, SqBuffObj, SqNormalObj;
GLuint ShadowCubeBuffObj, SCIndxBuffObj, ShadowNormalBuffObj, RampBuffObj, RIndxBuffObj, RampNormalBuffObj;
int g_CiboLen, g_GiboLen, g_RiboLen, g_SCiboLen, g_SqiboLen;

float g_width, g_height;

GLint h_uLightVec;
GLint h_uLightColor;
GLint h_uCamPos, h_uShadeMode;
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine, h_uMatAlpha;
GLint h_uLightViewMatrix, h_uLightProjMatrix;
GLint h_uTexUnit, h_uTexUnit2;
GLint h_uTexCoord, h_aTexCoord, h_uGuiMode, h_uTextMode;
GLuint TexBuffObj;

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
std::list<part*> particleSpawner;

/* projection matrix  - do not change */
glm::mat4 SetProjectionMatrix() {
   glm::mat4 Projection = glm::perspective(80.0f, (float)g_width/g_height, 0.1f, 400.f);	
   safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
   return Projection;
}

//Draws the currently selected entity
void drawSelectedObjects() {

   updateCurrentEntitiesPos();
   //Only draw if entitie(s) are selected
   if(areEntitiesSelected() == true) {
      //Get selected entities
      vector<Entity> tempEntities = getSelectedEntities();

      //Place each selected objects
      for(int i = 0; i < tempEntities.size(); ++i) {
         PlaceModel(*tempEntities.at(i).mesh, tempEntities.at(i).position.x, tempEntities.at(i).position.y, tempEntities.at(i).position.z, tempEntities.at(i).scale.x, tempEntities.at(i).scale.y, tempEntities.at(i).scale.z, tempEntities.at(i).angle, tempEntities.at(i).BSRadius);
      }
   }
}
float sizer = 45;
float cool = 0;

void cameraColision(){
	//get all the possible things the eye can collide with using the point level test
   vector<Entity*> posible = pointLevelTest(GetEye());
   int hit = 0;
   glm::vec3 tempEye = GetEye();
   
   //check to see if it collides with anything
   for(int i =0;!hit && i<posible.size();++i){
      camBox* boxCast = (camBox*)posible[i]->physics;
      if ( pointBox(tempEye,(camBox*)posible[i]->physics)){//test for camera collision with entities
         hit = 1;
      }
   }
   
   //if it collided with something, then move the distance forward, if not then move it back
   if(hit)
   	addDistance(-.15);
   else 
   	addDistance(.15);
   	
   //get the new location of the eye
   tempEye = GetEye();
   posible = pointLevelTest(tempEye);
   hit = 0;
   
   //check to see if the new location is colliding
   for(int i =0;!hit && i<posible.size();++i){
      camBox* boxCast = (camBox*)posible[i]->physics;
      if(pointBox(tempEye,(camBox*)posible[i]->physics)){//test for camera collision with entities
         hit = 1;
      }
   }
   
   //if it is colliding in the new location, then move it forward more
   if(hit) 
   	addDistance(-.15);
   	
   //keep the eye within its bounds
   if(getDistance()<2)  setDistance(2);
   if(getDistance()>20)  setDistance(20);

}
//Draws the entities into the world
void drawEntities(int passNum) {
   if(!Edit) cameraColision();
   Entity entityTemp;
   srand(sizer);
   int hit = 0;
   //printf("num ent rend %d\n",getEntityNum());
   for(int i = 0; i < getEntityNum(); ++i) {
      entityTemp = getEntityAt(i);
      
      if (passNum == 2)
         SetMaterial(17);
      else {
         int mat = rand()%13;
         while(!(mat = rand()%13));
         SetMaterial(mat);
      }
      if(!getGPressed('V')) PlaceModel(*entityTemp.mesh, entityTemp.position.x, entityTemp.position.y, entityTemp.position.z,
         entityTemp.scale.x*(sin(sizer)*.3+1), entityTemp.scale.y*(sin(sizer)*.3+1), entityTemp.scale.z*(sin(sizer)*.3+1), entityTemp.angle+sin(sizer)*10, entityTemp.BSRadius);
   }

   if(getGPressed('B')) cool = 1;
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

/* Main display function 
 * passNum: 0 = Create shadow map
 *          1 = Draw scene normally
 *          2 = Draw outlines around objects
 */
void glfwDraw (GLFWwindow *window, int passNum)
{
   glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);//andrew tag here!
   if (passNum != 2) {
      //Draw skybox
   	DrawSkyBox();
   	SetModelStat();

      //printText2D("Hello World!", 0, 0, 10);

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

      SetMaterial(2);
   }
   else {
      // Enable front face culling for object outlines
      glCullFace(GL_FRONT);
      glEnable(GL_CULL_FACE);

      SetMaterial(17);
   }

   //DRAW THE DANCING CYLINDER HERE!!
   btTransform pla;
   PlaceModel(playerMesh, physGetPlayerX(), physGetPlayerY(), physGetPlayerZ(), .25, .25, .25, 0, 1.7);
   //END OF DANCING CYLINDER CODE HERE!!

   drawSelectedObjects();
   drawEntities(passNum);

   // Disable backface culling for grapple
   glCullFace(GL_BACK);
   glDisable(GL_CULL_FACE);

   //Draw Cubes, ??????????
   if(isGrappleActive()){
      SetupCube(plsRndr().getX(),plsRndr().getY(),plsRndr().getZ(),5,60,1,1,1);
      for(float i=.05;i<1;i+=.0075){
         srand(physGetPlayerX());
         float y;
         if(physGetPlayerY()>grapplingHookLocation().y){
            y = physGetPlayerY()*(1-i)+grapplingHookLocation().y*(i);
         }
         else{
            y = physGetPlayerY()*(1-i*i)+grapplingHookLocation().y*i*i;
         }
         float x = physGetPlayerX()*(1-i)+grapplingHookLocation().x*i;
         float z = physGetPlayerZ()*(1-i)+grapplingHookLocation().z*i;
         SetupCube(x,y,z,5,rand()/300.0,.15,.15,.15);
      }
   }

   if (passNum != 2)
      SetMaterial(2);
   else {
      SetMaterial(17);
      glCullFace(GL_FRONT);
      glEnable(GL_CULL_FACE);
   }

   srand(0);

   if (passNum == 1) {
      //draw objectives
      for(int i = 0; i < objectives.size();++i){
         if(objectives[i]->active){
            PlaceModel(flag,objectives[i]->end.x, objectives[i]->end.y, objectives[i]->end.z, 50, 50, 50, 1, 1.7);
            SetupCube(objectives[i]->end.x, objectives[i]->end.y, objectives[i]->end.z, 16, 60, 10, 5000, 10);
         }
         else{
            PlaceModel(flag,objectives[i]->start.x, objectives[i]->start.y, objectives[i]->start.z, 50, 50, 50, 1, 1.7);
            SetupCube(objectives[i]->start.x, objectives[i]->start.y, objectives[i]->start.z, 15, 60, 10, 5000, 10);
         }
      }
      //draw particles
      for (std::list<part*>::iterator it=particleSpawner.begin(); it != particleSpawner.end(); it++){
         drawPart(*it);
//         particleSpawner[i]->drawPart();
      }
      printf("ya\n");
      if(particleSpawner.front() && particleSpawner.front()->age>60){
         destroyDustPart( particleSpawner.front());
         particleSpawner.erase(particleSpawner.begin());
      }
      printf("no\n");
   }

   glDisable(GL_CULL_FACE);
}

void renderScene(GLFWwindow *window, ShadowMap *shadowMap) {
   glm::vec3 origEye = GetEye();
   glm::vec3 origLookAt = GetLookAt();
   
   glm::vec3 ggaze;
   glm::vec3 gw;
  	glm::vec3 gu;
   
   glUseProgram(ShadeProg);

   glUniform1i(h_uShadeMode, ShadeMode);

   //Enable transparency
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
   glfwDraw(window, 0);
   shadowMap->UnbindFBO();

   // Render scene normally and draw
   shadowMap->BindDepthTex();
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   SetEye(origEye);
   
   //move the character to the left when not in edit mode
   if (Edit) {
   	SetLookAt(origLookAt);
   } else {
   	ggaze = GetLookAt() - GetEye();
		gw = ggaze/magnitude(ggaze);
	  	gw = glm::vec3(-1.0 * gw.x, -1.0 * gw.y, -1.0 * gw.z);
	  	gu = glm::cross(GetUp(), gw)/magnitude(glm::cross(GetUp(), gw));
		SetLookAt(origLookAt + vec3(0, 2, 0) + 2.f*vec3(gu.x, 0, gu.z));
   }
   curView = SetView();
   curProj = SetProjectionMatrix();
   glUniform3f(h_uCamPos, GetEye().x, GetEye().y, GetEye().z);
   glfwDraw(window, 1);
   shadowMap->UnbindDepthTex();

   // Draw outlines
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glLineWidth(3.0);
   glfwDraw(window, 2);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   //Draw any gui elements that should be on the screen
   DrawGui(Edit);

   // Disable textures
   glDisable(GL_TEXTURE_2D);

   // Disable the shaders
   glUseProgram(0);	
   glfwSwapBuffers(window);
}

int main( int argc, char *argv[] )
{
   GLFWwindow *window;
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
      initGui(Edit);
      initLevelLoader(Edit);
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
      initGui(Edit);
      initLevelLoader(Edit);
      printf("Reading lv\n");
      loadLevel(fileName);

      //music
      musicPlayer = SoundPlayer();
		musicPlayer.CreatePlayList("./playList.txt");
   }
   playerMesh = LoadMesh("../Assets/Models/topHatChar.obj");
   ShadowMap *shadowMap = new ShadowMap();
   if (shadowMap->MakeShadowMap(g_width, g_height) == -1) {
      printf("SHADOW MAP FAILED\n");
      exit(EXIT_FAILURE);  
   }
   Objective* tObj = new Objective(-42.0, -379.0, 230.0, 67.0);
   objectives.push_back(tObj);
   tObj->Init();
   tObj = new Objective(-42.0, -379.0, 230.0, 67.0);
   objectives.push_back(tObj);
   tObj->Init();
   tObj = new Objective(-42.0, -379.0, 230.0, 67.0);
   objectives.push_back(tObj);
   tObj->Init();
   tObj = new Objective(-42.0, -379.0, 230.0, 67.0);
   objectives.push_back(tObj);
   tObj->Init();

   printf("Starting main loop\n");
   // Start the main execution loop.
   while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      if(Edit) {
         if(paused == false) {
            //Keep the cursor centered if gui is not displayed
            if(getEPressed('G') == 0) {
               glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
               glfwSetCursorPos(window,g_width/2,g_height/2);
               glfwEditGetCursorPos(NULL,g_width/2.0,g_height/2.0);
            }  
            else {
               glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            }
            renderScene(window, shadowMap);
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
            for(int i=0;i<objectives.size();++i) objectives[i]->Update(glm::vec3(physGetPlayerX(),physGetPlayerY(),physGetPlayerZ()));
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


