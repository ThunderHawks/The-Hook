#define GL_GLEXT_PROTOTYPES
#include "GLSL_helper.h"
#include "Mesh.h"
using namespace glm;

Mesh::Mesh()
    : PositionHandle(0), NormalHandle(0), IndexHandle(0), IndexBufferLength(0)
{}

/*Creates a Mesh Object*/
Mesh::Mesh(std::vector<float> const & Positions, std::vector<float> const & Normals, std::vector<unsigned short> const & Indices)
    : PositionHandle(0), NormalHandle(0), IndexHandle(0), IndexBufferLength(0)
{
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

/*******************************************************************************
Input: takes in the path of to the model you want 

Uses Assimp to convert models into useable stuff

Output: a Mesh of the model you want
******************************************************************************/
Mesh LoadMesh(std::string file) {
	AssimpMesh AssimpModel = loadMesh(file);
	Mesh ret = Mesh(AssimpModel.vertex_array, AssimpModel.normal_array, AssimpModel.index_array);
	
	return ret;
}

/*******************************************************************************
Input: takes in a Mesh, and everything you would ever want to do to it. The angle is rotation around the y axis

draws the model :D AWW YEAH!

Output: YOU GET NOTHING!
******************************************************************************/
void PlaceModel(Mesh mesh, float locx, float locy, float locz, float sx, float sy, float sz, float angle) {
   mat4 Mod = SetModel(locx, locy, locz, sx, sy, sz, angle);
   
   if (checkViewFrustum (glm::vec3 (0,0,0), 0, curProj*curView*Mod) == 0) {
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
}

/*******************************************************************************
Input: Nothing

Sends the model view matrix to GLSL. The model view matrix is based off of ModelTrans for this one

Output: The model matrix you set
******************************************************************************/
glm::mat4 SetModelStat() {
   safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ModelTrans.modelViewMatrix));
   return ModelTrans.modelViewMatrix;
}

/*******************************************************************************
Input: translation x, y, z. Scaling x, y, z. And an angle of rotation arond the y axis

Sends the model view matrix to GLSL. The model view matrix is made off the inputs. order of scale, Rotate, then translate.

Output: The model matrix you set
******************************************************************************/
glm::mat4 SetModel(float x, float y, float z, float Sx, float Sy, float Sz, float angle) {
   glm::mat4 Trans = glm::translate( glm::mat4(1.0f), glm::vec3(x, y, z));
   glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(Sx, Sy, Sz));
   //printf("%f %f ModelPosition\n",Sy,Sz);
   glm::mat4 Rotate = glm::rotate(glm::mat4(1.0f), angle, glm::vec3(0.0f, 1.0f, 0.0f));
   glm::mat4 ctm = Trans * Rotate * Scale;
   safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ctm));
   
   return ctm;
}

/*currently does nothing, but might be used in the future. Talk to Andrew M.*/
void drawAtM4(Mesh *m, mat4 trans, int shaderProg){

}
