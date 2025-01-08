#include "camera.hpp"

namespace Ignition::IO {
   namespace Keys {
      const int space         = ' ';
      const int apostrophe    = '\'';
      const int comma         = ',';
      const int minus         = '-';
      const int period        = '.';
      const int slash         = '/';

      const int zero          = '0';
      const int one           = '1';
      const int two           = '2';
      const int three         = '3';
      const int four          = '4';
      const int five          = '5';
      const int six           = '6';
      const int seven         = '7';
      const int eight         = '8';
      const int nine          = '9';

      const int semicolon     = ';';
      const int equal         = '=';

      const int a             = 'A';
      const int b             = 'B';
      const int c             = 'C';
      const int d             = 'D';
      const int e             = 'E';
      const int f             = 'F';
      const int g             = 'G';
      const int h             = 'H';
      const int i             = 'I';
      const int j             = 'J';
      const int k             = 'K';
      const int l             = 'L';
      const int m             = 'M';
      const int n             = 'N';
      const int o             = 'O';
      const int p             = 'P';
      const int q             = 'Q';
      const int r             = 'R';
      const int s             = 'S';
      const int t             = 'T';
      const int u             = 'U';
      const int v             = 'V';
      const int w             = 'W';
      const int x             = 'X';
      const int y             = 'Y';
      const int z             = 'Z'; 

      const int up            = '\265';
      const int down          = '\264';
      const int left          = GLFW_KEY_LEFT;
      const int right         = '\262';

      const int enter         = '\n';
      const int tab           = '\t';
      const int lshift        = '\340';
      const int rshift        = '\344';

      const int esc          = '\256';
   }

   extern float scrollStatusX, scrollStatusY;
   void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);

   float GetScrollX();
   float GetScrollY();

   extern std::unordered_map<int, bool> keyStates;

   bool GetInput(int key);
   bool GetInputDown(int key);
   bool GetInputUp(int key);  

   bool GetMouse(int key);
   bool GetMouseDown(int key);
   bool GetMouseUp(int key); 

   static Ignition::Vector2 GetMousePosition() { double a,b; glfwGetCursorPos((GLFWwindow*)*Ignition::mainCamera->window, &a, &b); return {a,b}; }

   void LockMouse(bool lock);
   void HideMouse(bool hide);

   static float GetAxisHorizontal() {
      float f = 0;
      f += GetInput(Keys::a);
      f -= GetInput(Keys::d);
      return f;
   }

   static float GetAxisVertical() {
      float f = 0;
      f += GetInput(Keys::w);
      f -= GetInput(Keys::s);
      return f;
   }
}

