#include "utils/files.hpp"
#include "utils/io.hpp"

#include <iostream>
#include <fstream>
#include <sstream>
#include <string.h>
#include <filesystem>
#include <ctime>
#include <iomanip>

#ifdef __linux__
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
#include <shlobj.h>
#include <locale>
#include <codecvt>
#pragma message "fyi, windows support is an after thought, best used with linux (arch support is the best cause arch is the best)"
#else
#error your out of luck ig (for now)
#endif 

namespace Ignition::IO {
   
   std::string projectName;
   void SetProjectHome(std::string s)
   {
      projectName = s;
   }

   std::string FileExplorer()
   {
      char s[256];
   
      FILE* f = popen("zenity --file-selection", "r");
      
      if (!f) {
         Ignition::IO::Error("Could Not Open File Explorer (zenity --file-selection)");
         return NULL;
      }

      if ( fgets(s, 256, f) == NULL) {
         Ignition::IO::Error("Failed To Read From File Explorer (fgets(\"zenity --file-selection\"))");
      }

      pclose(f);
      return s;
   }

   std::string GetDateTime() {
      std::time_t time = std::time(nullptr);
      std::tm localTime;
      
#ifdef __linux
      localtime_r(&time, &localTime);
#elif defined(_WIN32)

      localtime_s(&localTime, &time);
#endif

      std::ostringstream oss;
      oss << std::put_time(&localTime, "%Y-%m-%d %I:%M:%S");

      return oss.str();
   }

   void EditFile(std::string file) {
#ifdef __linux
      const char* editor;
      if ((editor = getenv("EDITOR")) == NULL)
         if ((editor = getenv("VISUAL")) == NULL)
            if ((editor = getenv("SELECTED_EDITOR")) == NULL)
               if ((editor = "vim") && !std::filesystem::exists("/usr/bin/vim"))
                  if ((editor = "nvim") && !std::filesystem::exists("/usr/bin/nvim"))
                     if ((editor = "nano") && !std::filesystem::exists("/usr/bin/nano"))
                        editor = "vi"; // after all these tests, default to vi cause vi is :fire:
                                       //
      const char* term;
      if ((term = getenv("TERM")) == NULL) {
         Ignition::IO::Error("Cannot open file in new terminal window, consider setting $TERM");
      }

      if (system(((std::string)term + " -e " + editor + (std::string)" " + file + " &").data()) != 0)
         Ignition::IO::Error("Could not find a text editor, try setting the env variable EDITOR to your prefered text editor or install vim,nvim,nano, or vi");
#elif defined(_WIN32)
      ShellExecute(NULL, "open", file.data(), NULL, NULL, SW_SHOWNORMAL);
#endif
   }

   std::string GetProjectHome() { return projectName; }

   std::string ReadTextFile(std::string path) 
   {
      if (std::filesystem::is_symlink(path)) {
         path = std::filesystem::read_symlink(path);
         return path;
      }
      std::ifstream f(path);
      std::stringstream s;
      s << f.rdbuf();
      return s.str();
   }

   std::string ReadFileFromHome(std::string path)
   {
      return ReadTextFile(GetHome() + path);
   }

   void WriteFile(std::string path, std::string data)
   {
      std::ofstream f(path);
      f << data;
   }

   void WriteFileFromHome(std::string path, std::string data)
   {
      WriteFile(GetHome() + path, data);
   }
   
   struct {
      std::ifstream file;
      uint64_t i = 0;
      uint64_t size;
      std::string data;
   } read;

   struct {
      std::string data;
      std::string path;
   } write;

   bool BeginBinaryRead(std::string path)
   {
      read.file = std::ifstream(path);
      
      if (!read.file.is_open())
         return false;

      std::stringstream s;
      s << read.file.rdbuf();
      read.data = s.str();
      read.size = read.data.size();
      read.i = 0;
      return true;
   }
   void BeginBinaryWrite(std::string path)
   {
      write.data = "";
      write.path = path;
   }

   uint8_t Read8()
   {
      return read.data[read.i++];
   }
   uint16_t Read16()
   {
      uint16_t a, b;
      a = read.data[read.i++];
      b = read.data[read.i++];
      return (a << 8) + b;
   }
   uint32_t Read32()
   {
      char a, b, c, d;
      if (read.i + 4 > read.size)
      {
         std::cerr << "reading too far Read32\n";
         return 0;
      }
      a = read.data[read.i++];
      b = read.data[read.i++];
      c = read.data[read.i++];
      d = read.data[read.i++];
      uint32_t o;
      o = o << 8 | static_cast<unsigned char>(a);
      o = o << 8 | static_cast<unsigned char>(b);
      o = o << 8 | static_cast<unsigned char>(c);
      o = o << 8 | static_cast<unsigned char>(d);
      return o;
   }

   float ReadFloat()
   {
      char a, b, c, d;
      float o;
      
      if (read.i + 4 > read.size)
      {
#ifdef DEBUG
         std::cerr << "reading too far float\n";
#endif
         return 0;
      }
      a = read.data[read.i++];
      b = read.data[read.i++];
      c = read.data[read.i++];
      d = read.data[read.i++];
      
      // some narley conversion from char[4] to float
      char e[4] = {a, b, c, d}; 
      o = *(float*)&e;
      
      return o;
   }

   std::string ReadString()
   {
      uint16_t s = Read16();
      std::string o;

      for (int i = 0; i < s; ++i)
         o += Read8();

      return o;
   }

   bool CanRead(int amount)
   {
      return read.size > read.i+amount;
   }

   void Write8(uint8_t data)
   {
      write.data.push_back(data);
   }

   void Write16(uint16_t data)
   {
      write.data.push_back((char)(data >> 8));
      write.data.push_back((char)data);
   }

   void Write32(uint32_t data)
   {
      write.data.push_back(data >> 24);
      write.data.push_back(data >> 16);
      write.data.push_back(data >> 8);
      write.data.push_back(data);
   }

   void WriteFloat(float data)
   {  
      char* b;  
      // some conversion from float to char[4]
      b = (char*)&data;
      write.data.push_back(b[0]);
      write.data.push_back(b[1]);
      write.data.push_back(b[2]);
      write.data.push_back(b[3]);
   }

   void WriteString(std::string data)
   {
      Write16((int)data.size());

      for (int i = 0; i < data.size(); ++i)
         Write8(data[i]);
   }

   void EndBinaryRead()
   {
      read.file.close();
   }
   void EndBinaryWrite()
   {
      std::ofstream o(write.path);
      o << write.data;

      o.close();
      write.data = "";
      write.path = "";
   }


#ifdef __linux__
   std::string GetHome()
   {
      const char* home;
      if ((home = getenv("XDG_CONFIG_HOME")) == NULL)
         if ((home = getenv("HOME")) == NULL)
            home = getpwuid(getuid())->pw_dir;
      return home;
   }
#elif defined(_WIN32) || defined(_WIN64)
   std::string GetHome()
   {
      std::wstringstream ss;
      HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
      if (FAILED(hr)) 
      {
         std::cerr << "Failed to initialize COM library." << std::endl;
      }
      
      PWSTR path = NULL;
      hr = SHGetKnownFolderPath(FOLDERID_RoamingAppData, 0, NULL, &path);
      if (SUCCEEDED(hr)) 
      {
         std::filesystem::path appDataPath(path);
      
         ss << appDataPath;

         CoTaskMemFree(path);
      }
      else
      {
         std::cerr << "Failed to get AppData path." << std::endl;
      }
      
      CoUninitialize();
      return std::wstring_convert<std::codecvt_utf8<wchar_t>>().to_bytes(ss.str());
   }
#endif 
}
