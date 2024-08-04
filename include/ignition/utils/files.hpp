#pragma once

#include "types/texture.hpp"

#include <string>
#include <memory>
namespace FS {

   std::string GetHome();

   std::string ReadTextFile(std::string path);
   std::string ReadFileFromHome(std::string path);

   void WriteFile(std::string path, std::string data);
   void WriteFileFromHome(std::string path, std::string data);

   void BeginBinaryRead(std::string path);
   void BeginBinaryWrite(std::string path);

   uint8_t Read8();
   uint16_t Read16();
   uint32_t Read32();
   float ReadFloat();

   bool CanRead();

   void Write8(uint8_t data);
   void Write16(uint16_t data);
   void Write32(uint32_t data);
   void WriteFloat(float data);

   void EndBinaryRead();
   void EndBinaryWrite();
}

