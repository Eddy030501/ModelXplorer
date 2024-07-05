OpenGL Project README
Overview
This project is an OpenGL application developed in C++ for loading 3D models, displaying a skybox, and managing lights using ImGui for the user interface. It utilizes several libraries including Assimp, Glad, GLFW, GLM, ImGui, IrrKlang, KHR, stb, and tinydialog.

Features
Model Loading: Load 3D models in glTF format using Assimp.
Skybox: Render a skybox using cubemaps.
Lights: Control lighting parameters such as position, color, and intensity.
ImGui Integration: User-friendly interface for interacting with models, skybox, and lights.
Audio Integration: Basic integration with IrrKlang for audio playback.
Project Structure
bash
Copiar código
- /include        # Header files
- /src            # Source files
- /shaders        # Shader files
- /libraries      # External libraries (Assimp, Glad, GLFW, GLM, ImGui, IrrKlang, KHR, stb, tinydialog)
- /build          # Build configurations and output
- README.md       # Project documentation
Dependencies
Assimp: Open Asset Import Library for loading various 3D model formats.
Glad: OpenGL loader generator.
GLFW: OpenGL library for creating windows and handling input.
GLM: Mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.
ImGui: Immediate Mode Graphical User Interface library.
IrrKlang: Cross-platform sound library.
KHR: Khronos Group headers for OpenGL extensions.
stb: Single-file public domain libraries for C/C++.
Installation and Building
Clone the repository:

bash
Copiar código
git clone https://github.com/Eddy030501/ModelXplorer/tree/main
cd repository
Build the project (assuming CMake is used):

bash
Copiar código
mkdir build
cd build
cmake ..
make
Run the application:

bash
Copiar código
./your_application
Usage
Use ImGui interface to load glTF models and control lighting parameters.
Navigate the scene using standard camera controls (WASD keys for movement, mouse for rotation).
Interact with skybox and model properties dynamically.
Contributions
Contributions are welcome. Please fork the repository and submit pull requests for any enhancements or bug fixes.

License
This project is licensed under the MIT License. See LICENSE file for details.
