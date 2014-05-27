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
   FrameBufFar = 0;
   DepthTexFar = 0;
}

ShadowMap::~ShadowMap() {
}

int ShadowMap::MakeShadowMap(int width, int height) {
   float borderColor[] = {1.0f, 1.0f, 1.0f, 1.0f};
  
   texWidth = width;
   texHeight = height;

   // Create the depth texture
   glGenTextures(1, &DepthTex);
   glBindTexture(GL_TEXTURE_2D, DepthTex);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texWidth, texHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

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
      return -1;
   }


   // Create the wide depth texture
   glGenTextures(1, &DepthTexFar);
   glBindTexture(GL_TEXTURE_2D, DepthTexFar);
   glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, texWidth, texHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
   glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
   glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

   // Create a frame buffer and attach the depth texture to it
   glGenFramebuffersEXT(1, &FrameBufFar);
   glBindFramebufferEXT(GL_FRAMEBUFFER, FrameBufFar);
   glFramebufferTexture2DEXT(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, DepthTexFar, 0);

   // Specify that no color buffers should be written to the frame buffer
   glDrawBuffer(GL_NONE);
   glReadBuffer(GL_NONE);

   // Make sure everything was created alright
   if (glCheckFramebufferStatusEXT(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
      printf("Error creating 2nd frame buffer for shadows!\n");      
      return -1;
   }

   glBindFramebufferEXT(GL_FRAMEBUFFER, 0);

   return 0;
}

void ShadowMap::BindFBO() {
   glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, FrameBuf);
}

void ShadowMap::BindFBOFar() {
   glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, FrameBufFar);
}

void ShadowMap::UnbindFBO() {
   glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER, 0);
}

void ShadowMap::BindDepthTex() {
   glBindTexture(GL_TEXTURE_2D, DepthTex);
}

void ShadowMap::BindDepthTexFar() {
   glBindTexture(GL_TEXTURE_2D, DepthTexFar);
}

void ShadowMap::UnbindDepthTex() {
   glBindTexture(GL_TEXTURE_2D, 0);
}

glm::mat4 SetOrthoProjectionMatrix(float range, float dist, int buf) {
   glm::mat4 orthoProj = glm::ortho(-range, range, -range, range, dist - 50.f, dist + 100.f);
   safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(orthoProj));
   if (buf == 0)
      safe_glUniformMatrix4fv(h_uLightProjMatrix, glm::value_ptr(orthoProj)); 
   else
      safe_glUniformMatrix4fv(h_uLightFarProjMatrix, glm::value_ptr(orthoProj)); 
   return orthoProj;
}




