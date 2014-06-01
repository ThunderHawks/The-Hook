#ifndef MESH_LOADER_H_
#define MESH_LOADER_H_

#include <assimp/mesh.h> 
#include <assimp/Importer.hpp>      		// C++ importer interface
#include <assimp/scene.h>           		// Output data structure
#include <assimp/postprocess.h>     		// Post processing flags
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp" 	//perspective, trans etc
#include "glm/gtc/type_ptr.hpp" 			 	//value_ptr

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

struct aiScene;

struct Bone {
	aiString name;
	Bone *parent;									//parent in hierarchy
	aiMatrix4x4 offset;							//bone offset matrix

	//these next three will be deleted
	aiMatrix4x4 *transformations;
	glm::mat4 *glmTransforms;
	aiMatrix4x4 *personalTrans;

	glm::mat4 currentTransform;

	std::vector<aiVectorKey> posKeys;
	std::vector<aiQuatKey> rotKeys;
	std::vector<aiVectorKey> scaleKeys;
	std::vector<Bone *> childs;
};

struct vertexInfo {
	aiVector3D position;
	std::vector<float> bone_array; 			//Switched from aiBone to Bone *
	std::vector<float> weight_array;
};

struct AssimpMesh {
   std::vector<float> vertex_array;
   std::vector<float> normal_array;
   std::vector<float> uv_array;
   std::vector<unsigned short> index_array;
	std::vector <vertexInfo> skeleton_vertices;
	int numVerts;
	std::vector<Bone *> bone_array;
	Bone *root;
	int boneCt;
	bool hasBones;
};

AssimpMesh loadMesh(const std::string& path);
int setupTrans(Bone *parent, Bone *cur);
int GetFrame(int frame, AssimpMesh mesh);
int getFrame(Bone *parent, Bone *cur, aiMatrix4x4 globalTrans, int frame);
int CreateHierarchy(std::vector<aiBone *> *boneNames, aiNode *root, Bone *parent, AssimpMesh *mesh);
void CopyaiMat(const aiMatrix4x4 *from, glm::mat4 &to);

#endif // MESH_LOADER_H_

