#ifndef MESH_LOADER_H_
#define MESH_LOADER_H_

#include <assimp/mesh.h> 
#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags

#include <iostream>
#include <cstdio>
#include <string>
#include <vector>

struct aiScene;

struct Bone {
	aiString name;
	Bone *parent;					//parent in hierarchy
	aiMatrix4x4 offset;			//bone offset matrix
	
	int numPosKeyFrames;
	int numRotKeyFrames;
	int numScaleKeyFrames;
	aiVectorKey *posKeys;
	aiQuatKey *rotKeys;
	aiVectorKey *scaleKeys;
	aiMatrix4x4 *transformations;
	aiMatrix4x4 *personalTrans;
	
	//aiMatrix4x4 meshTrans;		//mesh matrix
	//aiString parentName;			//name of parent
	//aiBone bone;
	//aiNode boneNode;
};

struct vertexInfo {
	aiVector3D position;
	std::vector<Bone *> bone_array; //Switched from aiBone to Bone -- hopefully correct
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
	int boneCt;
	bool hasBones;
};

AssimpMesh loadMesh(const std::string& path);

#endif // MESH_LOADER_H_

//print out vertex position, bones names, and weight for each bone. DONT DO FOR EVERY VERTEX
