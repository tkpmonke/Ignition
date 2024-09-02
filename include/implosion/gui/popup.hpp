#include <functional>

namespace Implosion {
   class Popup {
   public:
      const char* name;
      void* render;

      Popup() = default;
      Popup(const char* name,  void* render) : name(name),render(render) {}
   };
}
