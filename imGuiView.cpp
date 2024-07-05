#include "ImGuiView.h"
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <tinyfiledialogs.h>
#include <stdio.h>
#include <irrKlang/irrKlang.h>

using namespace irrklang;
ImGuiView::ImGuiView()
    : skyboxLoader(new ModelLoader()), modelLoader(nullptr), modelRotation(0.0f), modelScale(1.0f),
    soundEngine(nullptr), buttonClickSound(nullptr) {

    modelLoaders = std::vector<ModelLoader*>();

    // Iniciar el motor de sonido
    soundEngine = createIrrKlangDevice();
    if (!soundEngine) {
        printf("Error al iniciar IrrKlang\n");
    }
    else {
        // Cargar el archivo de sonido
        buttonClickSound = soundEngine->addSoundSourceFromFile("sound.wav");
        if (!buttonClickSound) {
            printf("Error al cargar el sonido\n");
        }
    }
}
void ImGuiView::init(GLFWwindow* window) {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui::StyleColorsDark();
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");
}

void ImGuiView::shutdown() {
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiView::newFrame() {
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiView::render() {
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void ImGuiView::setModelLoader(ModelLoader* loader) {
    modelLoader = loader;
}
void ImGuiView::drawLightControl(GlobalLight& globalLight, Light& light) {
    ImGui::Begin("Control de Luz");

    // Añadir controles para la luz global usando globalLight
    ImGui::Text("Luz Global:");
    ImGui::ColorEdit3("Color ambiente", glm::value_ptr(globalLight.ambientColor));
    ImGui::SliderFloat("Intensidad ambiente", &globalLight.ambientIntensity, 0.0f, 1.0f);

    ImGui::Separator();

    // Añadir controles para la luz usando light
    ImGui::Text("Luz:");
    ImGui::Text("Posición:");
    ImGui::SliderFloat3("Posición", glm::value_ptr(light.position), -10.0f, 10.0f);
    ImGui::Text("Dirección:");
    ImGui::SliderFloat3("Dirección", glm::value_ptr(light.direction), -1.0f, 1.0f);
    ImGui::Text("Color:");
    ImGui::ColorEdit3("Color", glm::value_ptr(light.color));
    ImGui::SliderFloat("Intensidad", &light.intensity, 0.0f, 10.0f);
    ImGui::SliderFloat("Radio", &light.radius, 0.0f, 100.0f);
    ImGui::SliderFloat("Corte", &light.cutoff, 0.0f, 1.0f);

    ImGui::Separator();

    // Añadir controles para la textura de sombra
    ImGui::Text("Sombra:");
    ImGui::SliderFloat("Bias", &shadowBias, 0.0f, 0.01f);

    ImGui::Separator();

    ImGui::End();
}
void ImGuiView::drawImGuiContent() {
    ImGuiIO& io = ImGui::GetIO();

    int window_width = io.DisplaySize.x;
    int window_height = io.DisplaySize.y;

    ImGui::SetNextWindowSize(ImVec2(window_width / 3, window_height / 2), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);

    ImGui::Begin("Main Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);

    if (ImGui::Button("Agregar modelo")) {
        if (soundEngine && buttonClickSound) {
            soundEngine->play2D(buttonClickSound);
        }

        const char* file_path = tinyfd_openFileDialog(
            "Seleccione un archivo 3D", "", 0, nullptr, nullptr, 0);

        if (file_path) {
            printf("Archivo seleccionado: %s\n", file_path);
            if (modelLoader) {
                modelLoader->LoadModel(file_path);
            }
        }
    }

    ImGui::Separator();

    if (modelLoader && modelLoader->IsModelLoaded()) {
        ImGui::Text("Modelo cargado:");
        ImGui::Text("Ruta: %s", modelLoader->GetModelPath().c_str());

        glm::vec3 modelPosition = modelLoader->GetModelPosition();
        ImGui::Text("Posición:");
        ImGui::InputFloat3("##ModelPosition", glm::value_ptr(modelPosition), "%.2f");

        ImGui::Separator();
        ImGui::Text("Controles de movimiento:");
        ImGui::SliderFloat("X", &modelPosition.x, -10.0f, 10.0f);
        ImGui::SliderFloat("Y", &modelPosition.y, -10.0f, 10.0f);
        ImGui::SliderFloat("Z", &modelPosition.z, -10.0f, 10.0f);

        modelLoader->SetModelPosition(modelPosition);

        glm::vec3 modelRotation = modelLoader->GetModelRotation();
        ImGui::Text("Rotación:");
        ImGui::SliderFloat("Rotacion X", &modelRotation.x, 0.0f, 360.0f);
        ImGui::SliderFloat("Rotacion Y", &modelRotation.y, 0.0f, 360.0f);
        ImGui::SliderFloat("Rotacion Z", &modelRotation.z, 0.0f, 360.0f);

        modelLoader->SetModelRotation(modelRotation);

        ImGui::Separator();
        ImGui::Text("Escala:");
        ImGui::SliderFloat("Scale X", &modelScale.x, 0.1f, 10.0f);
        ImGui::SliderFloat("Scale Y", &modelScale.y, 0.1f, 10.0f);
        ImGui::SliderFloat("Scale Z", &modelScale.z, 0.1f, 10.0f);

        modelLoader->SetModelScale(modelScale);
    }
    else {
        ImGui::Text("No hay modelo cargado");
    }

    ImGui::End();
}

void ImGuiView::drawSkyboxControl() {
    ImGuiIO& io = ImGui::GetIO();
    int window_width = io.DisplaySize.x;
    int window_height = io.DisplaySize.y;

    ImGui::SetNextWindowSize(ImVec2(window_width / 3, window_height / 2), ImGuiCond_Always);
    ImGui::SetNextWindowPos(ImVec2(window_width - window_width / 3, 0), ImGuiCond_Always);

    ImGui::Begin("Skybox Menu", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove);

    if (!skyboxLoader) {
        ImGui::Text("SkyboxLoader no está inicializado.");
        ImGui::End();
        return;
    }

    if (ImGui::Button("Agregar Skybox")) {
        const char* file_path = tinyfd_openFileDialog(
            "Seleccione un archivo skybox", "", 0, nullptr, nullptr, 0);

        if (file_path) {
            printf("Archivo skybox seleccionado: %s\n", file_path);
            if (skyboxLoader) {
                bool result = skyboxLoader->LoadModel(file_path);
                printf("Resultado de carga: %s\n", result ? "true" : "false");
                if (result) {
                    glm::vec3 initialScale(1.0f);
                    skyboxLoader->SetModelScale(initialScale);
                }
                else {
                    printf("Error: No se pudo cargar el skybox.\n");
                }
            }
            else {
                printf("Error: skyboxLoader no está inicializado.\n");
            }
        }
    }

    ImGui::Separator();

    if (skyboxLoader && skyboxLoader->IsModelLoaded()) {
        ImGui::Text("Skybox cargado:");
        ImGui::Text("Ruta: %s", skyboxLoader->GetModelPath().c_str());

        ImGui::Separator();
        ImGui::Text("Escala:");

        glm::vec3 skyboxScale = skyboxLoader->GetModelScale();
        ImGui::SliderFloat("Scale X", &skyboxScale.x, 0.1f, 10.0f);
        ImGui::SliderFloat("Scale Y", &skyboxScale.y, 0.1f, 10.0f);
        ImGui::SliderFloat("Scale Z", &skyboxScale.z, 0.1f, 10.0f);

        skyboxLoader->SetModelScale(skyboxScale);
    }
    else {
        ImGui::Text("No hay skybox cargado");
    }

    ImGui::End();
}

void ImGuiView::setSkyboxLoader(std::shared_ptr<ModelLoader> loader) {
    skyboxLoader = loader;
}
void ImGuiView::setModelScale(const glm::vec3& scale) {
    modelScale = scale;
}

glm::vec3 ImGuiView::getModelScale() const {
    return modelScale;
}

void ImGuiView::drawCoordinateAxes() {
}

void ImGuiView::updateMousePosition(float mouseX, float mouseY) {
    // Implement mouse position update logic here
}

void ImGuiView::moveModel(const glm::vec3& delta) {
    if (modelLoader) {
        glm::vec3 currentPosition = modelLoader->GetModelPosition();
        modelLoader->SetModelPosition(currentPosition + delta);
    }
}

void ImGuiView::setModelRotation(const glm::vec3& rotation) {
    modelRotation = rotation;
    if (modelLoader) {
        modelLoader->SetModelRotation(rotation);
    }
}

glm::vec3 ImGuiView::getModelRotation() const {
    return modelRotation;
}

glm::vec3 ImGuiView::getModelPosition() const {
    if (modelLoader) {
        return modelLoader->GetModelPosition();
    }
    return glm::vec3(0.0f);
}