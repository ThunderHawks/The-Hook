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
	
	int numPosKeyFrames;
	int numRotKeyFrames;
	int numScaleKeyFrames;
	
	std::vector<aiVectorKey> posKeys;
	std::vector<aiQuatKey> rotKeys;
	std::vector<aiVectorKey> scaleKeys;
	glm::mat4 *glmTransforms;
	aiMatrix4x4 *transformations;
	aiMatrix4x4 *personalTrans;
	std::vector<Bone *> childs;
	
	//aiMatrix4x4 meshTrans;					//mesh matrix
	//aiString parentName;						//name of parent
	//aiBone bone;
	//aiNode boneNode;
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
	vertexInfo *skeleton_vertices;
	int numVerts;
	Bone *bone_array;
	Bone *root;
	int boneCt;
	bool hasBones;
};

AssimpMesh loadMesh(const std::string& path);
int setupTrans(Bone *array, Bone *parent, Bone *cur);
int CreateHierarchy(std::vector<aiBone *> *boneNames, aiNode *root, Bone *parent, int *iter, Bone *array);
void CopyaiMat(const aiMatrix4x4 *from, glm::mat4 &to);

#endif // MESH_LOADER_H_

