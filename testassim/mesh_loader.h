#ifndef MESH_LOADER_H_
#define MESH_LOADER_H_

#include <string>
#include <vector>

struct aiScene;

struct AssimpMesh {
   std::vector<float> vertex_array;
   std::vector<float> normal_array;
   std::vector<float> uv_array;
   std::vector<unsigned short> index_array;
};

AssimpMesh loadMesh(const std::string& path);

#endif // MESH_LOADER_H_
