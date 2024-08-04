#pragma once

#include "utils/files.hpp"
#include "gui/gui.hpp"

void PreferencesChanged();

void WritePreferences();
void ReadPreferences(Implosion::GUI*);
