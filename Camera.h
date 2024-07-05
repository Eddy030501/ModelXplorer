#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

const GLfloat YAW = -90.0f;
const GLfloat PITCH = 0.0f;
const GLfloat SPEED = 3.0f;
const GLfloat SENSITIVITY = 0.1f;
const GLfloat ZOOM = 45.0f;

enum class CameraMovement {
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT
};

class Camera {
public:
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), GLfloat yaw = YAW, GLfloat pitch = PITCH);
    float smoothMovement(float value);
    glm::mat4 GetViewMatrix();
    float GetZoom() const;
    glm::vec3 GetPosition() const;
    void ProcessKeyboard(bool keys[], float deltaTime);
    void ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch = true);
    void ProcessMouseScroll(float yOffset);

private:
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;

    GLfloat yaw;
    GLfloat pitch;
    GLfloat movementSpeed;
    GLfloat mouseSensitivity;
    GLfloat zoom;

    void updateCameraVectors();
};
