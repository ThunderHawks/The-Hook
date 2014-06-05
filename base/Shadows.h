#ifndef SHADOWS_H_
#define SHADOWS_H_

#include "glm/glm.hpp"

extern GLint h_uProjMatrix, h_uLightProjMatrix;

class ShadowMap {
public:
   ShadowMap();
   ~ShadowMap();
   int MakeShadowMap(int width, int height);
   int MakeGlowMap(int width, int height);
   void BindDrawFBO();
   void UnbindDrawFBO(int width, int height);
   void BindReadFBO();
   void UnbindReadFBO(int width, int height);
   void BindDepthTex();
   void BindColorTex();
   void UnbindTex();
   GLuint GetColorTex();

private:
   GLuint FrameBuf;
   GLuint DepthTex;
   GLuint ColorTex;
   int texWidth, texHeight;
};

glm::mat4 SetOrthoProjectionMatrix(glm::vec3 eye, glm::vec3 lookAt, float dist);

#endif
