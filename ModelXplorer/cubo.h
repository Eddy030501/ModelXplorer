#pragma once

#ifndef CUBO_H
#define CUBO_H

#include "ShaderProgram.h"
#include <glm/glm.hpp>
#include "application.h"
// Function declarations for initializing, rendering, and cleaning up the cube
void initCube(); // Change initCubo to initCube to match the implementation
void renderCube(ShaderProgram& shader, glm::vec3 position, glm::vec3 scale, glm::vec3 rotation, glm::mat4 view, glm::mat4 projection);
void cleanupCube();

#endif // CUBO_H
