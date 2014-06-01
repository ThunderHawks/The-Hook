
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

float g_groundSize = 10000.0, g_groundY = 0;   // half the ground length, the y of the ground, 

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

/*static void initRamp() {
   float RampPos[] = {
      //Slanted face, 4 verts
      0.25, 0.0, 0.5, //0
      -0.25, 0.0, 0.5, //1
      0.25, 0.5, -0.25, //2
      -0.25, 0.5, -0.25, //3

      //Right Triangle, 3 verts
      0.25, 0.0, 0.5, //4
      0.25, 0.0, -0.25, //5
      0.25, 0.5, -0.25, //6

      //Left Triangle, 3 verts
      -0.25, 0.0, 0.5, //7
      -0.25, 0.0, -0.25, //8
      -0.25, 0.5, -0.25, //9

      //Back Face, 4 verts
      0.25, 0, -0.25, //10 
      -0.25, 0, -0.25, //11
      0.25, 0.5, -0.25, //12
      -0.25, 0.5, -0.25, //13

      //Bottom, 4 verts
      0.25, 0, 0.5, //14
      -0.25, 0, 0.5,  //15
      0.25, 0, -0.25, //16
      -0.25, 0, -0.25, //17
   };*/
/*float RampNormal[] = {
      0, 0.8, 1,
      0, 0.8, 1,
      0, 0.8, 1,
      0, 0.8, 1,

      1.0, 0.0, 0.0,
      1.0, 0.0, 0.0,
      1.0, 0.0, 0.0,

      -1.0, 0.0, 0.0,
      -1.0, 0.0, 0.0,
      -1.0, 0.0, 0.0,

      0.0, 0.0, -1.0,
      0.0, 0.0, -1.0,
      0.0, 0.0, -1.0,

      0.0, -1.0, 0.0,
      0.0, -1.0, 0.0,
      0.0, -1.0, 0.0,
   };
   unsigned short idx[] = {0, 1, 3,
                           0, 2, 3,

                           4, 5, 6,

                           7, 8, 9,

                           10, 11, 12,
                           11, 12, 13,

                           14, 15, 16,
                           15, 16, 17,
                           };

    g_RiboLen = 24;
    glGenBuffers(1, &RampBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, RampBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(RampPos), RampPos, GL_STATIC_DRAW);

    glGenBuffers(1, &RIndxBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, RIndxBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    glGenBuffers(1, &RampNormalBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, RampNormalBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(RampNormal), RampNormal, GL_STATIC_DRAW);

}*/
/*static void initCubeShadow() {
   float ShadowPos[] = {
      0.5, 0.0, 0.5, //P0
      -0.5, 0.0, 0.5, //P1
      -0.5, 0.0, -0.5, //P2
      0.5, 0.0, -0.5, //P3
   };

    float ShadowNormal[] = {
     0, 1, 0,
     0, 1, 0,
     0, 1, 0,
     0, 1, 0,
     0, 1, 0,
     0, 1, 0
    };

   unsigned short idx[] = {0, 1, 2,
                           0, 2, 3};

    g_SCiboLen = 6;
    glGenBuffers(1, &ShadowCubeBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, ShadowCubeBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ShadowPos), ShadowPos, GL_STATIC_DRAW);

    glGenBuffers(1, &SCIndxBuffObj);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SCIndxBuffObj);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(idx), idx, GL_STATIC_DRAW);

    glGenBuffers(1, &ShadowNormalBuffObj);
    glBindBuffer(GL_ARRAY_BUFFER, ShadowNormalBuffObj);
    glBufferData(GL_ARRAY_BUFFER, sizeof(ShadowNormal), ShadowNormal, GL_STATIC_DRAW);
}*/


/* Initialize the geometry */
void InitGeom() {
  //initRamp();
  initGround();
  initCube();
  /*initCubeShadow();
  initMesh();*/
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
        break;
    //Dark Green Shadow
    case 1:
        safe_glUniform3f(h_uMatAmb, 0.125, 0.29, 0.195);
        safe_glUniform3f(h_uMatDif, 0.0, 0.0, 0.0);
        safe_glUniform3f(h_uMatSpec, 0.0, 0.0, 0.0);
        safe_glUniform1f(h_uMatShine, 192.0);
        break;
    //Ruby
    case 2:
        safe_glUniform3f(h_uMatAmb, 0.4, 0.2, 0.2);
        safe_glUniform3f(h_uMatDif, 0.6, 0.4, 0.4);
        safe_glUniform3f(h_uMatSpec, 0.4, 0.3, 0.3);
        safe_glUniform1f(h_uMatShine, 1.0);
        break;
    //Sapphire
    case 3:
        safe_glUniform3f(h_uMatAmb, 0.1, 0.1, 0.2);
        safe_glUniform3f(h_uMatDif, 0.3, 0.3, 0.4);
        safe_glUniform3f(h_uMatSpec, 0.3, 0.3, 0.4);
        safe_glUniform1f(h_uMatShine, 150.0);
        break;
    //Marble
    case 4:
        safe_glUniform3f(h_uMatAmb, 0.7, 0.7, 0.7);
        safe_glUniform3f(h_uMatDif, 0.7, 0.7, 0.7);
        safe_glUniform3f(h_uMatSpec, 0.4, 0.4, 0.4);
        safe_glUniform1f(h_uMatShine, 100.0);
        break;
    //Blood Red
    case 5:
        safe_glUniform3f(h_uMatAmb, 0.4, 0.0, 0.0);
        safe_glUniform3f(h_uMatDif, 0.7, 0.2, 0.2);
        safe_glUniform3f(h_uMatSpec, 0.7, 0.15, 0.1);
        safe_glUniform1f(h_uMatShine, 10.0);
        break;
    //Red Clay Steel
    case 6:
        safe_glUniform3f(h_uMatAmb, 0.4, 0.2, 0.2);
        safe_glUniform3f(h_uMatDif, 0.8, 0.4, 0.4);
        safe_glUniform3f(h_uMatSpec, 0.4, 0.3, 0.3);
        safe_glUniform1f(h_uMatShine, 1.0);
        break;
    //Stainless Steel
    case 7:
        safe_glUniform3f(h_uMatAmb, 0.1, 0.1, 0.1);
        safe_glUniform3f(h_uMatDif, 0.37, 0.37, 0.37);
        safe_glUniform3f(h_uMatSpec, 0.37, 0.37, 0.37);
        safe_glUniform1f(h_uMatShine, 100.0);
        break;
    //Dark Orange
    case 8:
        safe_glUniform3f(h_uMatAmb, 0.3, 0.15, 0.1);
        safe_glUniform3f(h_uMatDif, 1.0, 0.5, 0.0);
        safe_glUniform3f(h_uMatSpec, 1.0, 0.5, 0.0);
        safe_glUniform1f(h_uMatShine, 75.0);
        break;
    //Graphite
    case 9:
        safe_glUniform3f(h_uMatAmb, 0.3, 0.3, 0.3);
        safe_glUniform3f(h_uMatDif, 0.7, 0.7, 0.7);
        safe_glUniform3f(h_uMatSpec, 0.9, 0.9, 0.9);
        safe_glUniform1f(h_uMatShine, 100.0);
        break;
    //Ice
    case 10:
       safe_glUniform3f(h_uMatAmb, 0.6, 0.8, 0.9);
       safe_glUniform3f(h_uMatDif, 0.2, 0.4, 0.5);
       safe_glUniform3f(h_uMatSpec, 1.0, 1.0, 1.0);
       safe_glUniform1f(h_uMatShine, 200.0);
       break;
    //Super Shiny Concrete                         
    case 11:
        safe_glUniform3f(h_uMatAmb, 0.3, 0.3, 0.3);
        safe_glUniform3f(h_uMatDif, 0.5, 0.5, 0.5);
        safe_glUniform3f(h_uMatSpec, 0.45, 0.45, 0.45);
        safe_glUniform1f(h_uMatShine, 1.0);
        break;
    //Teal            
    case 12:
        safe_glUniform3f(h_uMatAmb, 0.453, 1.0, 1.0);
        safe_glUniform3f(h_uMatDif, 0.453, 1.0, 1.0);
        safe_glUniform3f(h_uMatSpec, 0.453, 1.0, 1.0);
        safe_glUniform1f(h_uMatShine, 50.0);
        break;
    //Collision Box
    case 13:
        safe_glUniform3f(h_uMatAmb, 1.0, 0.0, 0.0);
        safe_glUniform3f(h_uMatDif, 0.0, 0.0, 0.0);
        safe_glUniform3f(h_uMatSpec, 0.0, 0.0, 0.0);
        safe_glUniform1f(h_uMatShine, 0.0);
        break;

    }
}
