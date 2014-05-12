/*
 * OpenGL 3.0 Core-friendly replacement for glu primitives
 *
 *  Created on: 9/26/2012
 *      Author: Ian Dunn
 */

#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OPENGL/gl.h>
#endif

#ifdef __unix__
#include <GL/glut.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#include <GL/glut.h>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "freeglut.lib")
#endif

#include "glm/glm.hpp"
#include "mesh_loader.h"
#include "InitHelpers.h"
#include <vector>
#include <string>

struct Mesh
{
    GLuint PositionHandle;
    GLuint NormalHandle;
    GLuint IndexHandle;

    unsigned int IndexBufferLength;

    Mesh();
    Mesh(std::vector<float> const & Positions, std::vector<float> const & Normals, std::vector<unsigned short> const & Indices);
};

Mesh LoadMesh(std::string file);
void PlaceModel(Mesh mesh, float locx, float locy, float locz, float sx, float sy, float sz, float angle);
