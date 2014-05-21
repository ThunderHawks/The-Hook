#ifndef SHADOWS_H_
#define SHADOWS_H_

#include "glm/glm.hpp"

extern GLint h_uProjMatrix, h_uLightProjMatrix;

class ShadowMap {
public:
   ShadowMap();
   ~ShadowMap();
   int MakeShadowMap(int width, int height);
   void BindFBO();
   void UnbindFBO();
   void BindDepthTex();
   void UnbindDepthTex();

private:
   GLuint FrameBuf;
   GLuint DepthTex;
   int texWidth, texHeight;
};

glm::mat4 SetOrthoProjectionMatrix(float dist);

#endif
