#ifdef __STDC_ALLOC_LIB__
#define __STDC_WANT_LIB_EXT2__ 1
#else
#define _POSIX_C_SOURCE 200809L
#endif

#include "projects.h"
struct project* projects;
int project_count;
struct new_project_details new_project;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdint.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>
#include <unistd.h>
#include <ftw.h>

char* get_home() {
   return (char*)getenv("HOME");
}

static unsigned int total = 0;
int file_size_sum(const char* path, const struct stat *sb, int typeflag) {
   total += sb->st_size;
   return 0;
}

void print_human_readable_size(size_t size) {
   const char* units[] = {"B", "KB", "MB", "GB", "TB"};
   int unit_index = 0;

   double readable_size = (double)size;
   while (readable_size >= 1024 && unit_index < 4) {
      readable_size /= 1024;
      unit_index++;
   }

   printf("%.2f %s\n", readable_size, units[unit_index]);
}

void load_projects() {
   char projects_path[200];
   sprintf(projects_path, "%s/.config/Implosion-Hub/projects", get_home());
   FILE* f = fopen(projects_path, "r");

   if (f == NULL) {
      printf("Failed to open projects file\n");
      return;
   }

   char* s = NULL;
   size_t len;
   int i = 0;
   while (getline(&s, &len, f) != -1) {

      printf("%s", s);
      s[strcspn(s, "\n")] = '\0';
      if (i == 0) {
         project_count = atoi(s);
         projects = (struct project*)calloc(project_count, sizeof(struct project));

         i++;
         continue;
      }

      char* name = strtok(s, ":");
      if (name != NULL) {
         projects[i-1].name = malloc(strlen(name) + 1);
         if (projects[i-1].name == NULL) {
            printf("Failed to allocate memory for project name\n");
            return;
         } else {
            strcpy(projects[i-1].name, name);
         }
      }
      printf("Name: %s\n", projects[i-1].name);
      

      char* p = strtok(NULL, ":");
      if (p != NULL) {
          projects[i-1].path = malloc(strlen(p) + 1);
          if (projects[i-1].path == NULL) {
              printf("Failed to allocate memory for project path\n");
              free(projects[i-1].name);
              free(s);
              return;
          }
          strcpy(projects[i-1].path, p);
      }

      struct stat st = {0};
      if (stat(projects[i-1].path, &st) == -1) {
         printf("Failed to get last modified time\n");
         projects[i-1].lastModified = 0;
      } else {
         projects[i-1].lastModified = st.st_mtime;
         printf("Last Modified: %s\n", ctime(&projects[i-1].lastModified));
      }

      total = 0;
      if (ftw(projects[i-1].path, file_size_sum, 16) == -1) {
         printf("Failed to get size of project\n");
         projects[i-1].size = 0;
      } else {
         projects[i-1].size = total;
         print_human_readable_size(projects[i-1].size);
      }

      i++;
   }

}

void run_project(char* path) {
   char* s[4] = {"implosion", "-f", path, NULL};
   execvp("implosion", s);
}

void add_project_to_projects_file(char* name, char* path) {
   char projects_path[200];
   sprintf(projects_path, "%s/.config/Implosion-Hub/projects", get_home());
   FILE* projects = fopen(projects_path, "r");
   
   if (projects == NULL) {
      printf("Failed to open projects file\n");
      return;
   }

   sprintf(projects_path, "%s/.config/Implosion-Hub/projects.tmp", get_home());

   FILE* tmp = fopen(projects_path, "w");

   if (tmp == NULL) {
      printf("Failed to open tmp file\n");
      return;
   }

   char* s = NULL;
   size_t len;
   int i = 0;
   int project_count = 0;
   while (getline(&s, &len, projects) != -1) {
      if (i != 0) {
         fprintf(tmp, "%s", s);
      } else {
         project_count = atoi(s);
         fprintf(tmp, "%d\n", project_count + 1);
      }
      i++;
   }

   fprintf(tmp, "%s:%s\n", name, path);

   fclose(projects);
   fclose(tmp);

   char project_out_path[200];
   sprintf(project_out_path, "%s/.config/Implosion-Hub/projects", get_home());
   rename(projects_path, project_out_path);

   load_projects();
}

void create_project(char* name, char* path, char* dev) {
   if(mkdir(path, 0777) && errno != EEXIST)
        fprintf(stderr, "error while trying to create '%s'\n%m\n", path);
   
   char project_path[400];
   sprintf(project_path, "%s/project", path);
   FILE* project = fopen(project_path, "w");

   if (project == NULL) {
      printf("Failed to create project file\n");
      return;
   }

   uint16_t name_len = strlen(name);
   fprintf(project, "%c%c%s", name_len>>8, name_len, name);
   const char b[8] = {0x00, 0x01, 0x00, 0x00, 0x00, 0x01, 0x00, 0x07};
   fwrite(b, 8, 1, project);
   fprintf(project, "Default");

   fclose(project);

   char scene_path[400];
   sprintf(scene_path, "%s/Default.igscn", path);
   FILE* scene = fopen(scene_path, "w");
   for (int i = 0; i < 4; i++) {
      putc(0x00, scene);
   }
   fclose(scene);

   char settings_path[400];
   sprintf(settings_path, "%s/settings", path);
   mkdir(settings_path, 0777);

   char app_lua_path[400];
   sprintf(app_lua_path, "%s/settings/app.lua", path);
   FILE* app_lua = fopen(app_lua_path, "w");
   char app_lua_contents[200];
   CREATE_APP_LUA(app_lua_contents, name, dev);
   fprintf(app_lua, "%s", app_lua_contents);
   fclose(app_lua);

   char camera_lua_path[400];
   sprintf(camera_lua_path, "%s/settings/camera.lua", path);
   FILE* camera_lua = fopen(camera_lua_path, "w");
   fprintf(camera_lua, CREATE_CAMERA_LUA());

   add_project_to_projects_file(name, path);
}

char* file_explorer_for_project() {
   char* s = malloc(1024);
   
   FILE* f = popen("zenity --file-selection --directory", "r");
   fgets(s, 1024, f);

   s[strcspn(s, "\n")] = '\0';
   return s;
}
