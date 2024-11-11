#include <iostream>
#include <string>

namespace Ignition::IO {
   static void Print(std::string msg) { std::cout << "\033[0m" << msg << "\n"; }

   static void Warning(std::string msg) {std::cout << "\033[1;33mWarning : \033[21m " << msg << "\033[0m\n"; }

   static void Error(std::string msg) { std::cerr << "\033[1;31mError : \033[21m " << msg << "\033[0m\n"; }

   static void FatalError(std::string msg) { std::cerr << "\033[1;31;4mFatal Error : \033[21m " << msg << "\033[0m\n"; std::quick_exit(-1); }
}
