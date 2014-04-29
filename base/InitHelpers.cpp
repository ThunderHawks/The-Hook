#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define GLM_FORCE_RADIANS
#include "GLSL_helper.h"

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
#include "InitHelpers.h"
#include "Camera.h"

/*function to help load the shaders (both vertex and fragment */
/* for this assignment we are doing anything interesting with the fragment sahder */
int InstallShader(const GLchar *vShaderName, const GLchar *fShaderName) {
   GLuint VS; //handles to shader object
   GLuint FS; //handles to frag shader object
   GLint vCompiled, fCompiled, linked; //status of shader

   printf("IS1\n");

   VS = glCreateShader(GL_VERTEX_SHADER);
   FS = glCreateShader(GL_FRAGMENT_SHADER);

   printf("IS3\n");
   //load the source
   glShaderSource(VS, 1, &vShaderName, NULL);
   glShaderSource(FS, 1, &fShaderName, NULL);
   
   printf("IS2\n");

   //compile shader and print log
   glCompileShader(VS);
   /* check shader status requires helper functions */
   glGetShaderiv(VS, GL_COMPILE_STATUS, &vCompiled);

   //compile shader and print log
   glCompileShader(FS);
   /* check shader status requires helper functions */
   glGetShaderiv(FS, GL_COMPILE_STATUS, &fCompiled);

   if (!vCompiled || !fCompiled) {
       printf("Error compiling either shader %s or %s", vShaderName, fShaderName);
      return 0;
   }

   //create a program object and attach the compiled shader
   ShadeProg = glCreateProgram();
   glAttachShader(ShadeProg, VS);
   glAttachShader(ShadeProg, FS);

   glLinkProgram(ShadeProg);
   /* check shader status requires helper functions */
   glGetProgramiv(ShadeProg, GL_LINK_STATUS, &linked);

   glUseProgram(ShadeProg);
   printf("IS10\n");

   /* get handles to attribute and uniform data in shader */
        /* get handles to attribute data */

        h_aPosition = safe_glGetAttribLocation(ShadeProg, "aPosition");
        h_aNormal = safe_glGetAttribLocation(ShadeProg, "aNormal");
        h_uProjMatrix = safe_glGetUniformLocation(ShadeProg, "uProjMatrix");
        h_uViewMatrix = safe_glGetUniformLocation(ShadeProg, "uViewMatrix");
        h_uModelMatrix = safe_glGetUniformLocation(ShadeProg, "uModelMatrix1");
        h_uLightVec = safe_glGetUniformLocation(ShadeProg, "aLightVec");
        h_uLightColor = safe_glGetUniformLocation(ShadeProg, "uLColor");
        h_uMatAmb = safe_glGetUniformLocation(ShadeProg, "uMat.aColor");
        h_uMatDif = safe_glGetUniformLocation(ShadeProg, "uMat.dColor");
        h_uMatSpec = safe_glGetUniformLocation(ShadeProg, "uMat.sColor");
        h_uMatShine = safe_glGetUniformLocation(ShadeProg, "uMat.shine");
        h_uCamPos = safe_glGetUniformLocation(ShadeProg, "uCamPos");
        h_uShadeMode = safe_glGetUniformLocation(ShadeProg, "uShadeMode");

   printf("sucessfully installed shader %d\n", ShadeProg);
   return 1;
}

//Perform some initial GL setup.
void glInitialize(GLFWwindow *window)
{
   srand(time(0));

   glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

   // Black Background
   glClearColor (0.7f, 0.8f, 0.9f, 1.0f);                                                          
   glClearDepth (1.0f);    // Depth Buffer Setup
   glDepthFunc (GL_LEQUAL);        // The Type Of Depth Testing
   glEnable (GL_DEPTH_TEST);// Enable Depth Testing

   eyeAtx = cos(alpha) * cos(beta);
   eyeAty = sin(alpha);
   eyeAtz = cos(alpha) * cos(M_PI/2.0 - beta);
   SetEye(glm::vec3(eyeAtx, eyeAty, eyeAtz));

   ModelTrans.useModelViewMatrix();
   ModelTrans.loadIdentity();

   // Install the shader.
   if (!InstallShader(textFileRead((char *)"Lab4_vert.glsl"),
                      textFileRead((char *)"Lab4_frag.glsl"))) {
      std::cout << "Error installing shader!" << std::endl;
      exit(EXIT_FAILURE);
   }
   glCullFace(GL_BACK);

//   initTextures();
}
