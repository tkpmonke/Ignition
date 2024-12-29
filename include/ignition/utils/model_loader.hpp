#include <object.hpp>
#include <types/model.hpp>

#ifndef IGNITION_NO_ASSIMP
namespace Ignition::ModelLoader {
   Model LoadModel(std::string path);
}
#endif
