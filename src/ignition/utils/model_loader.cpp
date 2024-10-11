#include "utils/files.hpp"
#include "utils/model_loader.hpp"
#include "types/model.hpp"
#include "modules/rendering/meshrenderer.hpp"
#include "scene.hpp"

#include <string>
#include <iostream>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

namespace Ignition::ModelLoader {
   int LoadModel(std::string path) {

     
     std::vector<Object> o;
     Assimp::Importer importer;
     const aiScene *scene = importer.ReadFile(
         path.data(),
         aiProcess_Triangulate | aiProcess_GenNormals |
         aiProcess_FlipUVs);
   
     if (!scene) {
       std::cerr << importer.GetErrorString() << "\n";
     }

     int parent = Ignition::scene.CreateObject();
     Ignition::scene.GetObjects()->at(parent).name = scene->mName.data;
   
     if (scene->HasMeshes()) {
       for (unsigned int i = 0; i < scene->mNumMeshes; ++i) {
         Object obj;
         Model m;
         const aiMesh *mesh = scene->mMeshes[i];
   
         m.vertices.clear();
         m.uv.clear();
         m.normals.clear();
         m.indices.clear();
   
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

         Ignition::Rendering::MeshRenderer mr;
         mr.model = m;

         std::shared_ptr<Rendering::MeshRenderer> ptr = std::make_shared<Rendering::MeshRenderer>(m);
         obj.AddModule(ptr);

         Ignition::scene.GetObjects()->at(parent).AddChild(&obj);
       }
     }
   
     return 0;
   }
} // namespace Ignition::ModelLoader
