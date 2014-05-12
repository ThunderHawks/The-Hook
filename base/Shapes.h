#ifndef SHAPES_H_
#define SHAPES_H_

#include "Mesh.h"

extern GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine, h_uMatAlpha, h_aPosition, h_aNormal;
extern int ShadeProg;
extern GLuint CubeBuffObj, CIndxBuffObj, GrndBuffObj, GIndxBuffObj, GNBuffObj, GNIndxBuffObj;
extern int g_CiboLen, g_GiboLen;
extern GLuint NormalBuffObj;

void initGround();
void initCube();
//void initRamp()
//static void initCubeShadow()
void InitGeom();
void SetMaterial(int i);
void DrawSkyBox();

#endif
