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
#include "InitHelpers.h"
#include "Helper.h"
#include "IO.h"
#include "Shapes.h"

#include <bullet/btBulletDynamicsCommon.h>
#include <bullet/BulletCollision/Gimpact/btGImpactCollisionAlgorithm.h>

void HideMesh();

//position and color data handles
GLuint triBuffObj, colBuffObj;

int ShadeMode = 0;

//flag and ID to toggle on and off the shader
int shade = 1;
int ShadeProg;

//phys//
btRigidBody* groundRigidBody;
btRigidBody* fallRigidBody;
btRigidBody* fallRigidBodyb;
btDiscreteDynamicsWorld* dynamicsWorld;

//Handles to the shader data
GLint h_aPosition, h_aNormal, h_uViewMatrix, h_uProjMatrix;
GLuint CubeBuffObj, CIndxBuffObj, GrndBuffObj, GIndxBuffObj, GNBuffObj, GNIndxBuffObj;
GLuint ShadowCubeBuffObj, SCIndxBuffObj, ShadowNormalBuffObj, RampBuffObj, RIndxBuffObj, RampNormalBuffObj;
int g_CiboLen, g_GiboLen, g_RiboLen, g_SCiboLen;

float g_width, g_height;
float g_Camtrans = -3.0;
float g_Mtrans = 0;
GLint h_uLightVec;
GLint h_uLightColor;
GLint h_uCamPos, h_uShadeMode;
GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;

//declare Matrix stack
RenderingHelper ModelTrans;

/**ALL JUNK FOR MESH LOGIC**/
static glm::vec3 *faceNormals;
static float *MeshPos;
static unsigned short *idx;
int idxSize;
int MeshPosSize;
float *VertexNormal;
float g_sf = 1.7;
glm::vec3 g_axis(0.000000001, 0.000000001, 0.000000001);
glm::mat4 allRot(1.0);
glm::vec3 g_trans(0.0, 0.8, 0.0);
int g_track;
float g_angle = 180;
int g_MiboLen;
//CMesh *TheMesh;
GLint h_uModelMatrix;
GLuint NormalBuffObj;
GLuint MeshBuffObj, MeshIndxBuffObj;

/* projection matrix  - do not change */
void SetProjectionMatrix() {
   glm::mat4 Projection = glm::perspective(80.0f, (float)g_width/g_height, 0.1f, 100.f);	
   safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
}

/* camera controls - do not change */
void SetView() {
   glm::mat4 view = glm::lookAt(eye, lookAtPoint, up);
   //glm::mat4 view = glm::lookAt(lookAtPoint, eye, up);
   safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(view));
}

void SetModelStat() {
   safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ModelTrans.modelViewMatrix));
}

/* model transforms */
void SetModel(float x, float y, float z, float Sx, float Sy, float Sz, float angle) {
   glm::mat4 Trans = glm::translate( glm::mat4(1.0f), glm::vec3(x, y, z));
   glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(Sx, Sy, Sz));
   //printf("%f %f ModelPosition\n",Sy,Sz);
   glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
   glm::mat4 ctm = Trans * Rotate * Scale;
   safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ctm));
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
   DrawShadow(x, z + 0.6, scaleX, scaleY, scaleZ + 0.4, angle);
}

/* Main display function */
void glfwDraw (GLFWwindow *window)
{
   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   /*Start our shader      */
   glUseProgram(ShadeProg);

   SetProjectionMatrix();
   SetView();

   glUniform3f(h_uLightColor, 0.4, 0.4, 0.38);
   glUniform4f(h_uLightVec, 0.0, -1.0, 1.0, 0.0);
   glUniform3f(h_uCamPos, eye.x, eye.y, eye.z);

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
   SetupCube(lookAtPoint.x, lookAtPoint.y, lookAtPoint.z, 6, 0, 0.2, 0.2, 0.2);
   //END OF DANCING CYLINDER CODE HERE!!

   //Draw Cubes
   SetupCube(3,0,6,2,45,1,1,2);
   SetupCube(3,1,6,2,45,1,.5,1);
   SetupCube(3,2,6,2,45,2,1,1);
   SetupCube(3,3,6,2,60,1,1,1);

   ///render spherse
   btTransform trans;
   fallRigidBody->getMotionState()->getWorldTransform(trans);
   std::cout << "sphere height: " << trans.getOrigin().getY() << " sphere x:"<<trans.getOrigin().getX()<< " sphere z:"<< trans.getOrigin().getZ() <<std::endl;
   SetupCube(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ(),2,60,2,2,2);
   fallRigidBodyb->getMotionState()->getWorldTransform(trans);
   SetupCube(trans.getOrigin().getX(),trans.getOrigin().getY(),trans.getOrigin().getZ(),2,60,2,2,2);

   //Disable the shader
   glUseProgram(0);	
   glfwSwapBuffers(window);

}

void physicsInit() {
   btBroadphaseInterface* broadphase = new btDbvtBroadphase();
   btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
   btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
   btGImpactCollisionAlgorithm::registerAlgorithm(dispatcher);
   btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;
   /*btDiscreteDynamicsWorld* */dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);
   dynamicsWorld->setGravity(btVector3(0,-10,0));

   //shapes
   btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(0,1,0),1);//1m up (y=1)
   btCollisionShape* fallShape = new btSphereShape(1);
   btCollisionShape* fallShapeb = new btSphereShape(1);

   //ground   
   btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,-1,0)));//-1m (y=1-1=0)

   btRigidBody::btRigidBodyConstructionInfo
   groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));//zeros give infinite mass
   /*btRigidBody**/ groundRigidBody = new btRigidBody(groundRigidBodyCI);

   dynamicsWorld->addRigidBody(groundRigidBody);
   
   //sphere
   btDefaultMotionState* fallMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,50,0)));//50m up
   btScalar mass = 1;
   btVector3 fallInertia(0,0,0);//inital velocity?
   fallShape->calculateLocalInertia(mass,fallInertia);//i duknow

   btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape,fallInertia);
/*   btRigidBody**/ fallRigidBody = new btRigidBody(fallRigidBodyCI);

   dynamicsWorld->addRigidBody(fallRigidBody);

   //second
   btDefaultMotionState* fallMotionStateb = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(.5,55,0)));//50m up
   btScalar massb = 1;
   btVector3 fallInertiab(0,0,0);//inital velocity?
   fallShapeb->calculateLocalInertia(massb,fallInertiab);//i duknow

   btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCIb(massb,fallMotionStateb,fallShapeb,fallInertiab);
  /* btRigidBody**/ fallRigidBodyb = new btRigidBody(fallRigidBodyCIb);

   dynamicsWorld->addRigidBody(fallRigidBodyb);
}


int main( int argc, char *argv[] )
{
   GLFWwindow *window;

   glfwSetErrorCallback(glfwError);
   if (!glfwInit()) {
      exit(EXIT_FAILURE);
   }

   // Set up the main window.
   window = glfwCreateWindow(800, 800, "The Creation", NULL, NULL);
   if (!window) {
      glfwTerminate();
      exit(EXIT_FAILURE);
   }

   srand(time(0));

   glfwMakeContextCurrent(window);
   glfwSetWindowPos(window, 80, 80);
   glfwSetWindowSizeCallback(window, glfwWindowResize);
   glfwSetWindowSize(window,800,800);
   g_height = g_width = 800;
   glfwSetKeyCallback(window, glfwKeyPress);
   glfwSetCursorPosCallback( window, glfwGetCursorPos );

   glewInit();
   glInitialize(window);
   physicsInit();

   InitGeom();

   // Start the main execution loop.
   while (!glfwWindowShouldClose(window)) {
      glfwPollEvents();
      //player appy physics controls
      dynamicsWorld->stepSimulation(1/60.f,10);
      //Draw stuff
      glfwDraw(window);
      //Keep the cursor centered
      glfwSetCursorPos(window,g_width/2,g_height/2);
      //Allows movement based on the keyboard callback
      glfwKeyboard();
      usleep(15000);
   }

   // Clean up after GLFW.
   glfwDestroyWindow(window);
   glfwTerminate();
   exit(EXIT_SUCCESS);
}
