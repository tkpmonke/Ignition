#include "preglsl.hpp"

#include <type_traits>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>

#include <GL/gl.h>

template <typename T>
struct is_string : std::false_type {};

template <>
struct is_string<std::string> : std::true_type {};

template <>
struct is_string<const char*> : std::true_type {};

template <>
struct is_string<char*> : std::true_type {};

template<typename T>
void preglsl_add_define(const char* name, T value) {
   if constexpr (std::is_integral<T>::value || std::is_floating_point<T>::value)
   {
      constDefines.push_back({name, std::to_string(value)});
   } else if (is_string<T>::value) {
      constDefines.push_back({name, std::string(value)});
   } else {
      std::cerr << "PREGLSL : Tried to add define with invalid type, failing preprocessing" << std::endl;
      return;
   }
}

void preglsl_add_define(const char* name) {
   constDefines.push_back({name, ""});
}


void preglsl_add_standard_defines() {
#ifdef __linux__
   preglsl_add_define("__LINUX__");
#endif
#if defined(__WIN32__)
   preglsl_add_define("__WIN32__");
#endif
#if defined(__unix__)
   preglsl_add_define("__UNIX__");
#endif
#if defined(__APPLE__)
   preglsl_add_define("__APPLE__");
#endif

   std::string gpu_vender = reinterpret_cast<const char*>(glGetString(GL_VENDOR));

   if (gpu_vender.find("NVIDIA") != std::string::npos) preglsl_add_define("__NVIDIA__");
   if (gpu_vender.find("AMD")    != std::string::npos) preglsl_add_define("__AMD__");
   if (gpu_vender.find("INTEL")  != std::string::npos) preglsl_add_define("__INTEL__");
}

void preglsl_add_file_define(const char* filename) {
   preglsl_add_define("__FILE__", filename);
}

bool preglsl_is_whitespace(unsigned char c) {
   if (c == ' ' || c == '\t' || c == '\n' ||
       c == '\r' || c == '\f' || c == '\v') {
       return true;
   } else {
       return false;
   }
}

void preglsl_process_defines(std::string* data) {
   /* This function is split up between two chunks    */
   /* One for getting defines, one for applying them */
   std::string line;
   std::istringstream iss(*data);
   while (std::getline(iss, line)) {
      {
         if (line[0] != '#' || line[1] != 'd' || line[2] != 'e')
            continue;

         line = line.substr(8, line.size());

         int f = line.find(' ');

         if (f != std::string::npos) {
            std::string s1 = line.substr(0, f);
            std::string s2 = line.substr(f, line.size());
            
            s1.erase(std::remove_if(s1.begin(), s1.end(), preglsl_is_whitespace), s1.end());
            s2.erase(std::remove_if(s2.begin(), s2.end(), preglsl_is_whitespace), s2.end());

            preglsl_add_define(s1.c_str(), s2);
         } else {
            preglsl_add_define(line.c_str());
         }
      }

      {
         for (auto s : constDefines) {
            std::string g;
            //while ((g = line.find(s)) != std::string::npos) {}
         }
      }
   }


}
