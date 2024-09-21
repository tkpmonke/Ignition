/*-------------------------\
|       PREGLSL V1.0       |
|  DEVELOPED BY TKPMONKE   |
|  MAINTAINED BY TKPMONKE  |
|       MADE WITH <3       |
\-------------------------*/

/* PREGLSL is an extension to the glsl language that provides a stronger preprocessing
      step to regular glsl

 * Orignially made for the Ignition Engine
 
   New Preprocessing instructions that PREGLSL adds:
      - #include
      - #error
      - #warning
      - #define FUNC(x)
      - __WIN32__       if on windows
      - __UNIX__        if on unix based os
      - __LINUX__       if on linux
      - __APPLE__       if on an apple device
      - __NVIDIA__      if on an nvidia gpu
      - __AMD__         if on an amd gpu
      - __INTEL__       if on an intel gpu
      - __LINE__
      - __FILE__
      - __PREGLSL
   Old Preprocessing steps that PREGLSL rewrites:
      - #define
      - #ifdef
      - #ifndef
      - #elif
      - #else
      - #endif

   For Info on the order/steps PREGLSL takes, see preprocessing_steps.txt

*/

/* Individually prints each step of the preprocesser, used to
 *    test steps.
*/
#define PREGLSL_PRINT_STEPS

#define PREGLSL_VERSION 1.0

#define PREGLSL_VERSION_STRING "1.0"

#define PREGLSL_ERROR "\100\200\150\121"

#include <vector>
#include <string>


// Run PREGLSL
std::string preglsl_operate(std::string filename);
std::string preglsl_operate_raw(std::string data, std::string name);


static inline std::vector<std::pair<std::string,std::string>> constDefines = {
   {"__PREGLSL", PREGLSL_VERSION_STRING}
};

template<typename T>
void preglsl_add_define(const char* name, T value);
void preglsl_add_define(const char* name);

void preglsl_add_standard_defines();
void preglsl_add_file_define(const char* filename);

void preglsl_process_defines(std::string* data);

