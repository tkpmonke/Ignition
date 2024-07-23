#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace Overload {
   class Window {
   public:
      Window() = default;
      Window(int, int, const char*);
      
      bool IsOpen();

      void Update();
      
      void Shutdown();

      operator GLFWwindow*() const { return this->window; };

   private:
      GLFWwindow* window;
   };
}
