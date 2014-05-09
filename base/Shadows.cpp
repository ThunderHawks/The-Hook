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
#include "Shadows.h"

ShadowMap::ShadowMap() {
   FrameBuf = 0;
   DepthTex = 0;
}

ShadowMap::~ShadowMap() {
}

int ShadowMap::MakeShadowMap(int width, int height) {
   int isSuccessful = 0;

   // Create the depth texture
   glGenTextures(1, &DepthTex);
   glBindTexture(GL_TEXTURE_2D, DepthTex);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

   // Create a frame buffer and attach the depth texture to it
   glGenFramebuffersEXT(1, &FrameBuf);
   glBindFramebufferEXT(GL_FRAMEBUFFER, FrameBuf);
   glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTex, 0);

   // Specify that no color buffers should be written to the frame buffer
   glDrawBuffer(GL_NONE);
   glReadBuffer(GL_NONE);

   // Make sure everything was created alright
   if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      printf("Error creating frame buffer for shadows!\n");      
      isSuccessful = -1;
   }

   glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

   return isSuccessful;
}

void ShadowMap::BindTexWrite() {
   glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, FrameBuf);
}

void ShadowMap::BindTexRead(int texUnit) {
   glActiveTexture(texUnit);
   glBindTexture(GL_TEXTURE_2D, DepthTex);
}

void ShadowMap::UnbindTex() {
   glBindFramebufferEXT(GL_FRAMEBUFFER, 0);
}

void SetOrthoProjectionMatrix() {
   glm::mat4 orthoProj = glm::ortho(-10.f, 10.f, -10.f, 10.f, -10.f, 20.f);
   safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(orthoProj));
}




