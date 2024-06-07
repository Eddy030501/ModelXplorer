#include "shaderClass.h"

std::string read_file_contents(const char* filename) {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    }
    else {
        // Manejar la excepción aquí
        std::cerr << "Error: Unable to open file: " << filename << std::endl;
        // Realizar alguna acción alternativa o lanzar una excepción personalizada
        throw std::runtime_error("Unable to open file: " + std::string(filename));
    }
}
Shader::Shader(const char* vertexFile, const char* fragmentFile) {
    std::string vertexCode = read_file_contents(vertexFile);
    std::string fragmentCode = read_file_contents(fragmentFile);
    const char* vertexSource = vertexCode.c_str();
    const char* fragmentSource = fragmentCode.c_str();

    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexSource, NULL);
    glCompileShader(vertexShader);
   

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
    glCompileShader(fragmentShader);
    check_compile_errors(fragmentShader, "FRAGMENT");

    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);
    check_link_errors(ID);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::Activate() {
    glUseProgram(ID);
}

void Shader::Delete() {
    glDeleteProgram(ID);
}

void Shader::check_compile_errors(GLuint shader, std::string type) {
    GLint success;
    GLchar infoLog[1024];
    if (type == "VERTEX") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error("Vertex shader compilation failed:\n" + std::string(infoLog));
        }
    }
    else if (type == "FRAGMENT") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            throw std::runtime_error("Fragment shader compilation failed:\n" + std::string(infoLog));
        }
    }
}

void Shader::check_link_errors(GLuint program) {
    GLint success;
    GLchar infoLog[1024];
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(program, 1024, NULL, infoLog);
        throw std::runtime_error("Shader program linking failed:\n" + std::string(infoLog));
    }
}
