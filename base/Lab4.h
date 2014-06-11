
#ifndef LAB4_H_
#define LAB4_H_

#ifndef GL_GLEW_H_
#define GL_GLEW_H_
#include <GL/glew.h>
#endif 

#ifndef GLFW_3_H_
#define GLFW_3_H_
#include <GLFW/glfw3.h>
#endif

#include <stdlib.h>
#include <stdio.h>

#define GLM_FORCE_RADIANS
#include "GLSL_helper.h"
#include "MStackHelp.h"
#include "InitHelpers.h"

#ifndef GLM_HPP_
#define GLM_HPP_
#include "glm/glm.hpp"
#endif
#ifndef GLM_GTC_MATRIX_TRANSFORM_HPP
#define GLM_GTC_MATRIX_TRANSFORM_HPP
#include "glm/gtc/matrix_transform.hpp"
#endif

#ifndef GLM_GTC_TYPE_HPP
#define GLM_GTC_TYPE_HPP
#include "glm/gtc/type_ptr.hpp"
#endif

#include <list>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <time.h>
#include <unistd.h>
#include "particle.h"

#define STARTSCREEN_MODE -1
#define GAME_MODE 0
#define EDIT_MODE 1

// For playerAnim
#define BIND_POSE 0
#define START_RUN 1
#define RUN			2
#define JUMP		3
#define LAND		4
#define FIRE		5
#define FLAIL		6
#define CRASH		7
#define IDLE		8

using namespace std;

extern float g_width, g_height;
extern GLint h_uLightVec;
extern GLint h_uLightColor;
extern GLint h_uCamPos, h_uShadeMode;
extern GLint h_uMatAmb, h_uMatDif, h_uMatSpec, h_uMatShine, h_uMatAlpha;
extern GLint h_uTexUnit, h_uTexUnit2;
extern GLint h_uLightViewMatrix, h_uLightProjMatrix;
extern GLint h_aTexCoord;
//extern GLint h_aPointSize;
extern std::list<part*> particleSpawner;
extern GLuint TexBuffObj;
extern RenderingHelper ModelTrans;

extern int anim;
extern int startFrames[10];
extern int endFrames[10];


typedef struct Node {
   glm::vec3 Position;
   int modelType;
   struct Node *next;
} Node;

class SColor {

public:

    float Red, Green, Blue;

    SColor()
        : Red(0.7f), Green(0.7f), Blue(0.7f) {}

};

/* class to store vector for position (or vector) */
class SVector3 {

public:

        float X, Y, Z;

        SVector3()
                : X(0), Y(0), Z(0) {}

        SVector3(float in)
                : X(in), Y(in), Z(in) {}

        SVector3(float in_x, float in_y, float in_z)
                : X(in_x), Y(in_y), Z(in_z) {}

        SVector3 crossProduct(SVector3 const & v) const {
                return SVector3(Y*v.Z - v.Y*Z, v.X*Z - X*v.Z, X*v.Y - v.X*Y);
        }

        float dotProduct(SVector3 const & v) const {
                return X*v.X + Y*v.Y + Z*v.Z;
        }

        float length() const {
                return sqrtf(X*X + Y*Y + Z*Z);
        }

        SVector3 operator + (SVector3 const & v) const {
                return SVector3(X+v.X, Y+v.Y, Z+v.Z);
        }

        SVector3 & operator += (SVector3 const & v) {
                X += v.X;
                Y += v.Y;
                Z += v.Z;

                return * this;
        }
       SVector3 operator - (SVector3 const & v) const {
                return SVector3(X-v.X, Y-v.Y, Z-v.Z);
        }

        SVector3 & operator -= (SVector3 const & v) {
                X -= v.X;
                Y -= v.Y;
                Z -= v.Z;

                return * this;
        }

        SVector3 operator * (SVector3 const & v) const {
                return SVector3(X*v.X, Y*v.Y, Z*v.Z);
        }

        SVector3 & operator *= (SVector3 const & v) {
                X *= v.X;
                Y *= v.Y;
                Z *= v.Z;

                return * this;
        }

        SVector3 operator / (SVector3 const & v) const {
                return SVector3(X/v.X, Y/v.Y, Z/v.Z);
        }

        SVector3 & operator /= (SVector3 const & v) {
                X /= v.X;
                Y /= v.Y;
                Z /= v.Z;

                return * this;
        }

        SVector3 operator * (float const s) const {
                return SVector3(X*s, Y*s, Z*s);
        }

        SVector3 & operator *= (float const s) {
                X *= s;
                Y *= s;
                Z *= s;

                return * this;
        }

        SVector3 operator / (float const s) const {
                return SVector3(X/s, Y/s, Z/s);
        }

        SVector3 & operator /= (float const s) {
                X /= s;
                Y /= s;
                Z /= s;

                return * this;
        }

};
/* class to store a mesh vertex with both a position and color */
class SVertex {

public:

    SVector3 Position;
    SColor Color;

};

/* class to store all the vertices and all the triangles of a mesh */
class CMesh {

public:

    struct STriangle
    {
        unsigned int VertexIndex1, VertexIndex2, VertexIndex3;
                SColor Color;
    };

    std::vector<SVertex> Vertices;
    std::vector<STriangle> Triangles;

    CMesh();
        void PrintMesh();
        float WStoPS(float input);
        void Draw();

    ~CMesh();
    void centerMeshByExtents(SVector3 const & CenterLocation);

    void resizeMesh(SVector3 const & Scale);

};

void Add_Entry(glm::vec3 Position);
int Table_Size();
void Set_ModelType(int index, int type);
Node Get_Index(int index);
void Free_LinkedList();
void setCool(int i);
float getFPS();
//Bool that returns true if game is paused
bool isPaused();
//Pauses/unpasses game
void pauseorUnpause();
glm::vec3 computeWeightedNormal(int vertex);
/* projection matrix  - do not change */
glm::mat4 SetProjectionMatrix();
int RandomInt(int one, int two);
void toStartScreen();
void initStartScreen();
void initPlay(string fileName);
void initEdit(string fileName);


// Macro used to obtain relative offset of a field within a struct
#define FIELD_OFFSET(StructType, field) &(((StructType *)0)->field)
#endif 
