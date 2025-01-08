#include "GL/glew.h"

#include <stdio.h>

#include "gui.h"
#include "projects.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_GLFW_GL3_IMPLEMENTATION
#define NK_KEYSTATE_BASED_INPUT
#include "nuklear_glfw_gl3.h"

#define MAX_VERTEX_BUFFER 512 * 1024
#define MAX_ELEMENT_BUFFER 128 * 1024

#define SIDE_PANEL_PERCENT_OF_SCREEN 6
#define SPPOS SIDE_PANEL_PERCENT_OF_SCREEN

#define TOP_PANEL_SIZE 40

struct nk_glfw glfw = {0};
struct nk_context *ctx;
struct nk_colorf bg;

void set_style();

void render_toppanel();
void render_sidepanel();
void render_projects();
void render_create_project();
void render_add_project();

int open_create_project = 0;
int open_add_project = 0;

void initilize_gui() {

   ctx = nk_glfw3_init(&glfw, window, NK_GLFW3_INSTALL_CALLBACKS);

   struct nk_font_atlas *atlas;
   nk_glfw3_font_stash_begin(&glfw, &atlas);
   nk_glfw3_font_stash_end(&glfw);
   
   set_style();
}

void render_gui() {
   nk_glfw3_new_frame(&glfw);
   
   int width, height;

   glfwGetFramebufferSize(window, &width, &height);

   if (nk_begin(ctx, "TopPanel", nk_rect(0, 0, (float)width, TOP_PANEL_SIZE),
         NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR | NK_WINDOW_BACKGROUND)) {
      render_toppanel();
   }
   nk_end(ctx);

   if (nk_begin(ctx, "SidePanel", nk_rect(0, TOP_PANEL_SIZE, (float)width/SPPOS, height),
         NK_WINDOW_BORDER | NK_WINDOW_BACKGROUND)) {
      render_sidepanel();
   }
   nk_end(ctx);

   if (nk_begin(ctx, "Projects", nk_rect((float)width/SPPOS, TOP_PANEL_SIZE, (float)width/SPPOS*(SPPOS-1), height),
         NK_WINDOW_BORDER | NK_WINDOW_BACKGROUND)) {
      render_projects();
   }
   nk_end(ctx);

   if (open_create_project) 
      render_create_project();

   if (open_add_project)
      render_add_project();

   nk_glfw3_render(&glfw, NK_ANTI_ALIASING_ON, MAX_VERTEX_BUFFER, MAX_ELEMENT_BUFFER);
}

void shutdown_gui() {
   nk_glfw3_shutdown(&glfw);
}

// actual render code

void render_toppanel() {
   nk_layout_row_dynamic(ctx, 30, 1);
   nk_label(ctx, "Implosion Hub", NK_TEXT_CENTERED);
}

void render_sidepanel() {
   nk_layout_row_dynamic(ctx, 30, 1);
   if (nk_button_label(ctx, "Create Project")) {
      open_create_project = 1;
   }

   nk_layout_row_dynamic(ctx, 30, 1);
   if (nk_button_label(ctx, "Add Local Project")) {
      open_add_project = 1;
   }
}

void render_projects() {
   nk_layout_row_dynamic(ctx, 30, 7);
   nk_label(ctx, "Project Name", NK_TEXT_LEFT);
   nk_label(ctx, "Project Path", NK_TEXT_LEFT);
   nk_spacing(ctx, 3);
   nk_label(ctx, "Last Modified", NK_TEXT_LEFT);
   nk_label(ctx, "Size", NK_TEXT_LEFT);

   for (int i = 0; i < project_count; ++i) {
      nk_layout_row_dynamic(ctx, 40, 1);
      int b = 0;
      if (nk_group_begin(ctx, "Project", NK_WINDOW_BORDER | NK_WINDOW_NO_SCROLLBAR)) {

         if (nk_input_is_mouse_hovering_rect(&ctx->input, ctx->current->layout->bounds)) {
            nk_style_push_color(ctx, &ctx->style.text.color, nk_rgba(50, 50, 50, 255));
            b = 1;

            if (nk_input_is_mouse_click_in_rect(&ctx->input, NK_BUTTON_LEFT, ctx->current->layout->bounds)) {
               run_project(projects[i].path);
            }
         }

         nk_layout_row_dynamic(ctx, 30, 7);
         nk_label(ctx, projects[i].name, NK_TEXT_LEFT);
         nk_label(ctx, projects[i].path, NK_TEXT_LEFT);
         nk_spacing(ctx, 3);

         char time[100];
         strftime(time, sizeof(time), "%m-%d-%Y %I:%M:%S %p", localtime(&projects[i].lastModified));
         nk_label(ctx, time, NK_TEXT_LEFT);
         
         const char* units[] = {"B", "KB", "MB", "GB", "TB"};
         int unit_index = 0;

         double readable_size = (double)projects[i].size;
         while (readable_size >= 1024 && unit_index < 4) {
            readable_size /= 1024;
            unit_index++;
         }

         char size[100];
         snprintf(size, sizeof(size), "%.2f %s", readable_size, units[unit_index]);
         nk_label(ctx, size, NK_TEXT_LEFT);
         nk_group_end(ctx);

         if (b == 1) {
            nk_style_pop_color(ctx);
            b = 0;
         }
      }

   }
}

void render_create_project() {
   if (nk_begin(ctx, "Create Project", nk_rect(200, 200, 400, 700),
         NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE)) {
      nk_layout_row_dynamic(ctx, 30, 1);
      nk_label(ctx, "Create Project", NK_TEXT_CENTERED);

      nk_layout_row_dynamic(ctx, 30, 1);
      nk_label(ctx, "Project Name", NK_TEXT_LEFT);
      static char project_name[100];
      static int project_name_len = 0;
      nk_edit_string(ctx, NK_EDIT_FIELD, project_name, &project_name_len, 100, nk_filter_default);
      project_name[project_name_len] = '\0';
      
      nk_layout_row_dynamic(ctx, 30, 1);
      nk_label(ctx, "Developer Name", NK_TEXT_LEFT);
      static char dev_name[100];
      static int dev_name_len = 0;
      nk_edit_string(ctx, NK_EDIT_FIELD, dev_name, &dev_name_len, 100, nk_filter_default);
      dev_name[dev_name_len] = '\0';

      nk_layout_row_dynamic(ctx, 30, 1);
      nk_label(ctx, "Project Path", NK_TEXT_LEFT);
      static char project_path[300];
      static int project_path_len = 0;
      nk_edit_string(ctx, NK_EDIT_SIMPLE, project_path, &project_path_len, 300, nk_filter_default);
      project_path[project_path_len] = '\0';

      nk_layout_row_dynamic(ctx, 30, 1);
      char full_path[400] = {0};
      sprintf(full_path, "%s%s%s", project_path, project_path[project_path_len-1] == '/' ? "" : "/", project_name);
      nk_label(ctx, full_path, NK_TEXT_LEFT);

      nk_layout_row_dynamic(ctx, 30, 1);
      if (nk_button_label(ctx, "Browse For Project Path")) {
         char* path = file_explorer_for_project();
         if (path != NULL) {
            strcpy(project_path, path);
            project_path_len = strlen(project_path);
            free(path);
         }
      }

      nk_layout_row_dynamic(ctx, 30, 2);
      if (nk_button_label(ctx, "Cancel")) {
         open_create_project = 0;
      } if (nk_button_label(ctx, "Create")) {
         create_project(project_name, full_path, dev_name);
         open_create_project = 0;
      }
   }
   nk_end(ctx);
}

void render_add_project() {
   if (nk_begin(ctx, "Add Project", nk_rect(200, 200, 400, 700),
         NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_TITLE)) {
      nk_layout_row_dynamic(ctx, 30, 1);
      nk_label(ctx, "Add Project", NK_TEXT_CENTERED);

      nk_layout_row_dynamic(ctx, 30, 1);
      nk_label(ctx, "Project Name", NK_TEXT_LEFT);
      static char project_name[100];
      static int project_name_len = 0;
      nk_edit_string(ctx, NK_EDIT_FIELD, project_name, &project_name_len, 100, nk_filter_default);
      project_name[project_name_len] = '\0';

      nk_layout_row_dynamic(ctx, 30, 1);
      nk_label(ctx, "Project Path", NK_TEXT_LEFT);
      static char project_path[300];
      static int project_path_len = 0;
      nk_edit_string(ctx, NK_EDIT_SIMPLE, project_path, &project_path_len, 300, nk_filter_default);
      project_path[project_path_len] = '\0';

      nk_layout_row_dynamic(ctx, 30, 1);
      if (nk_button_label(ctx, "Browse For Project Path")) {
         char* path = file_explorer_for_project();
         if (path != NULL) {
            strcpy(project_path, path);
            project_path_len = strlen(project_path);
            free(path);
         }
      }

      nk_layout_row_dynamic(ctx, 30, 2);
      if (nk_button_label(ctx, "Cancel")) {
         open_add_project = 0;
      } if (nk_button_label(ctx, "Add")) {

         add_project_to_projects_file(project_name, project_path);
         open_add_project = 0;
      }
   }
   nk_end(ctx);
}

// modified version of https://github.com/vurtun/nuklear/blob/master/demo/style.c
void set_style() {
    struct nk_color table[NK_COLOR_COUNT];
    table[NK_COLOR_TEXT] = nk_rgba(190, 190, 190, 255);
    table[NK_COLOR_WINDOW] = nk_rgba(15, 11, 25, 215);
    table[NK_COLOR_HEADER] = nk_rgba(181, 45, 69, 220);
    table[NK_COLOR_BORDER] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_BUTTON] = nk_rgba(30, 30, 30, 255);
    table[NK_COLOR_BUTTON_HOVER] = nk_rgba(190, 50, 70, 255);
    table[NK_COLOR_BUTTON_ACTIVE] = nk_rgba(195, 55, 75, 255);
    table[NK_COLOR_TOGGLE] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_TOGGLE_HOVER] = nk_rgba(45, 60, 60, 255);
    table[NK_COLOR_TOGGLE_CURSOR] = nk_rgba(181, 45, 69, 255);
    table[NK_COLOR_SELECT] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_SELECT_ACTIVE] = nk_rgba(181, 45, 69, 255);
    table[NK_COLOR_SLIDER] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_SLIDER_CURSOR] = nk_rgba(181, 45, 69, 255);
    table[NK_COLOR_SLIDER_CURSOR_HOVER] = nk_rgba(186, 50, 74, 255);
    table[NK_COLOR_SLIDER_CURSOR_ACTIVE] = nk_rgba(191, 55, 79, 255);
    table[NK_COLOR_PROPERTY] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_EDIT] = nk_rgba(51, 55, 67, 225);
    table[NK_COLOR_EDIT_CURSOR] = nk_rgba(190, 190, 190, 255);
    table[NK_COLOR_COMBO] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_CHART] = nk_rgba(51, 55, 67, 255);
    table[NK_COLOR_CHART_COLOR] = nk_rgba(170, 40, 60, 255);
    table[NK_COLOR_CHART_COLOR_HIGHLIGHT] = nk_rgba( 255, 0, 0, 255);
    table[NK_COLOR_SCROLLBAR] = nk_rgba(30, 33, 40, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR] = nk_rgba(64, 84, 95, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_HOVER] = nk_rgba(70, 90, 100, 255);
    table[NK_COLOR_SCROLLBAR_CURSOR_ACTIVE] = nk_rgba(75, 95, 105, 255);
    table[NK_COLOR_TAB_HEADER] = nk_rgba(181, 45, 69, 220);
    nk_style_from_table(ctx, table);
}
