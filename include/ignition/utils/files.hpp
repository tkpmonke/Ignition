#pragma once

#include <string>
#include <cstdint>

#include "object.hpp"

namespace Ignition::IO {

   std::string GetHome();
   std::string FileExplorer();
   void EditFile(std::string);
   std::string GetDateTime();

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

   Vector2 ReadVector2();
   Vector3 ReadVector3();
   Vector4 ReadVector4();

   bool CanRead(int);

   void Write8(uint8_t data);
   void Write16(uint16_t data);
   void Write32(uint32_t data);
   void WriteFloat(float data);
   void WriteString(std::string data);

   void WriteVector2(Vector2 data);
   void WriteVector3(Vector3 data);
   void WriteVector4(Vector4 data);

   void EndBinaryRead();
   void EndBinaryWrite();
}

