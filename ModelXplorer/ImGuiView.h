#ifndef IMGUIVIEW_H
#define IMGUIVIEW_H

#include <irrKlang.h>   // Asegúrate de tener el path correcto a irrKlang.h
#include "ModelLoader.h"
#include <memory>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include "light.h"

struct GlobalLight {
    glm::vec3 ambientColor;
    float ambientIntensity;

    GlobalLight()
        : ambientColor(glm::vec3(1.0f)), ambientIntensity(0.5f) {}
};

class ImGuiView {
public:
    ImGuiView();
    void init(GLFWwindow* window);
    void shutdown();
    void newFrame();
    void render();
    void setModelLoader(ModelLoader* loader);
    void setSkyboxLoader(std::shared_ptr<ModelLoader> loader);
    void drawImGuiContent();
    void drawSkyboxControl();
    void setModelScale(const glm::vec3& scale);
    glm::vec3 getModelScale() const;
    void drawCoordinateAxes();
    void updateMousePosition(float mouseX, float mouseY);
    void moveModel(const glm::vec3& delta);
    void setModelRotation(const glm::vec3& rotation);
    glm::vec3 getModelRotation() const;
    glm::vec3 getModelPosition() const;
    void drawLightControl(GlobalLight& globalLight, Light& light);
    void drawLightCube();

private:
    ModelLoader* modelLoader;
    std::vector<ModelLoader*> modelLoaders;
    glm::vec3 modelPosition;
    std::shared_ptr<ModelLoader> skyboxLoader;
    glm::vec3 modelRotation;
    glm::vec3 modelScale;
    Light light;
    irrklang::ISoundEngine* soundEngine;   // Corregido: usando namespace irrklang para ISoundEngine
    irrklang::ISoundSource* buttonClickSound;   // Corregido: usando namespace irrklang para ISoundSource
    float shadowBias = 0.005f;
};

#endif // IMGUIVIEW_H
