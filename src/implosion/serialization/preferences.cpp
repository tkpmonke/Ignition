#include "serialization/saving.hpp"
#include "input/camera_movement.hpp"

#include <iostream>
#include <filesystem>

bool preferencesChanged;
Implosion::GUI* gui;

void PreferencesChanged() { preferencesChanged = true; }

void WritePreferences() 
{
   if (!std::filesystem::exists(FS::GetHome() + "/Implosion"))
   {
      std::filesystem::create_directory(FS::GetHome() + "/Implosion");
   }
   FS::BeginBinaryWrite(FS::GetHome() + "/Implosion/preferences");

   FS::Write8(gui->vsync);
   FS::Write8(gui->anti);
   FS::Write8(gui->enableGrid);
   FS::Write8(gui->enableDistanceFalloff);
   FS::WriteFloat(gui->gridSpacing);
   FS::WriteFloat(gui->gridSize);
   FS::WriteFloat(gui->gridHeight);
   FS::WriteFloat(gui->gridDistance);
   FS::WriteFloat(gui->gridFalloff);

   FS::WriteFloat(gui->gridColor[0]);
   FS::WriteFloat(gui->gridColor[1]);
   FS::WriteFloat(gui->gridColor[2]);
   FS::WriteFloat(gui->gridColor[3]);

   FS::WriteFloat(gui->gridWidth);

   FS::WriteFloat(sensitivity);
   FS::WriteFloat(movespeed);
   FS::WriteFloat(shiftMul);
   FS::WriteFloat(gui->camera->clipping_planes.min);
   FS::WriteFloat(gui->camera->clipping_planes.max);
   FS::WriteFloat(gui->camera->fov);

   FS::EndBinaryWrite();

   std::cout << "-- PREFERENCES SAVED --\n";
}

void ReadPreferences(Implosion::GUI* guis)
{
   gui = guis;
   if (!FS::BeginBinaryRead(FS::GetHome() + "/Implosion/preferences"))
      return;
   
   if (!FS::CanRead())
      return;
   guis->vsync = FS::Read8();
   guis->anti = FS::Read8();
   guis->enableGrid = FS::Read8();
   guis->enableDistanceFalloff = FS::Read8();
   guis->gridSpacing = FS::ReadFloat();
   guis->gridSize = FS::ReadFloat();
   guis->gridHeight = FS::ReadFloat();
   guis->gridDistance = FS::ReadFloat();
   guis->gridFalloff = FS::ReadFloat();

   guis->gridColor[0] = FS::ReadFloat();
   guis->gridColor[1] = FS::ReadFloat();
   guis->gridColor[2] = FS::ReadFloat();
   guis->gridColor[3] = FS::ReadFloat();

   guis->gridWidth = FS::ReadFloat();
   sensitivity = FS::ReadFloat();
   movespeed = FS::ReadFloat();
   shiftMul = FS::ReadFloat();
   guis->camera->clipping_planes.min = FS::ReadFloat();
   guis->camera->clipping_planes.max = FS::ReadFloat();
   guis->camera->fov = FS::ReadFloat();

   FS::EndBinaryRead();
}
