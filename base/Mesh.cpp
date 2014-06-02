#define GL_GLEXT_PROTOTYPES
#include "GLSL_helper.h"
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
    : PositionHandle(0), NormalHandle(0), IndexHandle(0), IndexBufferLength(0), JointHandle(0), WeightHandle(0), WeightCount(0)
{
  std::vector<float> pos, weights, joints, weightCtr;
  
  for (int i = 0; i < aMesh.numVerts; ++i) {
    pos.push_back((float) aMesh.skeleton_vertices[i].position.x);
    pos.push_back((float) aMesh.skeleton_vertices[i].position.y);
    pos.push_back((float) aMesh.skeleton_vertices[i].position.z);
    
    weightCtr.push_back((float) aMesh.skeleton_vertices[i].weight_array.size());
    
    printf("vertex %d: ", i);
    for (int j = 0; j < aMesh.skeleton_vertices[i].weight_array.size(); j++) {
      weights.push_back((float) aMesh.skeleton_vertices[i].weight_array[j]);
      printf("weight %d: %lf ", j, aMesh.skeleton_vertices[i].weight_array[j]);
    }
    printf("\n");
    
    for (int j = 0; j < aMesh.skeleton_vertices[i].bone_array.size(); j++)
      joints.push_back((float) aMesh.skeleton_vertices[i].bone_array[j]);
  }
      
  /*for (int i = 0; i < aMesh.boneCt; i++) {
    printf("Matrix %d is: \n", i);
    for (int j = 0; j < 4; ++j) {
      for (int k = 0; k < 4; k++) {
        printf("%lf, ", aMesh.bone_array[i]->glmTransforms[0][j][k]);
      }
      printf("\n");
    }
    printf("\n");
  }*/
    
  hasAss = true;
  Assimp = aMesh;
  IndexBufferLength = aMesh.index_array.size();
    
  glGenBuffers(1, & JointHandle);
  glBindBuffer(GL_ARRAY_BUFFER, JointHandle);
  glBufferData(GL_ARRAY_BUFFER, joints.size() * sizeof(float), & joints.front(), GL_STATIC_DRAW);

  glGenBuffers(1, & WeightCount);
  glBindBuffer(GL_ARRAY_BUFFER, WeightCount);
  glBufferData(GL_ARRAY_BUFFER, weightCtr.size() * sizeof(float), & weightCtr.front(), GL_STATIC_DRAW);

  glGenBuffers(1, & WeightHandle);
  glBindBuffer(GL_ARRAY_BUFFER, WeightHandle);
  glBufferData(GL_ARRAY_BUFFER, weights.size() * sizeof(float), & weights.front(), GL_STATIC_DRAW);
  
  glGenBuffers(1, & PositionHandle);
  glBindBuffer(GL_ARRAY_BUFFER, PositionHandle);
  glBufferData(GL_ARRAY_BUFFER, pos.size() * sizeof(float), & pos.front(), GL_STATIC_DRAW);

  glGenBuffers(1, & NormalHandle);
  glBindBuffer(GL_ARRAY_BUFFER, NormalHandle);
  glBufferData(GL_ARRAY_BUFFER, aMesh.normal_array.size() * sizeof(float), & aMesh.normal_array.front(), GL_STATIC_DRAW);

  glGenBuffers(1, & IndexHandle);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IndexHandle);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, aMesh.index_array.size() * sizeof(unsigned short), & aMesh.index_array.front(), GL_STATIC_DRAW);

//
  
//
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
void PlaceModel(Mesh mesh, float locx, float locy, float locz, float sx, float sy, float sz, float angle, float rad, int frame) {

  mat4 Mod = SetModel(locx, locy, locz, sx, sy, sz, angle);
   
  if (checkViewFrustum (glm::vec3 (0,0,0), rad, curProj*curView*Mod) == 0) {
    if (mesh.hasAss) {
      static glm::mat4 boneArr[30];
      int ctr = 0;

      GetFrame(frame, mesh.Assimp);
      
      /*for (int i = 0; i < mesh.Assimp.boneCt; i++) {
        printf("Matrix %d is: \n", i);
        for (int j = 0; j < 4; ++j) {
          for (int k = 0; k < 4; k++) {
            printf("%lf, ", mesh.Assimp.bone_array[i]->currentTransform[j][k]);
          }
          printf("\n");
        }
        printf("\n");
      }*/
      
      for (int i = 0; i < mesh.Assimp.boneCt; i++) {
        boneArr[i] = mesh.Assimp.bone_array[i]->glmTransforms[frame];
      }
      
      
      glUniformMatrix4fv(h_uBoneMatrix, 30*16, GL_FALSE, (GLfloat *)boneArr);
      
      glUniform1i(h_uAnimFlag, 1);
      
      glEnableVertexAttribArray(h_uJoints);
      glBindBuffer(GL_ARRAY_BUFFER, mesh.JointHandle);
      glVertexAttribPointer(h_uJoints, 4, GL_FLOAT, GL_FALSE, 0, 0);
      
      glEnableVertexAttribArray(h_uWeights);
      glBindBuffer(GL_ARRAY_BUFFER, mesh.WeightHandle);
      glVertexAttribPointer(h_uWeights, 4, GL_FLOAT, GL_FALSE, 0, 0);
      
      glEnableVertexAttribArray(h_uNumWeights);
      glBindBuffer(GL_ARRAY_BUFFER, mesh.WeightCount);
      glVertexAttribPointer(h_uNumWeights, 1, GL_FLOAT, GL_FALSE, 0, 0);
      
    } else {
      glUniform1i(h_uAnimFlag, 0);
    }
  
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
Input: takes in a Mesh, and everything you would ever want to do to it. The angle is rotation around the y axis

draws the model :D AWW YEAH!

Output: YOU GET NOTHING!
******************************************************************************/
void PlaceModel(Mesh mesh, float locx, float locy, float locz, float sx, float sy, float sz, float angle, float rad) {
   mat4 Mod = SetModel(locx, locy, locz, sx, sy, sz, angle);
   
   //if (rad < 0)
    //printf("rad: %f\n", rad);
   
  if (checkViewFrustum (glm::vec3 (0,0,0), rad, curProj*curView*Mod) == 0) {
     glUniform1i(h_uAnimFlag, 0);
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
Input: translation x, y. Scaling x, y.

Sends the model view matrix to GLSL. The model view matrix is made off the inputs. order of scale, Rotate, then translate.

Output: The model matrix you set
******************************************************************************/
glm::mat4 SetModel(float x, float y, float Sx, float Sy) {
   glm::mat4 Trans = glm::translate( glm::mat4(1.0f), glm::vec3(x, y, -1.0));
   glm::mat4 Scale = glm::scale(glm::mat4(1.0f), glm::vec3(Sx, Sy, 1.0));
   glm::mat4 ctm = Trans * Scale;
   safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ctm));

   return ctm;
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