#include "gui.h"
#include "projects.h"

int main() {
   load_projects();
   initilize_window();
   initilize_gui();
   while (update_window()) {
      render_gui();
      present_window();
   }
   shutdown_gui();
   shutdown_window();
   return 0;
}
