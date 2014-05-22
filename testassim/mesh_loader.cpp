#include "mesh_loader.h"
#include "Mesh.h"

int CreateHierarchy(std::vector<aiBone *> *boneNames, aiNode *root, Bone *parent, int *iter, Bone *array);

AssimpMesh loadMesh(const std::string& path) {
	Assimp::Importer importer;

	const aiScene* scene = importer.ReadFile(path,
		aiProcess_CalcTangentSpace       |
		aiProcess_Triangulate            |
		aiProcess_JoinIdenticalVertices  |
		aiProcess_SortByPType);
	if (!scene) {
		std::cerr << "Failed to load: " << path << std::endl;
		exit(EXIT_FAILURE);
	}

	const size_t kNumAxes = 3;
	aiMesh& mesh = *scene->mMeshes[0];
	AssimpMesh ret;
	ret.vertex_array = std::vector<float>(
          (float*)(mesh.mVertices),
          (float*)(mesh.mVertices) + mesh.mNumVertices * kNumAxes);

    ret.uv_array = std::vector<float>(
          (float*)(mesh.mTextureCoords),
          (float*)(mesh.mTextureCoords) + mesh.mNumVertices * kNumAxes);

    ret.normal_array = std::vector<float>(
          (float*)(mesh.mNormals),
          (float*)(mesh.mNormals) + mesh.mNumVertices * kNumAxes);

    for (unsigned int i = 0; i < mesh.mNumFaces; ++i) {
       ret.index_array.insert(
             ret.index_array.end(),
             mesh.mFaces[i].mIndices,
             mesh.mFaces[i].mIndices + kNumAxes);
    }
/******************************************************************************/
	int i, j;
	ret.skeleton_vertices = (vertexInfo *)calloc(sizeof(vertexInfo), mesh.mNumVertices);
		
	//fill all the positions for the skeleton_vertices
	for (i = 0; i < mesh.mNumVertices; i++) {
		ret.skeleton_vertices[i].position = mesh.mVertices[i];
		/*other option if we ues vec3*/
		/*
		ret.skeleton_verticies[i*3].position.x = ret.vertex_array[i];
		ret.skeleton_verticies[i*3 + 1].position.y = ret.vertex_array[i + 1];
		ret.skeleton_verticies[i*3 + 2].position.z = ret.vertex_array[i + 2];
		*/
	}

	ret.hasBones = mesh.HasBones();
	if(ret.hasBones) {
		//create a vector to hold all the names of the bones
		std::vector<aiBone *> boneNames;
		//intialize the array of bones
		ret.boneCt = mesh.mNumBones;
		ret.bone_array = (Bone *)calloc(sizeof(Bone), mesh.mNumBones);
		
		int smallest = 100;
		int iter = 0;
		aiNode * root = scene->mRootNode->FindNode(mesh.mBones[0]->mName.C_Str());
		
		//find the root bone
		for (j = 0; j < mesh.mNumBones; j++) {
			boneNames.push_back(mesh.mBones[j]);
			aiNode * temp = scene->mRootNode->FindNode(mesh.mBones[j]->mName.C_Str());
			for( i = 0; temp->mParent != NULL; i++) {
				temp = temp->mParent;
			}
			if(i < smallest) {
				smallest = i;
				root = temp;
			}
			//TODO: NOT REALLY SURE IF THIS WORKS CONSIDERING THIS IS PUSHING IN THE BONE ARRAY BEFORE WE CREATE THE HIERARCHY???
			//fill our array of vertices with bones and weights
			/*for (i = 0; i < mesh.mBones[j]->mNumWeights; i++) {
				//ret.skeleton_vertices[mesh.mBones[j]->mWeights[i].mVertexId].bone_array.push_back(*(mesh.mBones[j]));
				ret.skeleton_vertices[mesh.mBones[j]->mWeights[i].mVertexId].weight_array.push_back(mesh.mBones[j]->mWeights[i].mWeight);
			}*/
		}
		
		//construct the hierarchy of bones,starting with the root bone
		for (i = 0; i < root->mNumChildren; ++i) {
			CreateHierarchy(&boneNames, root->mChildren[i], NULL, &iter, ret.bone_array);
		}
		/**********************MODIFIED**********************/
		for(j = 0; j < mesh.mNumBones; j++) {
			for (i = 0; i < mesh.mBones[j]->mNumWeights; i++) {
				for (int k = 0; k < ret.boneCt; ++k) {
					if(mesh.mBones[j]->mName == ret.bone_array[k].name) {
						ret.skeleton_vertices[mesh.mBones[j]->mWeights[i].mVertexId].bone_array.push_back(ret.bone_array[k]);
						//printf("actually made it inside?\n");
					}
				}
				ret.skeleton_vertices[mesh.mBones[j]->mWeights[i].mVertexId].weight_array.push_back(mesh.mBones[j]->mWeights[i].mWeight);
			}
		}
		/*****************END**MODIFIED**********************/
		printf("anim name: %s\n", scene->mAnimations[0]->mName.C_Str());
		for(i = 0; i < scene->mAnimations[0]->mNumChannels; i++) {
			printf("name of %d's aiNodeAnim: %s; numPositionKeys: %d; numRotationKeys: %d; numScalingKeys: %d\n",i, scene->mAnimations[0]->mChannels[i]->mNodeName.C_Str(), 
			 scene->mAnimations[0]->mChannels[i]->mNumPositionKeys, scene->mAnimations[0]->mChannels[i]->mNumRotationKeys, scene->mAnimations[0]->mChannels[i]->mNumScalingKeys);
		}

/*		//check the vertices data
		for (i = 0; i < mesh.mNumVertices; i++) {
			if (i%100 == 0) {
				std::cout << "The position for verex " << i << " is ("<< ret.skeleton_vertices[i].position.x << ", " << ret.skeleton_vertices[i].position.y << ", " << ret.skeleton_vertices[i].position.z << ")" << std::endl;
				for (j = 0; j < ret.skeleton_vertices[i].weight_array.size(); j++) {
					printf("    Bone %s has the weight of %lf\n", ret.skeleton_vertices[i].bone_array[j].mName.C_Str(), ret.skeleton_vertices[i].weight_array[j]);
				}
			}
		}*/
	}
	/*
	for(i = 0; i < ret.boneCt; ++i) {
		printf("Bone Name: %s\n", mesh.mBones[i]->mName.C_Str());
	}
	
	for (i = 0; i < ret.boneCt; ++i) {
		if (ret.bone_array[i].parent == NULL) {
			printf("Bone %s has the parent NULL\n", ret.bone_array[i].name.C_Str());
		} else {	
			printf("Bone %s has the parent %s\n", ret.bone_array[i].name.C_Str(), ret.bone_array[i].parent->name.C_Str());
		}
	}*/

    return ret;
}

int CreateHierarchy(std::vector<aiBone *> *boneNames, aiNode *root, Bone *parent, int *iter, Bone *array) {
	int hold = *iter;
	
	//check to see if it is a bone by going through our vector of bone names
	for (std::vector<aiBone *>::iterator it = boneNames->begin() ; it != boneNames->end(); ++it) {
		//if its name matches a bone's name then it is a bone
		if ((*it)->mName == root->mName) {
			//we store that bone's info
			array[hold].offset = (*it)->mOffsetMatrix;
			array[hold].parent = parent;
			array[hold].name = (*it)->mName;
			//increment the spot in the array
			++(*iter);
			//then try to add any bones below it to the hierarchy
			for (int i = 0; i < root->mNumChildren; ++i) {
				//because the root is the bone, we make its location in the array the parent for any future bones
				CreateHierarchy(boneNames, root->mChildren[i], array + hold, iter, array);
			}
			//we are done here
			return 0;
		}
	}

	//if it is not found in the bone names vector, then you get here. We need to see if the other childeren are bones to add to the hierarchy
	for (int i = 0; i < root->mNumChildren; ++i) {
		//because this root is not a bone, we keep the location of any potential bone's parent the same
		CreateHierarchy(boneNames, root->mChildren[i], parent, iter, array);
	}
	
	return 1;
}
