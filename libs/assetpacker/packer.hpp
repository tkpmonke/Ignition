#pragma once

#define FILE_DEF_TXT    "PCKASTDTBS"
#define FILE_END_BYTES  "\0\0\0\0\0\0\0\0\0\0\0\0packendfile"
#define FILE_PATH_END_BYTES '\12'
#define OUT_FILE_NAME   "assets.pad"

#include <filesystem>
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <iostream>

#include "../../include/ignition/utils/files.hpp"

static void pack_compress(std::string& str) {
   std::string s;
   int i = 1;
   char l;
   for ( char c : str ) {
      if ( c == l ) {
         i++;
         continue;
      } else if ( i > 3) {
         s += '\27' + (char)i + c;
         i = 1;
      } else {
         s += c;
         i = 1;
      }
      l = c;
   }

   str = s;
}

static void pack(const char* directory, const char* outdir, bool compression) {
   if (std::filesystem::exists(outdir))
      std::filesystem::remove_all(outdir);

   auto dir = std::filesystem::recursive_directory_iterator{directory};
   int i = std::count_if(dir, {}, [](auto& x) { return !std::filesystem::is_directory(x)
                                                && x.path().extension() != ".igscn"; });
   std::string s;

   dir = std::filesystem::recursive_directory_iterator{directory};
   for (auto& p : dir) {

      if (std::filesystem::is_directory(p)) continue;
      if (p.path().extension() == ".igscn") continue;

      std::string path = p.path().string().substr(std::string(directory).size()+1, p.path().string().size());

      s += path + FILE_PATH_END_BYTES;
      s += Ignition::IO::ReadTextFile(p.path().string()) + FILE_END_BYTES;
   }

   if (compression) {
      //std::cout << "WITHOUT COMPRESSION : " << s.size() << "\n";
      pack_compress(s);
      //std::cout << "WITH COMPRESSION    : " << s.size() << "\n";
   }

   std::filesystem::create_directory(outdir);
   std::ofstream out((std::string)outdir + "/" + OUT_FILE_NAME);
   out << FILE_DEF_TXT;
   out << (char)compression;
   out << char(i<<24);
   out << char(i<<16);
   out << char(i<<8);
   out << char(i);
   out << s;
}

using Filesystem = std::unordered_map<std::string, std::string>;

[[nodiscard]] 
static Filesystem unpack(const char* file) {
   std::string s = Ignition::IO::ReadTextFile(file);
   if (s.substr(0, 10) != FILE_DEF_TXT) {
      std::cout << s.substr(0, 10) << "\n";
      return Filesystem(0);
   }
   
   bool compressed = s[11]; 
   int numOfFiles =  ((int)s[12]<<24) +
                     ((int)s[13]<<16) +
                     ((int)s[14]<<8) +
                     ((int)s[15]);

   Filesystem f;

   std::string contents;
   std::string filepath;
   
   // offset a cause other stuffs
   int a = 1,b;

   while ((b=s.find(FILE_END_BYTES, a)+sizeof(FILE_END_BYTES))!=s.size()-1) {
      filepath = s.substr(a-1, s.find(FILE_PATH_END_BYTES, a));
      contents = s.substr(s.find(FILE_PATH_END_BYTES,a)+sizeof(FILE_PATH_END_BYTES), s.find(FILE_END_BYTES, a));

      f[filepath] = contents;
      a = b;

      //std::cout << filepath << "\n";
      //std::cout << contents.substr(0, 50) << "\n";
   }

   return f;
}
