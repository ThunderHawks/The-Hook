
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>

#define GLM_FORCE_RADIANS
#include "GLSL_helper.h"
#include "MStackHelp.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" //perspective, trans etc
#include "glm/gtc/type_ptr.hpp" //value_ptr
#include <list>
#include <math.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <vector>
#include <time.h>
#include <unistd.h>

#define MODEL_TYPE_UNDEFINED -1
#define MODEL_TYPE_STACKED_CUBES 0
#define MODEL_TYPE_WALL_JUMP 1
#define MODEL_TYPE_RAMP 2

using namespace std;

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
glm::vec3 computeWeightedNormal(int vertex);
int RandomInt(int one, int two);

// Macro used to obtain relative offset of a field within a struct
#define FIELD_OFFSET(StructType, field) &(((StructType *)0)->field)
//
