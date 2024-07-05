#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "ModelLoader.h"
#include "ImGuiView.h"
#include "Camera.h"
#include "light.h"
#include "cubo.h"
class Application {
public:
    Application();
    ~Application();
    void renderModel();
    void run();

private:
    glm::mat4 lightSpaceMatrix;
    Light light;
    GLFWwindow* window;
    ShaderProgram shaderProgram;
    ShaderProgram skyboxShader;
    ShaderProgram luces;
    ModelLoader modelLoader;
    std::vector<ModelLoader*> modelLoaders;
    ImGuiView imguiView;
    Camera camera;
    std::shared_ptr<ModelLoader> skyboxLoader;
    float deltaTime;
    float lastFrame;
    bool keys[1024];
    bool initializeGLFW();
    bool initializeOpenGL();
    void setupShaders();
    void setupImGui();
    void cleanup();
    void processInput();
    void renderImGui();
    void updateLightSpaceMatrix();
    void renderSkybox(ShaderProgram& shader);
    GlobalLight globalLight;
    static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
    static void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
    static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    bool& IsKeyPressed(int key);
    void ProcessScroll(double yOffset);
};