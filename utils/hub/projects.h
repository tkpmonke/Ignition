#pragma once
#include <time.h>

struct project {
   char* name;
   char* path;
   time_t lastModified;
   int size;
};

extern struct project* projects;
extern int project_count;

struct new_project_details {
   char* name;
   char* path;

   char* app_lua;
   char* camera_lua;
};
extern struct new_project_details new_project;

#define CREATE_APP_LUA(out, name, dev) \
   sprintf(out, "local M = {\n" \
   "   appName = \"%s\",\n" \
   "   version = \"0.0.1\",\n" \
   "   devName = \"%s\",\n" \
   "   graphicsAPI = \"opengl\"\n" \
   "}\n" \
   "return M\n", name, dev);

#define CREATE_CAMERA_LUA() \
   "local M = {\n" \
   "   position = ig.Vector3(0, 0, 0),\n" \
   "   rotation = ig.Vector3(0, 0, 0),\n" \
   "   fov = 70,\n" \
   "   near = 0.1,\n" \
   "   far = 1000\n" \
   "}\n" \
   "return M\n"

void load_projects();
void create_project(char* name, char* path, char* dev);
void remove_project(char* path);
void run_project(char* path);

void add_project_to_projects_file(char* name, char* path);

char* file_explorer_for_project();
