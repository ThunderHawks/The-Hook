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
#include "IOStartScreen.h"
#include "physSystem.h"
#include "Shapes.h"
#include "level.h"
#include <stdio.h>
#include <vector>
#include <string>
#include "octree.h"
#include "Camera.h"
#include "Shadows.h"
#include "SoundPlayer.h"
#include "Objective.h"
#include "camBox.h"
#include "Gui.h"
//#include "particle.h"

#include "types.h"
#include "Image.h"
#include "playerAnim.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

#define FAR_PLANE 300


//Window
GLFWwindow *window;
//Shadow Map and glow map
ShadowMap *shadowMap;
ShadowMap *glowMap, *glowBlurMap;
//Music player
SoundPlayer musicPlayer;
//Paused/unpause
bool paused;
//Mode is either -1, 0 or 1 for Start screen, game, or edit more respectively
int Mode = -1;
//position and color data handles
GLuint triBuffObj, colBuffObj;

int ShadeMode = 0;

//flag and ID to toggle on and off the shader
int ShadeProg;

//Handles to the shader data
GLint h_aPointSize, h_aPosition, h_aNormal, h_aUVVertex, h_uViewMatrix, h_uProjMatrix;
GLuint CubeBuffObj, CIndxBuffObj, GrndBuffObj, GIndxBuffObj, GNBuffObj, GNIndxBuffObj, SqIndxBuffObj, SqBuffObj, SqNormalObj, h_uTexSampler;
GLuint ShadowCubeBuffObj, SCIndxBuffObj, ShadowNormalBuffObj, RampBuffObj, RIndxBuffObj, RampNormalBuffObj;
int g_CiboLen, g_GiboLen, g_RiboLen, g_SCiboLen, g_SqiboLen;

float g_width, g_height;

GLint h_uLightVec;
GLint h_uLightColor;
GLint h_uCamPos, h_uShadeMode;
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine, h_uMatAlpha;
GLint h_uLightViewMatrix, h_uLightProjMatrix;
GLint h_uTexUnit, h_uTexUnit2;
GLint h_uTexCoord, h_aTexCoord, h_uGuiMode, h_uTextMode, h_utexpos, h_uMatMode;
GLuint TexBuffObj;

//declare Matrix stack
RenderingHelper ModelTrans;

GLint h_uModelMatrix;
GLuint NormalBuffObj;
GLuint MeshBuffObj, MeshIndxBuffObj;

//The assimp mesh stuff
Player player;
Mesh startNPC, flag, arrow;
int startFrames[]  = {1,  2, 19, 61, 74, 91, 101, 111, 121};
int animDuration[] = {1, 16, 41, 12, 16,  9,   9,   9,   9};

//animation stuff here
GLint h_uAnimFlag, h_uNumWeights, h_uBoneMatrix, h_uWeights, h_uJoints;

/*Do something better with this later. I don't like that it is a global but oh well for now.*/
glm::mat4 curProj, curView;

vector<Objective*> objectives;
std::list<part*> particleSpawner;

/*adding in a way to store model matricies for each building/unmoving object*/
vector<glm::mat4> allModelMats;
vector<Entity> allEnts;

//camera thing
int MAX_DISTANCE = 20;

/* projection matrix  - do not change */
glm::mat4 SetProjectionMatrix() {
   glm::mat4 Projection = glm::perspective(80.0f, (float)g_width/g_height, 0.1f, (float)FAR_PLANE);	
   safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
   return Projection;
}

glm::vec3 getCurentObjective(){
	for(int i = 0;i<objectives.size();i++)
	 if (objectives[i]->active)
		 return objectives[i]->end;
	return glm::vec3(0,0,0);
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
         PlaceModel(*tempEntities.at(i).mesh, tempEntities.at(i).position.x, tempEntities.at(i).position.y, tempEntities.at(i).position.z, tempEntities.at(i).scale.x, tempEntities.at(i).scale.y, tempEntities.at(i).scale.z, tempEntities.at(i).angle);
      }
   }
}
float sizer = 45;
float cool = 0;
void setCool(int i){

   cool = i;
}

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
   if(getDistance()>MAX_DISTANCE)  setDistance(MAX_DISTANCE);

}
//Draws the entities into the world
void drawEntities(int passNum, std::vector<Entity > *entities, std::vector <glm::mat4> *storedModels) {
	//int objects = 0;
	float sized;
	glm::mat4 modelMat;

   if(Mode == GAME_MODE) {
      cameraColision();
   }
   Entity entityTemp;
   srand(sizer);
   sized = sin(sizer);
   int hit = 0;
   //printf("num ent rend %d\n",getEntityNum());
   for(int i = 0; i < entities->size(); ++i) {
      entityTemp = (*entities)[i];
      modelMat = storedModels->at(i);
      if(entityTemp.meshIndex != 18 && Mode == GAME_MODE) {
         if (passNum == 2)
            SetMaterial(17);
         else {
            //int mat = rand()%13;
            //while(!(mat = rand()%13));
               SetMaterial(entityTemp.material);
         }
            
         if(!getGPressed('V')) {
         	if (cool)
         		PlaceModel(*entityTemp.mesh, entityTemp.position.x, entityTemp.position.y, entityTemp.position.z, entityTemp.scale.x*(sized*.3+1), entityTemp.scale.y*(sized*.3+1), entityTemp.scale.z*(sized*.3+1), entityTemp.angle+sized*3);
         	else
      			PlaceModel(*entityTemp.mesh, modelMat);
         	//++objects;		
         }
      }
      else if(Mode == EDIT_MODE) {
         if (passNum == 2)
            SetMaterial(17);
         else {
            //int mat = rand()%13;
            //while(!(mat = rand()%13));
               SetMaterial(entityTemp.material);
         }
         if(!getGPressed('V')) {
          	if (cool)
         		PlaceModel(*entityTemp.mesh, entityTemp.position.x, entityTemp.position.y, entityTemp.position.z, entityTemp.scale.x*(sized*.3+1), entityTemp.scale.y*(sized*.3+1), entityTemp.scale.z*(sized*.3+1), entityTemp.angle+sized*3);
         	else
      			PlaceModel(*entityTemp.mesh, modelMat);
         }
      }
   }

   if(getGPressed('B')) cool = 1;
   if(cool) sizer+=.06;
   else{
      cool = 0;
      sizer=0;

   }
   if(cool) cool--;
   
   //if(passNum == 2)
   	//printf("OBJECTS! %d\n", objects);
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

void drawGameElements(int passNum, std::vector<Entity > *entities, std::vector <glm::mat4> *storedModels) {
   //DRAW THE DANCING CYLINDER HERE!!
   btTransform pla;
   glm::vec3 ObjDir = getCurentObjective();
  
   /*These are for animation. They should be removed later*/
   static unsigned int ctr = 0;
   static unsigned int frm = 0;

   /*REMOVE LATER TODO*/
   /*if (passNum == 0)
      if (ctr++%3 == 0)
         frm++;*/

	player.Animate(START_RUN, startFrame[START_RUN], animDuration[START_RUN]);
   //PlaceModel(playerMesh, physGetPlayerX(), physGetPlayerY() - 1.3, physGetPlayerZ(), .25, .25, .25, -getYaw()*180/3.14 - 90, frm%120);
   //END OF DANCING CYLINDER CODE HERE!!
   
   /*Draw the arrow*/
   if (Mode == GAME_MODE && passNum != 0 && (ObjDir.x != 0 || ObjDir.y != 0 || ObjDir.z != 0)) {
   	
   	glm::vec3 gaze = glm::normalize(GetLookAt() - GetEye());
   	glm::vec3 ArrowLoc = GetEye() + 3.f*gaze + glm::vec3(0, 1, 0);

      glm::vec3 tmpa,tmpb;
      float angle, diffX, diffZ;
      tmpa = ArrowLoc;//GetEye();
      tmpb = getCurentObjective();

      diffZ = tmpb.z - tmpa.z;
      diffX = tmpb.x - tmpa.x;

      if (diffX == 0.0)
         angle = diffZ < 0 ? 90.0 : -90.0;
      else
         angle = -atan(diffZ/diffX) * 180.0 / 3.14159;

      if (diffZ <= 0.0 && diffX < 0.0) // Quadrant III wrt tmpa
         angle += 180.0;
      else if (diffZ > 0.0 && diffX < 0.0) // Quadrant IV wrt tmpa
         angle -= 180.0;
      angle -= 90.0;

      PlaceModel(arrow, ArrowLoc.x, ArrowLoc.y, ArrowLoc.z, .1, .1, .1, angle);
/*
		tmpa.y=0;
		tmpb.y=0;
		
		//printf("%f is angle %f %f\n",acos((tmpa.z-tmpb.z)/(tmpa-tmpb).length()),tmpa.z-tmpb.z,(tmpa-tmpb).length());	
		PlaceModel(arrow, ArrowLoc.x, ArrowLoc.y, ArrowLoc.z, .1, .1, .1 , acos((tmpa.z-tmpb.z)/(tmpa-tmpb).length())*180/3.14159);
*/
   }

   drawSelectedObjects();
   drawEntities(passNum, entities, storedModels);

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
   glColorMask(true, true, true, true);

   if (passNum == 1 || passNum == 3) {
   	float ang = 90 + -getYaw()*180/3.14;
      //draw objectives
      for(int i = 0; i < objectives.size();++i){
         if(objectives[i]->active){
            //PlaceModel(flag,objectives[i]->end.x, objectives[i]->end.y, objectives[i]->end.z, 50, 50, 50, 1);
            SetupCube(objectives[i]->end.x, objectives[i]->end.y, objectives[i]->end.z, 16, 60, 10, 5000, 10);
         }
         else{
            PlaceModel(startNPC, objectives[i]->start.x, objectives[i]->start.y, objectives[i]->start.z, .25, .25, .25, ang, frm%48);
            //PlaceModel(flag,objectives[i]->start.x, objectives[i]->start.y, objectives[i]->start.z, 50, 50, 50, 1);
            SetupCube(objectives[i]->start.x, objectives[i]->start.y, objectives[i]->start.z, 15, 60, 10, 5000, 10);
         }
      }
   }
   if (passNum == 1) {
      //draw particles
      for (std::list<part*>::iterator it=particleSpawner.begin(); it != particleSpawner.end(); it++){
         drawPart(*it);
//         particleSpawner[i]->drawPart();
      }

      if(particleSpawner.size() && particleSpawner.front() && particleSpawner.front()->age>60){
         destroyDustPart( particleSpawner.front());
         particleSpawner.erase(particleSpawner.begin());
      }

   }
   glEnable(GL_CULL_FACE);
}

/* Main display function 
 * passNum: 0 = Create shadow map
 *          1 = Draw scene normally
 *          2 = Draw outlines around objects
 *          3 = Create glow map for bloom effect
 */
void glfwDraw (GLFWwindow *window, int passNum, std::vector<Entity > *entities, std::vector <glm::mat4> *storedModels)
{   
   if (passNum == 3)
      glColorMask(false, false, false, false);

   if (passNum != 2) {
      //Draw skybox
   	//DrawSkyBox();
   	SetModelStat();

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
   else if (passNum == 2) {
      // Enable front face culling for object outlines
      glCullFace(GL_FRONT);
      glEnable(GL_CULL_FACE);

      SetMaterial(17);
   }
   drawGameElements(passNum, entities, storedModels);
   glColorMask(true, true, true, true);
}

void renderScene() {
   glm::vec3 origEye = GetEye();
   glm::vec3 origLookAt = GetLookAt();
   
   std::vector<Entity> entities, all;
   glm::mat4 holdMat;
	vector<glm::mat4> storedModels;
   
   glm::vec3 ggaze;
   glm::vec3 gw;
  	glm::vec3 gu;
   glUseProgram(ShadeProg);
   
   float largestRadius = 0;
   Entity largestEntity;
   int i;


	curView = SetView();
   curProj = SetProjectionMatrix();
   ggaze = GetLookAt() - GetEye();
	gw = ggaze/magnitude(ggaze);
	gw = glm::vec3(-1.0 * gw.x, -1.0 * gw.y, -1.0 * gw.z);
	gu = glm::cross(GetUp(), gw)/magnitude(glm::cross(GetUp(), gw));
	  	
   //all = GetNearby(FAR_PLANE);
   /*TODO change to an iterator*/
   if (Mode == GAME_MODE) {
   	for (i = allEnts.size() - 1; i > 0; i--) {
			Entity entityTemp = allEnts[i];

			holdMat = allModelMats[i];
		
		
			if ( checkViewFrustum(glm::vec3 (0,0,0), entityTemp.BSRadius, curProj*curView*holdMat) == 0) {
				if (largestRadius < entityTemp.BSRadius) {
					largestRadius = entityTemp.BSRadius;
					largestEntity = entityTemp;
				}
				entities.push_back(entityTemp);
				storedModels.push_back(holdMat);
			}
			
		}
   } else {
		for (i = getEntityNum() - 1; i > 0; i--) {
			Entity entityTemp = getEntityAt(i);
			

			holdMat = GetModel(entityTemp.position.x, entityTemp.position.y, entityTemp.position.z, entityTemp.scale.x, entityTemp.scale.y, entityTemp.scale.z, entityTemp.angle);
		
		
			if ( checkViewFrustum(glm::vec3 (0,0,0), entityTemp.BSRadius, curProj*curView*holdMat) == 0) {
				if (largestRadius < entityTemp.BSRadius) {
					largestRadius = entityTemp.BSRadius;
					largestEntity = entityTemp;
				}
				entities.push_back(entityTemp);
				storedModels.push_back(holdMat);
			}
			
		}
   }
   //entities = all; Change the entities 5 lines up to all if you want.
   
   
   /*for (int i = 0; i < all.size(); i++) {
   	Entity entityTemp = all[i];
   	holdMat = GetModel(entityTemp.position.x, entityTemp.position.y, entityTemp.position.z, entityTemp.scale.x, entityTemp.scale.y, entityTemp.scale.z, entityTemp.angle);
   	if (checkViewFrustum(glm::vec3 (0,0,0), -entityTemp.BSRadius, curProj*GetView(largestEntity.position)*holdMat) != 0)
   		entities.push_back(entityTemp);
   }*/


   glUniform1i(h_uShadeMode, ShadeMode);

   // Specify texture unit
   safe_glUniform1i(h_uTexUnit, 0);
   glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE0);
   shadowMap->BindDepthTex();

   // Render depth info from light's perspective
	bool isLock = isLocked();
	camlock(false);
	glm::vec3 tempEye = GetEye();
	glm::vec3 tempLook = GetLookAt();
	float tempDist = getDistance();

   shadowMap->BindDrawFBO();
   glClear(GL_DEPTH_BUFFER_BIT);
   SetEye(glm::vec3(origLookAt.x, origLookAt.y + 6.0, origLookAt.z + 8.0));
   SetLookAt(origLookAt);
   curView = SetShadowView();
   curProj = SetOrthoProjectionMatrix(GetEye(), GetLookAt(), 10.0);
   glUniform3f(h_uCamPos, 0.0, 3.0, 4.0);
   glfwDraw(window, 0, &entities, &storedModels);
   shadowMap->UnbindDrawFBO(g_width, g_height);
   
   SetLookAt(tempLook);
	SetEye(tempEye);   
	setDistance(tempDist);

   // Set the eye and look at point to their original locations
   resetVecs();
	camlock(isLock);

   if (Mode != GAME_MODE) {
   	SetLookAt(origLookAt);
   } else {
		SetLookAt(origLookAt + vec3(0, 2, 0) + 2.f*vec3(gu.x, 0, gu.z));
   }
   curView = SetView();
   curProj = SetProjectionMatrix();
   glUniform3f(h_uCamPos, GetEye().x, GetEye().y, GetEye().z);

   // Render glow map
   if (ShadeMode == 1) {
      glowMap->BindDrawFBO();
      GLenum renderTargets[] = {GL_COLOR_ATTACHMENT0, GL_DEPTH_ATTACHMENT};
      glDrawBuffers(2, renderTargets);
      glUniform1f(h_uTextMode, 2);
      glClearColor(1.0, 1.0, 1.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      glfwDraw(window, 3, &entities, &storedModels);
      glowMap->UnbindDrawFBO(g_width, g_height);
      glUniform1f(h_uTextMode, 0);
   }

   // Render scene normally and draw
   glClearColor(0.7f, 0.8f, 0.9f, 1.0f);
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glfwDraw(window, 1, &entities, &storedModels);
   shadowMap->UnbindTex();
   glowMap->UnbindTex();

   // Draw outlines
   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
   glfwDraw(window, 2, &entities, &storedModels);
   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

   // Blur the glow map
   if (ShadeMode == 1) {
      glowMap->BindReadFBO();
      glowBlurMap->BindDrawFBO();
      glClearColor(1.0, 1.0, 1.0, 0.0);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      ready2D();
      glUniform1f(h_uGuiMode, 0);
      glUniform1f(h_uTextMode, 3);
      SetupSq(0.0, 0.0, glowMap->GetColorTex(), 2.0, 2.0);
      glowMap->UnbindReadFBO(g_width, g_height);
      glowBlurMap->UnbindDrawFBO(g_width, g_height);

      // Draw glowmap over scene
      glUniform1f(h_uTextMode, 5);
      SetupSq(0.0, 0.0, glowBlurMap->GetColorTex(), 2.0, 2.0);
      ready3D();
      glUniform1f(h_uTextMode, 0);
      glClearColor(0.7f, 0.8f, 0.9f, 1.0f);
   }

   //Draw any gui elements that should be on the screen
   DrawGui(Mode);

   // Disable textures
   glDisable(GL_TEXTURE_2D);

   // Disable the shaders
   glUseProgram(0);	
   glfwSwapBuffers(window);
}

void toStartScreen() {
	if (!musicPlayer.BGM.isPaused())
		musicPlayer.muteBGM();
   Mode = STARTSCREEN_MODE;
   glfwSetMouseButtonCallback( window, glfwStartScreenMouse );
   glfwSetCursorPosCallback( window, glfwStartScreenGetCursorPos );
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
   freeLevelData();

	allModelMats.clear();
	allEnts.clear();   
}

void initStartScreen() {
   Mode = STARTSCREEN_MODE;
   glfwSetErrorCallback(glfwError);
   if (!glfwInit()) {
      exit(EXIT_FAILURE);
   }

   window = glfwCreateWindow(800, 800, "Grapple", NULL, NULL);
   if(!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
   }

   glfwMakeContextCurrent(window);
   glfwSetWindowPos(window, 80, 80);
   glfwSetWindowSizeCallback(window, glfwWindowResize);
   glfwSetWindowSize(window,1600,800);
   g_height =800;
   g_width = 1600;
   //Set Mouse callbacks for start screen
   glewInit();
   glInitialize(window);
   InitGeom();
   initGui();

   glfwSetMouseButtonCallback( window, glfwStartScreenMouse );
   glfwSetCursorPosCallback( window, glfwStartScreenGetCursorPos );

   shadowMap = new ShadowMap();
   glowMap = new ShadowMap();
   glowBlurMap = new ShadowMap();
   if (shadowMap->MakeShadowMap(g_width, g_height) == -1) {
      //printf("SHADOW MAP FAILED\n");
      exit(EXIT_FAILURE);  
   }
   if (glowMap->MakeGlowMap(g_width, g_height) == -1) {
      //printf("GLOW MAP FAILED\n");
      exit(EXIT_FAILURE); 
   }
   if (glowBlurMap->MakeGlowMap(g_width, g_height) == -1) {
      //printf("GLOW BLUR MAP FAILED\n");
      exit(EXIT_FAILURE); 
   }

   // Enable transparency
   glEnable(GL_BLEND);
   glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

   // Set light uniforms
   glUniform3f(h_uLightColor, 0.4, 0.4, 0.38);
   glUniform4f(h_uLightVec, 0.0, 3.0, 4.0, 0.0);

   // Set line width for object outlines
   glLineWidth(3.0);
}

void initPlay(string fileName) {
   static bool musicStarted = false;

   Mode = GAME_MODE;
   srand(time(0));
   SetEdit(Mode);
   paused = false;
   player = Player();
   startNPC = LoadMesh("../Assets/Models/npcJumping.dae");
   arrow = LoadMesh("../Assets/Models/arrow.obj");

   //music
   if (!musicStarted) {
      musicPlayer = SoundPlayer();
      musicPlayer.CreatePlayList("./playList.txt");
      musicStarted = true;
   } else {
   	musicPlayer.muteBGM();
   }

   glfwSetKeyCallback(window, glfwGameKeyPress);
   glfwSetCursorPosCallback( window, glfwGameGetCursorPos );
   glfwSetMouseButtonCallback( window, glfwGameMouse );
   glfwSetScrollCallback( window, glfwGameScroll );
   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
   physicsInit();
   initLevelLoader(Mode);
   loadLevel(fileName);
   
   /*liquid efficiency poured here*/
   //all = GetNearby(FAR_PLANE);
   for (int i = getEntityNum() - 1; i > 0; i--) {
   	Entity entityTemp = getEntityAt(i);
		allModelMats.push_back(GetModel(entityTemp.position.x, entityTemp.position.y, entityTemp.position.z, entityTemp.scale.x, entityTemp.scale.y, entityTemp.scale.z, entityTemp.angle) );
		allEnts.push_back(entityTemp);
   }
	
   Objective *tObj;
   while (objectives.size() < 4) {
      tObj = new Objective(-42.0, -379.0, 230.0, 67.0);
      objectives.push_back(tObj);
      tObj->Init();
   }
}

void initEdit(string fileName) {
   setDistance(7);
   SetEdit(Mode);
   paused = false;

   glfwSetKeyCallback( window, glfwEditKeyPress);
   glfwSetCursorPosCallback( window, glfwEditGetCursorPos );
   glfwSetMouseButtonCallback( window, glfwEditMouse );
   glfwSetScrollCallback( window, glfwEditScroll );

   Mode = EDIT_MODE;
   physicsInit();
   initLevelLoader(Mode);
   loadLevel(fileName);
}

float getFPS() {
   static float prev = 0.0;
   static float curr;
   float diff;
   char title[30];
   float cap = 60.0;

   curr = glfwGetTime();
   diff = curr - prev;
   prev = curr;

	if (1.0/diff > cap) {
      usleep((1.0/cap - diff)*1000000);
      diff = 1.0/cap; 
   }

   sprintf(title, "Grapple --- FPS: %d", int(1.0/diff));
   glfwSetWindowTitle(window, title); 
   
   return 1.0/diff;
}

float camTime = 0;
float camH = 0;

int main( int argc, char *argv[] )
{
	float t;
   initStartScreen();

   printf("Starting main loop\n");
   // Start the main execution loop.
   glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);//andrew tag here!

   while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      t=getFPS();

      if(Mode == STARTSCREEN_MODE) {
         glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
         glUseProgram(ShadeProg);
         DrawGui(Mode);
       	 glUseProgram(0);	
         glfwSwapBuffers(window);
         camTime = 0;
      }
      else if(Mode == GAME_MODE){
         if(paused == false) {
				if(camTime<7){
					camlock(false);
					camTime+=1/t;
					SetLookAt(glm::vec3(0,20,0));
					incrementYaw(1.2/t);
					setPitch(.5);
					setDistance(25+50*(9-camTime));
					resetVecs();
					camlock(true);
				}
				else if(camTime<9){
					camlock(false);
					camTime+=1/t;
					camH+=1/t;
					SetLookAt(glm::vec3(0,25-camH*11.75,0));
					incrementYaw(1/t);
					setPitch(.5);
					setDistance(20+50*(9-camTime));
					resetVecs();
					camlock(true);
				}
				else{
					camlock(false);
				}
            //player appy physics controls
            SetLookAt(glm::vec3(physGetPlayerX(),physGetPlayerY(),physGetPlayerZ()));
            SetSpeed(.05*magnitude(getPlayerSpeed()));
            //Keep the cursor centered
            glfwSetCursorPos(window,g_width/2,g_height/2);         
            physStep(t);
            for(int i=0;i<objectives.size();++i) objectives[i]->Update(glm::vec3(physGetPlayerX(),physGetPlayerY(),physGetPlayerZ()));
            //printf("oop%d\n",      getVecList().size());
            //Draw stuff
            renderScene();
            glfwGameGetCursorPos(NULL,g_width/2.0,g_height/2.0);
            //glfw Game Keyboard
            glfwGameKeyboard();
         }
      }
      else if(Mode == EDIT_MODE) {
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
            renderScene();
            //glfw Game Keyboard
            glfwEditKeyboard();
         }
      }

      //usleep(15000);
   }

   // Clean up after GLFW.
   glfwDestroyWindow(window);
   glfwTerminate();
   exit(EXIT_SUCCESS);
}


