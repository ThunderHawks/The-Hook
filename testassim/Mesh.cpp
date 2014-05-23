#define GL_GLEXT_PROTOTYPES
#include "Mesh.h"
using namespace glm;

Mesh::Mesh()
    : PositionHandle(0), NormalHandle(0), IndexHandle(0), IndexBufferLength(0)
{
	hasAss = false;
}

Mesh::Mesh(std::vector<float> const & Positions, std::vector<float> const & Normals, std::vector<unsigned short> const & Indices)
    : PositionHandle(0), NormalHandle(0), IndexHandle(0), IndexBufferLength(0)
{
	hasAss = false;
    IndexBufferLength = Indices.size();
    
    glGenBuffers(1, & PositionHandle);
    glBindBuffer(GL_ARRAY_BUFFER, PositionHandle);
    glBufferData(GL_ARRAY_BUFFER, Positions.size() * sizeof(float), & Positions.front(), GL_STATIC_DRAW);

    glGenBuffers(1, & NormalHandle);
    glBindBuffer(GL_ARRAY_BUFFER, NormalHandle);
    glBufferData(GL_ARRAY_BUFFER, Normals.size() * sizeof(float), & Normals.front(), GL_STATIC_DRAW);

    glGenBuffers(1, & IndexHandle);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexHandle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(unsigned short), & Indices.front(), GL_STATIC_DRAW);
}

Mesh::Mesh(AssimpMesh aMesh)
    : PositionHandle(0), NormalHandle(0), IndexHandle(0), IndexBufferLength(0)
{
	std::vector<float> temp;
	
	for (int i = 0; i < aMesh.numVerts; ++i) {
		temp.push_back((float) aMesh.skeleton_vertices[i].position.x);
		temp.push_back((float) aMesh.skeleton_vertices[i].position.y);
		temp.push_back((float) aMesh.skeleton_vertices[i].position.z);
	}
		
	hasAss = true;
	IndexBufferLength = aMesh.index_array.size();
    
	glGenBuffers(1, & PositionHandle);
	glBindBuffer(GL_ARRAY_BUFFER, PositionHandle);
	glBufferData(GL_ARRAY_BUFFER, temp.size() * sizeof(float), & temp.front(), GL_STATIC_DRAW);

	glGenBuffers(1, & NormalHandle);
	glBindBuffer(GL_ARRAY_BUFFER, NormalHandle);
	glBufferData(GL_ARRAY_BUFFER, aMesh.normal_array.size() * sizeof(float), & aMesh.normal_array.front(), GL_STATIC_DRAW);

	glGenBuffers(1, & IndexHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, aMesh.index_array.size() * sizeof(unsigned short), & aMesh.index_array.front(), GL_STATIC_DRAW);
}

/*******************************************************************************
Input: takes in the path of to the model you want 

Uses Assimp to convert models into useable stuff

Output: a Mesh of the model you want
******************************************************************************/
Mesh LoadMesh(std::string file) {
	AssimpMesh AssimpModel = loadMesh(file);
	std::vector<float> temp;
	Mesh ret;
	
	if (AssimpModel.hasBones)
		ret = Mesh(AssimpModel);
	else
		ret = Mesh(AssimpModel.vertex_array, AssimpModel.normal_array, AssimpModel.index_array);
	
	return ret;
}

/*******************************************************************************
Input: takes in a Mesh, and everything you would ever want to do to it. The angle is rotation around the y axis

draws the model :D AWW YEAH!

Output: YOU GET NOTHING!
******************************************************************************/
void PlaceModel(Mesh mesh, float locx, float locy, float locz, float sx, float sy, float sz, float angle) {
   SetModel(locx, locy, locz, sx, sy, sz, angle);
   //safe_glEnableVertexAttribArray(h_aPosition);
   glEnableVertexAttribArray(h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, mesh.PositionHandle);
   //safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);

   //safe_glEnableVertexAttribArray(h_aNormal);
   glEnableVertexAttribArray(h_aNormal);
   glBindBuffer(GL_ARRAY_BUFFER, mesh.NormalHandle);
   //safe_glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glVertexAttribPointer(h_aNormal, 3, GL_FLOAT, GL_FALSE, 0, 0);

   /* draw!*/
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.IndexHandle);

   glDrawElements(GL_TRIANGLES, mesh.IndexBufferLength, GL_UNSIGNED_SHORT, 0);
}

