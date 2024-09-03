#pragma once

#include "types/texture.hpp"

#include <string>
#include <memory>
namespace FS {

   std::string GetHome();
   std::string FileExplorer();

   void SetProjectHome(std::string);
   std::string GetProjectHome();

   std::string ReadTextFile(std::string path);
   void WriteTextFile(std::string path, std::string data);

   bool BeginBinaryRead(std::string path);
   void BeginBinaryWrite(std::string path);

   uint8_t Read8();
   uint16_t Read16();
   uint32_t Read32();
   float ReadFloat();
   std::string ReadString();

   bool CanRead(int);

   void Write8(uint8_t data);
   void Write16(uint16_t data);
   void Write32(uint32_t data);
   void WriteFloat(float data);
   void WriteString(std::string data);

   void EndBinaryRead();
   void EndBinaryWrite();
}

