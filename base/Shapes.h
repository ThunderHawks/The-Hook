#ifndef SHAPES_H_
#define SHAPES_H_

#include "Mesh.h"

extern GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine, h_uMatAlpha, h_aPosition, h_aNormal, h_uTexUnit, h_aTexCoord, h_aUVVertex;
extern int ShadeProg;
extern GLuint CubeBuffObj, CIndxBuffObj, GrndBuffObj, GIndxBuffObj, GNBuffObj, GNIndxBuffObj, SqIndxBuffObj, SqBuffObj, SqNormalObj, TexBuffObj, h_uTexSampler;
extern int g_CiboLen, g_GiboLen, g_SqiboLen;
extern GLuint NormalBuffObj;

void initGround();
void initCube();
void initScore(glm::vec2 upLeft, glm::vec2 upRight, glm::vec2 downLeft, glm::vec2 downRight);
void SetupCube(float x, float y, float z, int material, float angle, float scaleX, float scaleY, float scaleZ);
void SetupColoredSq(float x, float y, glm::vec3 rgbVals, float scaleX, float scaleY);
void SetupSq(float x, float y, int texture, float scaleX, float scaleY);
void SetupScore(float x, float y, int texture, float scaleX, float scaleY);
//void initRamp()
//static void initCubeShadow()
void InitGeom();
void SetCustomMaterialColor(float r, float g, float b);
void SetMaterial(int i);
void DrawSkyBox();

#endif
