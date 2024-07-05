---


---

<h1 id="opengl-project-readme">OpenGL Project README</h1>
<h2 id="overview">Overview</h2>
<p>This project is an OpenGL application developed in C++ for loading 3D models, displaying a skybox, and managing lights using ImGui for the user interface. It utilizes several libraries including Assimp, Glad, GLFW, GLM, ImGui, IrrKlang, KHR, stb, and tinydialog.</p>
<h2 id="features">Features</h2>
<ul>
<li><strong>Model Loading</strong>: Load 3D models in glTF format using Assimp.</li>
<li><strong>Skybox</strong>: Render a skybox using cubemaps.</li>
<li><strong>Lights</strong>: Control lighting parameters such as position, color, and intensity.</li>
<li><strong>ImGui Integration</strong>: User-friendly interface for interacting with models, skybox, and lights.</li>
<li><strong>Audio Integration</strong>: Basic integration with IrrKlang for audio playback.</li>
</ul>
<h2 id="project-structure">Project Structure</h2>
<p>bash</p>
<p>Copiar código</p>
<p><code>/include # Header files /src # Source files /shaders # Shader files /libraries # External libraries (Assimp, Glad, GLFW, GLM, ImGui, IrrKlang, KHR, stb, tinydialog) /build # Build configurations and output README.md # Project documentation</code></p>
<h2 id="dependencies">Dependencies</h2>
<ul>
<li><strong>Assimp</strong>: Open Asset Import Library for loading various 3D model formats.</li>
<li><strong>Glad</strong>: OpenGL loader generator.</li>
<li><strong>GLFW</strong>: OpenGL library for creating windows and handling input.</li>
<li><strong>GLM</strong>: Mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.</li>
<li><strong>ImGui</strong>: Immediate Mode Graphical User Interface library.</li>
<li><strong>IrrKlang</strong>: Cross-platform sound library.</li>
<li><strong>KHR</strong>: Khronos Group headers for OpenGL extensions.</li>
<li><strong>stb</strong>: Single-file public domain libraries for C/C++.</li>
</ul>
<h2 id="installation-and-building">Installation and Building</h2>
<ol>
<li>
<p><strong>Clone the repository</strong>:</p>
<p>bash</p>
<p>Copiar código</p>
<p><code>git clone https://github.com/your/repository.git cd repository</code></p>
</li>
<li>
<p><strong>Build the project</strong> (assuming CMake is used):</p>
<p>bash</p>
<p>Copiar código</p>
<p><code>mkdir build cd build cmake .. make</code></p>
</li>
<li>
<p><strong>Run the application</strong>:</p>
<p>bash</p>
<p>Copiar código</p>
<p><code>./your_application</code></p>
</li>
</ol>
<h2 id="usage">Usage</h2>
<ul>
<li>Use ImGui interface to load glTF models and control lighting parameters.</li>
<li>Navigate the scene using standard camera controls (WASD keys for movement, mouse for rotation).</li>
<li>Interact with skybox and model properties dynamically.</li>
</ul>
<h2 id="contributions">Contributions</h2>
<p>Contributions are welcome. Please fork the repository and submit pull requests for any enhancements or bug fixes.</p>

