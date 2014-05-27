#ifndef SHADOWS_H_
#define SHADOWS_H_

#include "glm/glm.hpp"

extern GLint h_uProjMatrix, h_uLightProjMatrix, h_uLightFarProjMatrix;

class ShadowMap {
public:
   ShadowMap();
   ~ShadowMap();
   int MakeShadowMap(int width, int height);
   void BindFBO();
   void BindFBOFar();
   void UnbindFBO();
   void BindDepthTex();
   void BindDepthTexFar();
   void UnbindDepthTex();

private:
   GLuint FrameBuf;
   GLuint DepthTex;
   GLuint FrameBufFar;
   GLuint DepthTexFar;
   int texWidth, texHeight;
};

glm::mat4 SetOrthoProjectionMatrix(float range, float dist, int buf);

#endif
