#ifndef SHAPES_H_
#define SHAPES_H_

#include "Mesh.h"

extern GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine, h_uMatAlpha, h_aPosition, h_aNormal, h_uaUVVertex;
extern int ShadeProg;
extern GLuint CubeBuffObj, CIndxBuffObj, GrndBuffObj, GIndxBuffObj, GNBuffObj, GNIndxBuffObj, SqIndxBuffObj, SqBuffObj, SqNormalObj, TexBuffObj, h_uTexSampler;
extern int g_CiboLen, g_GiboLen, g_SqiboLen;
extern GLuint NormalBuffObj;

void initGround();
void initCube();
void SetupCube(float x, float y, float z, int material, float angle, float scaleX, float scaleY, float scaleZ);
void SetupSq(float x, float y, int texture, float scaleX, float scaleY);
//void initRamp()
//static void initCubeShadow()
void InitGeom();
void SetMaterial(int i);
void DrawSkyBox();
GLuint loadBMP(const char* path);

#endif
