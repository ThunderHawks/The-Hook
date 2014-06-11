#include <GLFW/glfw3.h>
#include "MStackHelp.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr

extern GLint h_aPosition, h_aNormal, h_uViewMatrix, h_uProjMatrix;
extern GLuint CubeBuffObj, CIndxBuffObj, GrndBuffObj, GIndxBuffObj, GNBuffObj, GNIndxBuffObj;
extern GLuint ShadowCubeBuffObj, SCIndxBuffObj, ShadowNormalBuffObj, RampBuffObj, RIndxBuffObj, RampNormalBuffObj;
extern GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;
extern float g_width, g_height, beta, alpha, eyeAtx, eyeAty, eyeAtz;
extern int ShadeProg;
extern GLint h_uLightVec;
extern GLint h_uLightColor;
extern GLint h_uCamPos, h_uShadeMode;
extern GLint h_uModelMatrix;
extern RenderingHelper ModelTrans;
extern GLint h_aTexCoord, h_uTexUnit;  

class ShadowMap {
public:
   ShadowMap();
   ~ShadowMap();
   int MakeShadowMap(int width, int height);
   void BindFBO();
   void BindDepthTex(int texUnit);
   void UnbindFBO();

private:
   GLuint FrameBuf;
   GLuint DepthTex;
};

glm::mat4 SetOrthoProjectionMatrix();
