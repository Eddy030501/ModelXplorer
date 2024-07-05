#include "Camera.h"
#include <iostream>
Camera::Camera(glm::vec3 position, glm::vec3 up, GLfloat yaw, GLfloat pitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), movementSpeed(SPEED), mouseSensitivity(SENSITIVITY), zoom(ZOOM) {
    this->position = position;
    this->worldUp = up;
    this->yaw = yaw;
    this->pitch = pitch;
    updateCameraVectors();
}

void Camera::ProcessKeyboard(bool keys[], float deltaTime) {
    float velocity = movementSpeed * deltaTime;
    bool positionChanged = false;

    if (keys[GLFW_KEY_W]) {
        position += front * velocity;
        positionChanged = true;
    }
    if (keys[GLFW_KEY_S]) {
        position -= front * velocity;
        positionChanged = true;
    }
    if (keys[GLFW_KEY_A]) {
        position -= glm::normalize(glm::cross(front, up)) * velocity;
        positionChanged = true;
    }
    if (keys[GLFW_KEY_D]) {
        position += glm::normalize(glm::cross(front, up)) * velocity;
        positionChanged = true;
    }

    if (positionChanged) {
        std::cout << "Camera Position: " << position.x << ", " << position.y << ", " << position.z << std::endl;
    }
}


glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

float Camera::GetZoom() const {
    return zoom;
}

glm::vec3 Camera::GetPosition() const {
    return position;
}

void Camera::ProcessMouseMovement(float xOffset, float yOffset, GLboolean constrainPitch) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (constrainPitch) {
        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yOffset) {
    if (zoom >= 1.0f && zoom <= 45.0f)
        zoom -= yOffset;
    if (zoom <= 1.0f)
        zoom = 1.0f;
    if (zoom >= 45.0f)
        zoom = 45.0f;
}


void Camera::updateCameraVectors() {
    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    this->front = glm::normalize(front);

    right = glm::normalize(glm::cross(this->front, worldUp));
    up = glm::normalize(glm::cross(right, this->front));
}

