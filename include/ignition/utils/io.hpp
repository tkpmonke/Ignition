#pragma once

#include <iostream>
#include <string>

#include <unistd.h>

namespace Ignition::IO {
   extern std::string log;
   static void Print(std::string msg) { write(1, ("\033[0m"+msg+"\n").c_str(), (size_t)(msg.size()+5)); }

#ifdef DEBUG
   static void DebugPrint(std::string msg) { write(1, ("\033[1;34;5mDebug > \033[21m"+msg+"\033[0m\n").c_str(), msg.size()+27); }
#else
   static void DebugPrint(std::string msg) {}
#endif

   static void Warning(std::string msg) {write(1, ("\033[1;33mWarning > \033[21m " + msg + "\033[0m\n").data(), msg.size()+28); }

   static void Error(std::string msg) { write(2, ("\033[1;31mError > \033[21m "+msg+"\033[0m\n").data(), msg.size()+26); }

   static void FatalError(std::string msg) { write(2, ("\033[1;31;4mFatal Error > \033[21m"+msg+"\033[0m\n").data(), msg.size()+34); std::quick_exit(-1); }

   extern bool editor;
   static bool InEditor() { return editor; }
}
