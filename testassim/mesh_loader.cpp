#include "mesh_loader.h"
#include "Mesh.h"

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

	/*this part sets up the mesh like a normal, non-animated mesh*/
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
/**********************We*Get*Animating*Here***********************************/
	int i, j;
	ret.skeleton_vertices = (vertexInfo *)calloc(sizeof(vertexInfo), mesh.mNumVertices);
	ret.numVerts = mesh.mNumVertices;
		
	//fill all the positions for the skeleton_vertices with the actual vertices
	for (i = 0; i < mesh.mNumVertices; i++) {
		ret.skeleton_vertices[i].position = mesh.mVertices[i];
	}

	//store whether it has bones or not. If it does, then we are dealing with an animated model and we go in the if
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
		
		//find the root node
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
		}
		
		//construct the hierarchy of bones,starting with the root node. It will go to each of the children adding any bone its finds to the hierarchy
		CreateHierarchy(&boneNames, root, NULL, &iter, ret.bone_array);

		/*Here is where we actually fill our array of vertices with bones and weights*/
		for(j = 0; j < mesh.mNumBones; j++) {
			//go through each weight in each bone
			for (i = 0; i < mesh.mBones[j]->mNumWeights; i++) {
				//find a bone it may match
				for (int k = 0; k < ret.boneCt; ++k) {
					//fill with bones
					if(mesh.mBones[j]->mName == ret.bone_array[k].name) {
						//fill the bone
						ret.skeleton_vertices[mesh.mBones[j]->mWeights[i].mVertexId].bone_array.push_back(k);
						//fill the weights. There is a max of 4 weights
						ret.skeleton_vertices[mesh.mBones[j]->mWeights[i].mVertexId].weight_array.push_back(mesh.mBones[j]->mWeights[i].mWeight);
						k = ret.boneCt;
					}
				}
				
			}
		}
		
		
		/*****************GRAB KEY FRAMES******************/
		//go through all the animations (brace your efficiency organs for sadness)
		for (i = 0; i < scene->mNumAnimations; i++) {
			//go through each channel of each animation
			for (j = 0; j < scene->mAnimations[i]->mNumChannels; j++) {
				//go through each bone and see if it matches the node in the animation
				for(int k = 0; k < ret.boneCt; k++) {
					//if there is a bone whos name matches a node in the animation, then we must give it the key frames
					if (ret.bone_array[k].name == scene->mAnimations[i]->mChannels[j]->mNodeName) {
						//this variable is a sanity check :D
						aiNodeAnim *nodeHold = scene->mAnimations[i]->mChannels[j];
						
						//set up the number of key frames per transform. Though they should all be the same or badness ensues in our code
						ret.bone_array[k].numRotKeyFrames = nodeHold->mNumRotationKeys;
						ret.bone_array[k].numPosKeyFrames = nodeHold->mNumPositionKeys;
						ret.bone_array[k].numScaleKeyFrames = nodeHold->mNumScalingKeys;
						//allocate space to hold the keyframe transforms
						ret.bone_array[k].posKeys = (aiVectorKey *)calloc(sizeof(aiVectorKey), nodeHold->mNumPositionKeys);
						ret.bone_array[k].rotKeys = (aiQuatKey *)calloc(sizeof(aiQuatKey), nodeHold->mNumRotationKeys);
						ret.bone_array[k].scaleKeys = (aiVectorKey *)calloc(sizeof(aiVectorKey), nodeHold->mNumScalingKeys);
						
						//get all the position transforms
						for (int l = 0; l < nodeHold->mNumPositionKeys; l++) {
							ret.bone_array[k].posKeys[l] = nodeHold->mPositionKeys[l];
						}
						//get all the rotation transforms
						for (int l = 0; l < nodeHold->mNumRotationKeys; l++) {
							ret.bone_array[k].rotKeys[l] = nodeHold->mRotationKeys[l];
						}
						//get all the scaling transforms	
						for (int l = 0; l < nodeHold->mNumScalingKeys; l++) {
							ret.bone_array[k].scaleKeys[l] = nodeHold->mScalingKeys[l];
						}
						
						//get out of the loop
						k = ret.boneCt;
					}
				}
			}
		}
		
		
		
		/********************Save the Root bone*****************************************************************************************/
		//go through each bone
		for (i = 0; i < ret.boneCt; i++) {
			//store the root bone
			if (ret.bone_array[i].parent == NULL)
				ret.root = ret.bone_array +i;
		}		
		/********************Transforms done here*************************************************************************************************/
		//setup the transforms hierarchically. Starting with the root and going down
		setupTrans(ret.bone_array, NULL, ret.root);		
		
		/**************************************************/

		/****************DEBUGGING*************************/
		/*for(i = 0; i < ret.boneCt; ++i) {
			
			if (i < 30) {
				printf("Bone %d: \n", i);
				for (j = 0; j < 4; ++j) {
					for (int k = 0; k < 4; ++k) {
						printf("%.3lf, ", ret.bone_array[i].glmTransforms[0][j][k]);
					}
					printf("\n");
				}
			
				printf("\n");
			}
		}*/
		
		/*
		
		for (i = 0; i < mesh.mNumVertices; i++) {
			for (j = 0; j < ret.skeleton_vertices[i].weight_array.size(); j++) {
				if (ret.skeleton_vertices[i].weight_array[j] > 1 || ret.skeleton_vertices[i].weight_array[j] < 0)
					printf("Bad weight at %d\n", i);
			}
		}
		
		printf("anim name: %s\n", scene->mAnimations[0]->mName.C_Str());
		for(i = 0; i < scene->mAnimations[0]->mNumChannels; i++) {
			printf("name of %d's aiNodeAnim: %s; numPositionKeys: %d; numRotationKeys: %d; numScalingKeys: %d\n",i, scene->mAnimations[0]->mChannels[i]->mNodeName.C_Str(), 
			 scene->mAnimations[0]->mChannels[i]->mNumPositionKeys, scene->mAnimations[0]->mChannels[i]->mNumRotationKeys, scene->mAnimations[0]->mChannels[i]->mNumScalingKeys);
		}

	//check the vertices data
		for (i = 0; i < mesh.mNumVertices; i++) {
			if (i%100 == 0) {
				std::cout << "The position for vertex " << i << " is ("<< ret.skeleton_vertices[i].position.x << ", " << ret.skeleton_vertices[i].position.y << ", " << ret.skeleton_vertices[i].position.z << ")" << std::endl;
			}
		}*/
	}
	
	//checking vertex counts
	//printf("Vertex Count~ In Bone Array: %d, in Mesh: %d\n", ret.vertex_array.size()/3, ret.numVerts);
	/*
	for(i = 0; i < ret.boneCt; ++i) {
		printf("Bone Name: %s\n", mesh.mBones[i]->mName.C_Str());
	}
	*/
	for (i = 0; i < ret.boneCt; ++i) {
		if (ret.bone_array[i].parent == NULL) {
			printf("Bone %s has the parent NULL\n", ret.bone_array[i].name.C_Str());
		} else {	
			printf("Bone %s has the parent %s\n", ret.bone_array[i].name.C_Str(), ret.bone_array[i].parent->name.C_Str());
		}
	}
	
	for (i = 0; i < ret.boneCt; ++i) {
		if (ret.bone_array[i].childs.size() == 0)
			printf("Bone %s has no childeren\n", ret.bone_array[i].name.C_Str());
			
		for (j = 0; j < ret.bone_array[i].childs.size(); j++)
			printf("Bone %s has the child %s\n", ret.bone_array[i].name.C_Str(), ret.bone_array[i].childs[j]->name.C_Str());
	}
	
		/**************************************************/

    return ret;
}

int setupTrans(Bone *array, Bone *parent, Bone *cur) {
	int i;
	aiMatrix4x4t<float> translateM, rotateM, scaleM;
	
	//setup the transforms hierarchically. Starting with the root and going down
	//allocate the space for the transforms first
	cur->transformations = (aiMatrix4x4 *)calloc(sizeof(aiMatrix4x4), cur->numPosKeyFrames);
	cur->glmTransforms = (glm::mat4 *)calloc(sizeof(glm::mat4), cur->numPosKeyFrames);
	cur->personalTrans = (aiMatrix4x4 *)calloc(sizeof(aiMatrix4x4), cur->numPosKeyFrames);
	
	//set the bone's transformation to its parent's transformation (without the bone offset) * it's own transformation (without the bone offset) * the bone offset
	for (i = 0; i < cur->numPosKeyFrames; i++) {
		//set up the bone's own transformation, for each bone we have, we will need to store their personal transform. This is their  translate*rotate*scale transform per keyframe
		aiMatrix4x4::Translation(cur->posKeys[i].mValue, translateM);
		rotateM = aiMatrix4x4(cur->rotKeys[i].mValue.GetMatrix());
		aiMatrix4x4::Scaling(cur->scaleKeys[i].mValue, scaleM);
		cur->personalTrans[i] = translateM * rotateM * scaleM;
		
		//a check to make sure we aren't the root
		if (parent == NULL) {
			cur->transformations[i] = cur->personalTrans[i] * cur->offset;
		}
		else
			cur->transformations[i] = parent->personalTrans[i] * cur->personalTrans[i] * cur->offset;
		
		//store the matrix as a glm mat4	
		CopyaiMat(cur->transformations + i, cur->glmTransforms[i]);	
	}
	
	//call setup transform on all the children
	for (i = 0; i < cur->childs.size(); i++)
		setupTrans(array, cur, cur->childs[i]);
		
	return 0;
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
			//give the parent the child
			if (parent != NULL) {
				parent->childs.push_back(array + hold);
			}
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

//Borrowed from http://ephenationopengl.blogspot.com/2012/06/doing-animations-in-opengl.html
//Since Assimp stores matrices in row major and glm stores matrices in column major, we need to do some converting
void CopyaiMat(const aiMatrix4x4 *from, glm::mat4 &to) {
	to[0][0] = from->a1; to[1][0] = from->a2; to[2][0] = from->a3; to[3][0] = from->a4;
	to[0][1] = from->b1; to[1][1] = from->b2; to[2][1] = from->b3; to[3][1] = from->b4;
	to[0][2] = from->c1; to[1][2] = from->c2; to[2][2] = from->c3; to[3][2] = from->c4;
	to[0][3] = from->d1; to[1][3] = from->d2; to[2][3] = from->d3; to[3][3] = from->d4;
}
