#pragma once

#include <glm/glm.hpp>

enum LightType {
    DIRECTIONAL,
    POINT,
    SPOT
};

struct Light {
    LightType type;
    glm::vec3 position;
    glm::vec3 direction;
    glm::vec3 color;
    float intensity;
    float radius;
    float cutoff;

    Light()
        : type(DIRECTIONAL), position(glm::vec3(0.0f)), direction(glm::vec3(0.0f, -1.0f, 0.0f)),
        color(glm::vec3(1.0f)), intensity(1.0f), radius(10.0f), cutoff(45.0f) {}
};
