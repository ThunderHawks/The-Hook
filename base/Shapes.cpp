
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

#include "Shapes.h"
#include "Camera.h"
#include "Gui.h"

float g_groundSize = 10000.0, g_groundY = 0;   // half the ground length, the y of the ground, 

GLuint skytex;

void initGround() {

  // A x-z plane at y = g_groundY of dimension [-g_groundSize, g_groundSize]^2
    float GrndPos[] = {
    -g_groundSize, g_groundY, -g_groundSize,
    -g_groundSize, g_groundY,  g_groundSize,
     g_groundSize, g_groundY,  g_groundSize,
     g_groundSize, g_groundY, -g_groundSize
    };

    float GrndNorm[] = {
     0, 1, 0,
     0, 1, 0,
     0, 1, 0,
     0, 1, 0,
     0, 1, 0,
     0, 1, 0
    };

    unsigned short idx[] = {0, 1, 2, 0, 2, 3};

    g_GiboLen = 6;
    glGenBuffers(1, &GrndBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, GrndBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndPos), GrndPos, GL_STATIC_DRAW);

    glGenBuffers(1, &GIndxBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, GIndxBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    glGenBuffers(1, &GNBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, GNBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GrndNorm), GrndNorm, GL_STATIC_DRAW);

}

void initCube() {

  float CubePos[] = {
    //Back face, 4 verts
    -0.5, -0.5, -0.5, //Point0
    -0.5, 0.5, -0.5,  //Point1
    0.5, 0.5, -0.5,   //Point2
    0.5, -0.5, -0.5,  //Point3

    //Right face, 4 verts
    0.5, 0.5, -0.5,   //P4
    0.5, 0.5, 0.5,    //P5
    0.5, -0.5, -0.5,  //P6
    0.5, -0.5, 0.5,   //P7

   //Left face 4 verts
   -0.5, 0.5, -0.5,   //P8
   -0.5, 0.5, 0.5,    //P9
   -0.5, -0.5, -0.5,  //P10
   -0.5, -0.5, 0.5,   //P11

    //Bottom face 4 verts
   -0.5, -0.5, -0.5,  //P12
   -0.5, -0.5, 0.5,   //P13
   0.5, -0.5, -0.5,   //P14
   0.5, -0.5, 0.5,   //P15

    //Top face 4 verts
    0.5, 0.5, -0.5,  //P16
    0.5, 0.5, 0.5,   //P17
    -0.5, 0.5, -0.5, //P18
    -0.5, 0.5, 0.5,  //P19

    //Front face 4 verts
    0.5, 0.5, 0.5,   //P20
    0.5, -0.5, 0.5,  //P21
    -0.5, 0.5, 0.5,  //P22
    -0.5, -0.5, 0.5, //P23
  };
  float CubeNormal[] = {
     0.0, 0.0, -1.0,
     0.0, 0.0, -1.0,
     0.0, 0.0, -1.0,
     0.0, 0.0, -1.0,

     1.0, 0.0, 0.0,
     1.0, 0.0, 0.0,
     1.0, 0.0, 0.0,
     1.0, 0.0, 0.0,

     -1.0, 0.0, 0.0,
     -1.0, 0.0, 0.0,
     -1.0, 0.0, 0.0,
     -1.0, 0.0, 0.0,

     0.0, -1.0, 0.0,
     0.0, -1.0, 0.0,
     0.0, -1.0, 0.0,
     0.0, -1.0, 0.0,

     0.0, 1.0, 0.0,
     0.0, 1.0, 0.0,
     0.0, 1.0, 0.0,
     0.0, 1.0, 0.0,

     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
  };
unsigned short idx[] = {0, 1, 2,
                           0, 2, 3,

                           4, 5, 6,
                           5, 6, 7,

                           8, 9, 10,
                           9, 10, 11,

                           12, 13, 14,
                           13, 14, 15,

                           16, 17, 18,
                           17, 18, 19,

                           20, 21, 22,
                           21, 22, 23};
    g_CiboLen = 36;
    glGenBuffers(1, &CubeBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, CubeBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubePos), CubePos, GL_STATIC_DRAW);

    glGenBuffers(1, &CIndxBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CIndxBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    glGenBuffers(1, &NormalBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, NormalBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(CubeNormal), CubeNormal, GL_STATIC_DRAW);
}

void initSquare() {

  float SqPos[] = {
    -0.5, -0.5, 0.0, //Point0
    -0.5, 0.5, 0.0,  //Point1
    0.5, 0.5, 0.0,   //Point2
    0.5, -0.5, 0.0,  //Point3

  };
  float SqNormal[] = {
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
  };
unsigned short idx[] = {0, 1, 2,
                        0, 2, 3,
                               };
     static GLfloat SqTex[] = {
     0, 0,
     0, 1,
     1, 1,
     1, 0,     
    };

    g_SqiboLen = 6;
    glGenBuffers(1, &SqBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, SqBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SqPos), SqPos, GL_STATIC_DRAW);

    glGenBuffers(1, &SqIndxBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SqIndxBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    glGenBuffers(1, &SqNormalObj);
    glBindBuffer(GL_ARRAY_BUFFER, SqNormalObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SqNormal), SqNormal, GL_STATIC_DRAW);

    glGenBuffers(1, &TexBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, TexBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SqTex), SqTex, GL_STATIC_DRAW);
}

void initScore(glm::vec2 upLeft, glm::vec2 upRight, glm::vec2 downLeft, glm::vec2 downRight) {

  float SqPos[] = {
    -0.5, -0.5, 0.0, //Point0
    -0.5, 0.5, 0.0,  //Point1
    0.5, 0.5, 0.0,   //Point2
    0.5, -0.5, 0.0,  //Point3

  };
  float SqNormal[] = {
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
     0.0, 0.0, 1.0,
  };

  unsigned short idx[] = {0, 1, 2,
                        0, 2, 3,
                               };
     static GLfloat SqTex[] = {
     downLeft.x, downLeft.y,
     upLeft.x, upLeft.y,
     upRight.x, upRight.y,
     downRight.x, downRight.y,     
    };

    g_SqiboLen = 6;
    glGenBuffers(1, &SqBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, SqBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SqPos), SqPos, GL_STATIC_DRAW);

    glGenBuffers(1, &SqIndxBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SqIndxBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    glGenBuffers(1, &SqNormalObj);
    glBindBuffer(GL_ARRAY_BUFFER, SqNormalObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SqNormal), SqNormal, GL_STATIC_DRAW);

    glGenBuffers(1, &TexBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, TexBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(SqTex), SqTex, GL_STATIC_DRAW);
}

void SetupScore(float x, float y, int texture, float scaleX, float scaleY) {
   /*First Cube*/
   SetModel(x, y, scaleX, scaleY);
 
   //setup texture unit
   //glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texture);

   safe_glUniform1i(h_uTexUnit, 1);
   safe_glEnableVertexAttribArray(h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, SqBuffObj);
   safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

   safe_glEnableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, TexBuffObj);
   safe_glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

   safe_glEnableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, SqNormalObj);
   safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

   /* draw!*/
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SqIndxBuffObj);

   glDrawElements(GL_TRIANGLES, g_SqiboLen, GL_UNSIGNED_SHORT, 0);

   //safe_glUniform1i(h_uTexUnit, 0);
   /* Disable the attributes used by our shader*/
   glDisable(GL_TEXTURE_2D);
   safe_glDisableVertexAttribArray(h_aPosition);
   safe_glDisableVertexAttribArray(h_aNormal);
   safe_glDisableVertexAttribArray(h_aTexCoord);
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
}

void SetupSq(float x, float y, int texture, float scaleX, float scaleY) {
   /*First Cube*/
   SetModel(x, y, scaleX, scaleY);
 
   //setup texture unit
   //glEnable(GL_TEXTURE_2D);
   glActiveTexture(GL_TEXTURE1);
   glBindTexture(GL_TEXTURE_2D, texture);


   safe_glUniform1i(h_uTexUnit, 1);
   safe_glEnableVertexAttribArray(h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, SqBuffObj);
   safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

   safe_glEnableVertexAttribArray(h_aTexCoord);
   glBindBuffer(GL_ARRAY_BUFFER, TexBuffObj);
   safe_glVertexAttribPointer(h_aTexCoord, 2, GL_FLOAT, GL_FALSE, 0, 0);

   safe_glEnableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, SqNormalObj);
   safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

   /* draw!*/
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SqIndxBuffObj);

   glDrawElements(GL_TRIANGLES, g_SqiboLen, GL_UNSIGNED_SHORT, 0);

   //safe_glUniform1i(h_uTexUnit, 0);
   /* Disable the attributes used by our shader*/
   glDisable(GL_TEXTURE_2D);
   safe_glDisableVertexAttribArray(h_aPosition);
   safe_glDisableVertexAttribArray(h_aNormal);
   safe_glDisableVertexAttribArray(h_aTexCoord);
}

/* Initialize the geometry */
void InitGeom() {;
  initGround();
  initCube();
  initSquare();

  glGenTextures(1, &skytex);
  //Load texture image
  LoadTexture((char*)"../Assets/Textures/skybox_texture_g.bmp", skytex);
}

/* helper function to set up material for shading */
void SetMaterial(int i) {
  glUseProgram(ShadeProg);
  switch (i) {
    //Bright Lime Green
    case 0:
        safe_glUniform3f(h_uMatAmb, 0.23, 0.5, 0.3);
        safe_glUniform3f(h_uMatDif, 0.04, 1.0, 0.1);
        safe_glUniform3f(h_uMatSpec, 0.23, 0.5, 0.3);
        safe_glUniform1f(h_uMatShine, 15.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
    //Dark Green Shadow
    case 1:
        safe_glUniform3f(h_uMatAmb, 0.125, 0.29, 0.195);
        safe_glUniform3f(h_uMatDif, 0.0, 0.0, 0.0);
        safe_glUniform3f(h_uMatSpec, 0.0, 0.0, 0.0);
        safe_glUniform1f(h_uMatShine, 192.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
    //Ruby
    case 2:
        safe_glUniform3f(h_uMatAmb, 0.4, 0.2, 0.2);
        safe_glUniform3f(h_uMatDif, 0.6, 0.4, 0.4);
        safe_glUniform3f(h_uMatSpec, 0.4, 0.3, 0.3);
        safe_glUniform1f(h_uMatShine, 1.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
    //Sapphire
    case 3:
        safe_glUniform3f(h_uMatAmb, 0.1, 0.1, 0.2);
        safe_glUniform3f(h_uMatDif, 0.3, 0.3, 0.4);
        safe_glUniform3f(h_uMatSpec, 0.3, 0.3, 0.4);
        safe_glUniform1f(h_uMatShine, 150.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
    //Marble
    case 4:
        safe_glUniform3f(h_uMatAmb, 0.7, 0.7, 0.7);
        safe_glUniform3f(h_uMatDif, 0.7, 0.7, 0.7);
        safe_glUniform3f(h_uMatSpec, 0.4, 0.4, 0.4);
        safe_glUniform1f(h_uMatShine, 100.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
    //Blood Red
    case 5:
        safe_glUniform3f(h_uMatAmb, 0.4, 0.0, 0.0);
        safe_glUniform3f(h_uMatDif, 0.7, 0.2, 0.2);
        safe_glUniform3f(h_uMatSpec, 0.7, 0.15, 0.1);
        safe_glUniform1f(h_uMatShine, 10.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
    //Red Clay Steel
    case 6:
        safe_glUniform3f(h_uMatAmb, 0.4, 0.2, 0.2);
        safe_glUniform3f(h_uMatDif, 0.8, 0.4, 0.4);
        safe_glUniform3f(h_uMatSpec, 0.4, 0.3, 0.3);
        safe_glUniform1f(h_uMatShine, 1.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
    //Stainless Steel
    case 7:
        safe_glUniform3f(h_uMatAmb, 0.1, 0.1, 0.1);
        safe_glUniform3f(h_uMatDif, 0.37, 0.37, 0.37);
        safe_glUniform3f(h_uMatSpec, 0.37, 0.37, 0.37);
        safe_glUniform1f(h_uMatShine, 100.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
    //Dark Orange
    case 8:
        safe_glUniform3f(h_uMatAmb, 0.3, 0.15, 0.1);
        safe_glUniform3f(h_uMatDif, 1.0, 0.5, 0.0);
        safe_glUniform3f(h_uMatSpec, 1.0, 0.5, 0.0);
        safe_glUniform1f(h_uMatShine, 75.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
    //Graphite
    case 9:
        safe_glUniform3f(h_uMatAmb, 0.3, 0.3, 0.3);
        safe_glUniform3f(h_uMatDif, 0.7, 0.7, 0.7);
        safe_glUniform3f(h_uMatSpec, 0.9, 0.9, 0.9);
        safe_glUniform1f(h_uMatShine, 100.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
    //Ice
    case 10:
       safe_glUniform3f(h_uMatAmb, 0.6, 0.8, 0.9);
       safe_glUniform3f(h_uMatDif, 0.2, 0.4, 0.5);
       safe_glUniform3f(h_uMatSpec, 1.0, 1.0, 1.0);
       safe_glUniform1f(h_uMatShine, 200.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
       break;
    //Super Shiny Concrete                         
    case 11:
        safe_glUniform3f(h_uMatAmb, 0.3, 0.3, 0.3);
        safe_glUniform3f(h_uMatDif, 0.5, 0.5, 0.5);
        safe_glUniform3f(h_uMatSpec, 0.45, 0.45, 0.45);
        safe_glUniform1f(h_uMatShine, 1.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
    //Teal            
    case 12:
        safe_glUniform3f(h_uMatAmb, 0.453, 1.0, 1.0);
        safe_glUniform3f(h_uMatDif, 0.453, 1.0, 1.0);
        safe_glUniform3f(h_uMatSpec, 0.453, 1.0, 1.0);
        safe_glUniform1f(h_uMatShine, 50.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
    //Collision Box
    case 13:
        safe_glUniform3f(h_uMatAmb, 1.0, 0.0, 0.0);
        safe_glUniform3f(h_uMatDif, 0.0, 0.0, 0.0);
        safe_glUniform3f(h_uMatSpec, 0.0, 0.0, 0.0);
        safe_glUniform1f(h_uMatShine, 0.0);
        safe_glUniform1f(h_uMatAlpha, 1.0);
        break;
        
    //Sky
    case 14:
       safe_glUniform3f(h_uMatAmb, 1.2, 1.6, 1.8);
       safe_glUniform3f(h_uMatDif, 0, 0, 0);
       safe_glUniform3f(h_uMatSpec, 0, 0, 0);
       safe_glUniform1f(h_uMatShine, 200.0);
       safe_glUniform1f(h_uMatAlpha, 1.0);
       break;
   //Start Beacon
    case 15:
       safe_glUniform3f(h_uMatAmb, 1.0, 1.0, 0.0);
       safe_glUniform3f(h_uMatDif, 1.0, 1.0, 0.0);
       safe_glUniform3f(h_uMatSpec, 1.0, 1.0, 0.0);
       safe_glUniform1f(h_uMatShine, 0.0);
       safe_glUniform1f(h_uMatAlpha, 0.4);
       break;
    //End Beacon
    case 16:
       safe_glUniform3f(h_uMatAmb, 0.0, 1.0, 0.0);
       safe_glUniform3f(h_uMatDif, 0.0, 1.0, 0.0);
       safe_glUniform3f(h_uMatSpec, 0.0, 1.0, 0.0);
       safe_glUniform1f(h_uMatShine, 0.0);
       safe_glUniform1f(h_uMatAlpha, 0.4);
       break;
    //Oulines
    case 17:
       safe_glUniform3f(h_uMatAmb, 0.0, 0.0, 0.0);
       safe_glUniform3f(h_uMatDif, 0.0, 0.0, 0.0);
       safe_glUniform3f(h_uMatSpec, 0.0, 0.0, 0.0);
       safe_glUniform1f(h_uMatShine, 0.0);
       safe_glUniform1f(h_uMatAlpha, 1.0);
       break;
    //dust
    case 18:
        safe_glUniform3f(h_uMatAmb, 0.3, 0.3, 0.3);
        safe_glUniform3f(h_uMatDif, 0.0, 0.0, 0.0);
        safe_glUniform3f(h_uMatSpec, 0.0, 0.0, 0.0);
        safe_glUniform1f(h_uMatShine, 100.0);
        safe_glUniform1f(h_uMatAlpha, .25);
        break;
    //money
    case 19:
        safe_glUniform3f(h_uMatAmb, 0.25, 0.8, 0.3);
        safe_glUniform3f(h_uMatDif, 0.0, 0.0, 0.0);
        safe_glUniform3f(h_uMatSpec, 0.0, 0.0, 0.0);
        safe_glUniform1f(h_uMatShine, 100.0);
        safe_glUniform1f(h_uMatAlpha, .9);
        break;
    }
}

void DrawSkyBox() {
  

  // Two UV coordinatesfor each vertex.
  // This corresponds with the order of each vertex.
  static const GLfloat g_uv_buffer_data[] = { 
                //Back
                .273f, 1.f, 
                .273f, .75f, 
                .5223f, .75f,
                .5223f, 1.0f,
                //Right
                .745f, .5f, 
                .523f, .5f,
                .745f, .25f, 
                .523f, .25f, 
                //Left
                .023f, .5f,
                .273f, .5f, 
                .023f, .25f, 
                .273f, .25f,
                //Bottom
                .023f, .5f, 
                .273f, .5f, 
                .023f, .25f,
                .273f, .5f, 
                //Top
                .5223f, .75f, 
                .5223f, .5f,
                .273f, .75f, 
                .273f, .5f, 
                //Front
                .523f, .5f,
                .523f, .25f, 
                .273f, .5f, 
                .273f, .25f,
  };


  // Disable backface culling for skybox
   glCullFace(GL_BACK);
   glDisable(GL_CULL_FACE);
   

  /*GLuint UVBuffObj;
  glGenBuffers(1, &UVBuffObj);
  glBindBuffer(GL_ARRAY_BUFFER, UVBuffObj);
  glBufferData(GL_ARRAY_BUFFER, sizeof(g_uv_buffer_data), g_uv_buffer_data, GL_STATIC_DRAW);

  // Bind our texture in Texture Unit 1
  glActiveTexture(GL_TEXTURE1);
  glBindTexture(GL_TEXTURE_2D, skytex);
  // Set "h_uTexSampler" sampler to user Texture Unit 1
  glUniform1i(h_uTexSampler, 1);*/

  SetMaterial(14);
  
  glDepthMask(GL_FALSE);
  
  SetModel(GetEye().x, GetEye().y, GetEye().z, 1, 1, 1, 1);
  
  safe_glEnableVertexAttribArray(h_aPosition);

   glBindBuffer(GL_ARRAY_BUFFER, CubeBuffObj);
   safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CIndxBuffObj);


   safe_glEnableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, NormalBuffObj);
   safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

   /*safe_glEnableVertexAttribArray(h_aUVVertex);
   glBindBuffer(GL_ARRAY_BUFFER, UVBuffObj);
   safe_glVertexAttribPointer(h_aUVVertex, 2, GL_FLOAT, GL_FALSE, 0, 0);

   /* draw!*/
   glDrawElements(GL_TRIANGLES, g_CiboLen, GL_UNSIGNED_SHORT, 0);
   
   safe_glDisableVertexAttribArray(h_aPosition);
   safe_glDisableVertexAttribArray(h_aNormal);
   /*safe_glDisableVertexAttribArray(h_aUVVertex);

   glDeleteBuffers(1, &UVBuffObj);
   glDeleteTextures(1, &h_uTexSampler);*/

   glDepthMask(GL_ALWAYS);
   
   // Enable backface culling
   glCullFace(GL_BACK);
   glEnable(GL_CULL_FACE);
}
