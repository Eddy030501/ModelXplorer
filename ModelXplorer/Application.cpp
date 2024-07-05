#include "Application.h"
#include "cubo.h"
Application::Application()
    : window(nullptr), shaderProgram(), skyboxShader(), camera(glm::vec3(0.0f, 0.0f, 10.0f)), deltaTime(0.0f), lastFrame(0.0f),
    modelLoader(), skyboxLoader(std::make_shared<ModelLoader>()), light() {
    if (!initializeGLFW()) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    if (!initializeOpenGL()) {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }

    setupShaders();
    setupImGui();

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    glfwSetWindowUserPointer(window, this);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    memset(keys, 0, sizeof(keys));

    glfwSetKeyCallback(window, keyCallback);
    glfwSetFramebufferSizeCallback(window, framebufferSizeCallback);
    glfwSetScrollCallback(window, scrollCallback);

    light.type = LightType::DIRECTIONAL;
}

Application::~Application() {
    cleanup();
}

void Application::run() {
    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        processInput();

        // Limpiar el buffer de color y el buffer de profundidad
        glClearColor(0.5f, 0.5f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Renderizar el skybox primero
        renderSkybox(skyboxShader);

        // Renderizar los modelos
        renderModel();

        // Renderizar ImGui encima de todo
        renderImGui();


        // Intercambiar el buffer trasero y el buffer frontal
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}


void Application::processInput() {
    ImGuiIO& io = ImGui::GetIO();
    if (!io.WantCaptureKeyboard && !io.WantCaptureMouse) {
        camera.ProcessKeyboard(keys, deltaTime);

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
            static double lastX = xpos, lastY = ypos;
            double xoffset = xpos - lastX;
            double yoffset = lastY - ypos;
            lastX = xpos;
            lastY = ypos;
            camera.ProcessMouseMovement(xoffset, yoffset);
        }
    }

    if (keys[GLFW_KEY_ESCAPE]) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Application::renderImGui() {
    imguiView.newFrame();
    imguiView.drawImGuiContent();
    imguiView.drawSkyboxControl();
    imguiView.drawLightControl(globalLight,light);
    imguiView.render();
}
void Application::updateLightSpaceMatrix() {
    float near_plane = 1.0f; // Definir el plano cercano adecuadamente
    float far_plane = 100.0f; // Definir el plano lejano adecuadamente

    glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);
    glm::mat4 lightView = glm::lookAt(light.position, glm::vec3(0.0f), glm::vec3(0.0, 1.0, 0.0));
    lightSpaceMatrix = lightProjection * lightView;
}


void Application::renderModel() {
    // Configurar el shader de iluminación con sombras
    shaderProgram.use();
    shaderProgram.setMat4("view", camera.GetViewMatrix());
    shaderProgram.setMat4("projection", glm::perspective(glm::radians(camera.GetZoom()), 800.0f / 600.0f, 0.1f, 100.0f));

    // Configurar los parámetros de la luz global
    shaderProgram.setVec3("ambientColor", globalLight.ambientColor);
    shaderProgram.setFloat("ambientIntensity", globalLight.ambientIntensity);

    // Configurar los parámetros de la luz direccional, puntual o focal
    shaderProgram.setVec3("light.position", light.position);
    shaderProgram.setVec3("light.direction", light.direction);
    shaderProgram.setVec3("light.color", light.color);
    shaderProgram.setFloat("light.intensity", light.intensity);
    shaderProgram.setFloat("light.radius", light.radius);
    shaderProgram.setFloat("light.cutoff", light.cutoff);

    // Configurar la textura de sombras y la matriz de espacio de luz
    shaderProgram.setInt("shadowMap", 1); // Asegúrate de que coincida con el índice de textura utilizado para el mapa de sombras
    shaderProgram.setMat4("lightSpaceMatrix", lightSpaceMatrix); // Matriz de espacio de luz (lightSpaceMatrix)

    // Renderizar el modelo
    modelLoader.Draw(shaderProgram);
}

void Application::renderSkybox(ShaderProgram& shader) {
    if (skyboxLoader && skyboxLoader->IsModelLoaded()) {
        glDepthFunc(GL_LEQUAL); // Establecer antes de renderizar la skybox

        shader.use();
        glm::mat4 view = glm::mat4(glm::mat3(camera.GetViewMatrix())); // Sin la traslación
        glm::mat4 projection = glm::perspective(glm::radians(camera.GetZoom()), 800.0f / 600.0f, 0.1f, 100.0f);
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // Desactivar la prueba de profundidad para la skybox
        glDepthMask(GL_FALSE);
        skyboxLoader->Draw(shader); // Asumiendo que skyboxLoader->Draw() maneja la renderización del skybox con el shader adecuado
        glDepthMask(GL_TRUE);

        glDepthFunc(GL_LESS); // Restablecer después de renderizar la skybox
    }
}

bool Application::initializeGLFW() {
    if (!glfwInit()) {
        return false;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window = glfwCreateWindow(1920, 1080, "OpenGL App", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    return true;
}

bool Application::initializeOpenGL() {
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        return false;
    }

    glEnable(GL_DEPTH_TEST);
    return true;
}

void Application::setupShaders() {
    // Inicializar shaders
    shaderProgram.createShaderFromFile("shader.vert", "shader.frag");
    luces.createShaderFromFile("light_vertex_shader.vert", "light_fragment_shader.frag");
    skyboxShader.createShaderFromFile("skybox_vertex_shader.vert", "skybox_fragment_shader.frag");

    // Inicializar el cubo
    initCube(); // Ensure this matches the function definition in cubo.cpp
}


void Application::setupImGui() {
    imguiView.init(window);
    imguiView.setModelLoader(&modelLoader);
    imguiView.setSkyboxLoader(skyboxLoader);
}

void Application::cleanup() {
    imguiView.shutdown();
    glfwDestroyWindow(window);
    glfwTerminate();
}

void Application::framebufferSizeCallback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void Application::scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    app->ProcessScroll(yoffset);
}

void Application::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    if (action == GLFW_PRESS) {
        app->IsKeyPressed(key) = true;
    }
    else if (action == GLFW_RELEASE) {
        app->IsKeyPressed(key) = false;
    }
}

bool& Application::IsKeyPressed(int key) {
    return keys[key];
}

void Application::ProcessScroll(double yOffset) {
    camera.ProcessMouseScroll(yOffset);
}

int main() {
    Application app;
    app.run();
    return 0;
}