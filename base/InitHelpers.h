#ifndef INIT_HELPERS_H_
#define INIT_HELPERS_H_

#include <GLFW/glfw3.h>
#include "MStackHelp.h"

#define STARTSCREEN_MODE -1

extern GLint h_aPointSize, h_aPosition, h_aNormal, h_aUVVertex, h_uViewMatrix, h_uProjMatrix;
extern GLuint CubeBuffObj, CIndxBuffObj, GrndBuffObj, GIndxBuffObj, GNBuffObj, GNIndxBuffObj, h_uTexSampler;

extern GLuint ShadowCubeBuffObj, SCIndxBuffObj, ShadowNormalBuffObj, RampBuffObj, RIndxBuffObj, RampNormalBuffObj;
extern GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine, h_uMatAlpha;
extern GLint h_uTexUnit;
extern GLint h_uLightViewMatrix, h_uLightProjMatrix;
extern float g_width, g_height, beta, alpha, eyeAtx, eyeAty, eyeAtz;
extern int ShadeProg;
extern GLint h_uLightVec;
extern GLint h_uLightColor;
extern GLint h_uCamPos, h_uShadeMode;
extern GLint h_uModelMatrix;
extern RenderingHelper ModelTrans;
extern GLint h_uTexCoord, h_aTexCoord, h_uGuiMode, h_uTextMode, h_utexpos;

// Animation stuff
extern GLint h_uAnimFlag;
extern GLint h_uNumWeights;
extern GLint h_uBoneMatrix;
extern GLint h_uWeights;
extern GLint h_uJoints;
//

int InstallShader(const GLchar *vShaderName, const GLchar *fShaderName);
void glInitialize(GLFWwindow *window);

#endif
