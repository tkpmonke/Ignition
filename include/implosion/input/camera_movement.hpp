#pragma once

#include "camera.hpp"
#include "window.hpp"

extern float sensitivity, movespeed;

void cameraMovement(Ignition::Window* window, Ignition::Camera* camera);
