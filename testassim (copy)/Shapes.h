extern GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine;
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
