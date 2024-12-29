#ifndef IGNITION_NO_ASSIMP

#include "utils/files.hpp"
#include "utils/model_loader.hpp"
#include "types/model.hpp"
#include "modules/rendering/meshrenderer.hpp"
#include "scene.hpp"
#include "utils/io.hpp"

#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Ignition::ModelLoader {
   Model LoadModel(std::string path) {
      Model m;
      Assimp::Importer importer;
      const aiScene *scene = importer.ReadFile(
         path.data(),
         aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_GenUVCoords 
         );
   
      if (!scene) {
         Ignition::IO::Error(importer.GetErrorString());
     }

     if (scene->HasMeshes()) {
       for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
         const aiMesh *mesh = scene->mMeshes[i];
   
         for (unsigned int v = 0; v < mesh->mNumVertices; ++v) {
           m.vertices.push_back(mesh->mVertices[v].x);
           m.vertices.push_back(mesh->mVertices[v].y);
           m.vertices.push_back(mesh->mVertices[v].z);

           if (mesh->HasTextureCoords(0)) {
             m.uv.push_back(mesh->mTextureCoords[0][v].x);
             m.uv.push_back(mesh->mTextureCoords[0][v].y);
             m.uv.push_back(mesh->mTextureCoords[0][v].z);
           } else {
             m.uv.push_back(0);
             m.uv.push_back(0);
             m.uv.push_back(0);
           }
   
           if (mesh->HasNormals()) {
             m.normals.push_back(mesh->mNormals[v].x);
             m.normals.push_back(mesh->mNormals[v].y);
             m.normals.push_back(mesh->mNormals[v].z);
           } else {
             m.normals.push_back(0);
             m.normals.push_back(0);
             m.normals.push_back(0);
           }
         }
         for (unsigned int v = 0; v < mesh->mNumFaces; ++v)
         {
            aiFace face = mesh->mFaces[v];
            for (unsigned int ind = 0; ind < face.mNumIndices; ++ind) {
               m.indices.push_back(face.mIndices[ind]);
            }
         }
         m.path = path.substr(Ignition::IO::GetProjectHome().size(), path.size());
         m.name = mesh->mName.C_Str() + i == 0 ? "" : std::to_string(i);
       }
     }

     /* used for ico_sphere, was originally generated in blender

     std::cout << "\t.indices = {\n";
     for (int i = 0; i < (int)m.indices.size(); i += 3) {
        std::cout << "\t\t" +
                     std::to_string(m.indices[i]) + ", " 
                   + std::to_string(m.indices[i+1]) + ", "
                   + std::to_string(m.indices[i+2]) + ",\n";
     }
     std::cout << "},\n";
     
     std::cout << "\t.vertices = {\n";
     for (int i = 0; i < (int)m.vertices.size(); i += 3) {
        std::cout << "\t\t" +
                     std::to_string(m.vertices[i]) + ", " 
                   + std::to_string(m.vertices[i+1]) + ", "
                   + std::to_string(m.vertices[i+2]) + ",\n";
     }
     std::cout << "},\n";
   
     std::cout << "\t.normals = {\n";
     for (int i = 0; i < (int)m.normals.size(); i += 3) {
        std::cout << "\t\t" +
                     std::to_string(m.normals[i]) + ", " 
                   + std::to_string(m.normals[i+1]) + ", "
                   + std::to_string(m.normals[i+2]) + ",\n";
     }
     std::cout << "},\n";
     
     std::cout << "\t.uv = {\n";
     for (int i = 0; i < (int)m.uv.size(); i += 3) {
        std::cout << "\t\t" +
                     std::to_string(m.uv[i]) + ", " 
                   + std::to_string(m.uv[i+1]) + ", "
                   + std::to_string(m.uv[i+2]) + ",\n";
     }
     std::cout << "},\n";

     */
     return m;
   }
} // namespace Ignition::ModelLoader
  
#endif
