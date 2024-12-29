#include "serialization/saving.hpp"
#include "input/camera_movement.hpp"

#include <iostream>
#include <filesystem>

bool preferencesChanged;
Implosion::GUI* gui;

void PreferencesChanged() { preferencesChanged = true; }

void WritePreferences() 
{
   if (!std::filesystem::exists(Ignition::IO::GetHome() + "/Implosion"))
   {
      std::filesystem::create_directory(Ignition::IO::GetHome() + "/Implosion");
   }
   Ignition::IO::BeginBinaryWrite(Ignition::IO::GetHome() + "/Implosion/preferences");

   Ignition::IO::Write8(gui->vsync);
   Ignition::IO::Write8(gui->anti);
   Ignition::IO::Write8(gui->enableGrid);
   Ignition::IO::Write8(gui->enableDistanceFalloff);
   Ignition::IO::WriteFloat(gui->gridSpacing);
   Ignition::IO::WriteFloat(gui->gridSize);
   Ignition::IO::WriteFloat(gui->gridHeight);
   Ignition::IO::WriteFloat(gui->gridDistance);
   Ignition::IO::WriteFloat(gui->gridFalloff);

   Ignition::IO::WriteFloat(gui->gridColor[0]);
   Ignition::IO::WriteFloat(gui->gridColor[1]);
   Ignition::IO::WriteFloat(gui->gridColor[2]);
   Ignition::IO::WriteFloat(gui->gridColor[3]);

   Ignition::IO::WriteFloat(gui->gridWidth);

   Ignition::IO::WriteFloat(sensitivity);
   Ignition::IO::WriteFloat(movespeed);
   Ignition::IO::WriteFloat(shiftMul);
   Ignition::IO::WriteFloat(gui->camera->clippingPlanes.min);
   Ignition::IO::WriteFloat(gui->camera->clippingPlanes.max);
   Ignition::IO::WriteFloat(gui->camera->fov);

   Ignition::IO::WriteFloat(gui->files.size);

   Ignition::IO::EndBinaryWrite();
}

void ReadPreferences(Implosion::GUI* guis)
{
   gui = guis;
   if (!Ignition::IO::BeginBinaryRead(Ignition::IO::GetHome() + "/Implosion/preferences"))
      return;
   
   if (!Ignition::IO::CanRead(0))
      return;
   guis->vsync = Ignition::IO::Read8();
   glfwSwapInterval(guis->vsync);
   guis->anti = Ignition::IO::Read8();
   guis->enableGrid = Ignition::IO::Read8();
   guis->enableDistanceFalloff = Ignition::IO::Read8();
   guis->gridSpacing = Ignition::IO::ReadFloat();
   guis->gridSize = Ignition::IO::ReadFloat();
   guis->gridHeight = Ignition::IO::ReadFloat();
   guis->gridDistance = Ignition::IO::ReadFloat();
   guis->gridFalloff = Ignition::IO::ReadFloat();

   guis->gridColor[0] = Ignition::IO::ReadFloat();
   guis->gridColor[1] = Ignition::IO::ReadFloat();
   guis->gridColor[2] = Ignition::IO::ReadFloat();
   guis->gridColor[3] = Ignition::IO::ReadFloat();

   guis->gridWidth = Ignition::IO::ReadFloat();
   sensitivity = Ignition::IO::ReadFloat();
   movespeed = Ignition::IO::ReadFloat();
   shiftMul = Ignition::IO::ReadFloat();
   guis->camera->clippingPlanes.min = Ignition::IO::ReadFloat();
   guis->camera->clippingPlanes.max = Ignition::IO::ReadFloat();
   guis->camera->fov = Ignition::IO::ReadFloat();

   //WritePreferences();
   gui->files.size = Ignition::IO::ReadFloat();
   Ignition::IO::EndBinaryRead();
}
