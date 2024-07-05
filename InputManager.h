#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class InputManager {
public:
    InputManager();
    void processInput(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp, float deltaTime);
    void processKeyboard(GLFWwindow* window, glm::vec3& cameraPos, glm::vec3& cameraFront, glm::vec3& cameraUp, float deltaTime);
    static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos);
    static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
    void mouse_callback(GLFWwindow* window, double xpos, double ypos);
    glm::vec3 getCameraFront() const;
    void setCameraFront(glm::vec3 front);
    void setLeftMouseButtonPressed(bool pressed);
    bool isLeftMouseButtonPressed() const;

private:
    bool leftMouseButtonPressed;
    bool firstMouse;
    float lastX;
    float lastY;
    float yaw;
    float pitch;
    float sensitivity;
    float speed;
    glm::vec3 cameraFront;
};

#endif

