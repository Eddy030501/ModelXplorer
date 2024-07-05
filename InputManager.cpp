#include "InputManager.h"
#include <iostream>

InputManager::InputManager()
    : leftMouseButtonPressed(false), lastX(0.0), lastY(0.0), firstMouse(true), yaw(-90.0f), pitch(0.0f), sensitivity(0.1f), speed(2.5f), cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)) {}

void InputManager::processInput(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp, float deltaTime) {
    processKeyboard(window, cameraPos, cameraFront, cameraUp, deltaTime);
}

void InputManager::processKeyboard(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp, float deltaTime) {
    float cameraSpeed = speed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

void InputManager::cursor_pos_callback(GLFWwindow* window, double xpos, double ypos) {
    auto inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager && inputManager->leftMouseButtonPressed) {
        inputManager->mouse_callback(window, xpos, ypos);
    }
}

void InputManager::mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    auto inputManager = static_cast<InputManager*>(glfwGetWindowUserPointer(window));
    if (inputManager) {
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                inputManager->leftMouseButtonPressed = true;
            }
            else if (action == GLFW_RELEASE) {
                inputManager->leftMouseButtonPressed = false;
                inputManager->firstMouse = true; // Reset to avoid a big jump
            }
        }
    }
}

void InputManager::mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!leftMouseButtonPressed) {
        return;
    }

    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // Las coordenadas y van de abajo hacia arriba
    lastX = xpos;
    lastY = ypos;

    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    if (pitch > 89.0f)
        pitch = 89.0f;
    if (pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 front;
    front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

    setCameraFront(glm::normalize(front));

    // Impresión de depuración
    std::cout << "Callback del mouse: xpos = " << xpos << ", ypos = " << ypos << std::endl;
}

glm::vec3 InputManager::getCameraFront() const {
    return glm::vec3(cameraFront);
}

void InputManager::setCameraFront(glm::vec3 front) {
    cameraFront = front;
}

void InputManager::setLeftMouseButtonPressed(bool pressed) {
    leftMouseButtonPressed = pressed;
}

bool InputManager::isLeftMouseButtonPressed() const {
    return leftMouseButtonPressed;
}
