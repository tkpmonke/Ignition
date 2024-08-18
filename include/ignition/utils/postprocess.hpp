#include "types/shader.hpp"
#include "window.hpp"

namespace Ignition::Rendering {
   class PostProcess : public Shader {
   public:
      PostProcess(std::string data, Window* window);
      PostProcess() = default;
   private:
      Window* window;
   };
}
