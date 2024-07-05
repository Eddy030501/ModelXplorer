#include "ShaderProgram.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

ShaderProgram::ShaderProgram() : programID(0), vertexShaderID(0), fragmentShaderID(0) {}

ShaderProgram::~ShaderProgram() {
    cleanup();
}
void ShaderProgram::setInt(const char* name, int value) {
    glUniform1i(glGetUniformLocation(programID, name), value);
}
void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) {
    glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, glm::value_ptr(value));
}

void ShaderProgram::setFloat(const std::string& name, float value) {
    glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) {
    GLint loc = glGetUniformLocation(programID, name.c_str());
    if (loc != -1) {
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }
    else {
        std::cerr << "Warning: Uniform '" << name << "' not found in shader program!" << std::endl;
    }
}
std::string readFile(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Error: Unable to open file " << filePath << std::endl;
        return "";
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

bool ShaderProgram::createShaderFromSource(const char* vertexShaderSource, const char* fragmentShaderSource) {
    vertexShaderID = compileShader(vertexShaderSource, GL_VERTEX_SHADER);
    fragmentShaderID = compileShader(fragmentShaderSource, GL_FRAGMENT_SHADER);

    programID = glCreateProgram();
    glAttachShader(programID, vertexShaderID);
    glAttachShader(programID, fragmentShaderID);
    glLinkProgram(programID);
    checkCompileErrors(programID, "PROGRAM");

    // Clean up shaders after linking
    glDeleteShader(vertexShaderID);
    glDeleteShader(fragmentShaderID);

    return true;
}

bool ShaderProgram::createShaderFromFile(const char* vertexShaderPath, const char* fragmentShaderPath) {
    std::string vertexShaderSource = readFile(vertexShaderPath);
    std::string fragmentShaderSource = readFile(fragmentShaderPath);

    return createShaderFromSource(vertexShaderSource.c_str(), fragmentShaderSource.c_str());
}

void ShaderProgram::use() {
    glUseProgram(programID);
}

void ShaderProgram::cleanup() {
    if (programID != 0) {
        glDeleteProgram(programID);
    }
}

GLuint ShaderProgram::compileShader(const char* source, GLenum type) {
    if (!source) {
        std::cerr << "Error: Shader source code is null." << std::endl;
        return 0;
    }

    GLuint shaderID = glCreateShader(type);
    if (!shaderID) {
        std::cerr << "Error: Failed to create shader of type " << type << "." << std::endl;
        return 0;
    }

    const char* shaderType = (type == GL_VERTEX_SHADER) ? "VERTEX" :
        (type == GL_FRAGMENT_SHADER) ? "FRAGMENT" :
        (type == GL_GEOMETRY_SHADER) ? "GEOMETRY" :
        "UNKNOWN";
    std::cout << "Compiling " << shaderType << " shader..." << std::endl;

    // Load shader source and compile
    glShaderSource(shaderID, 1, &source, nullptr);
    glCompileShader(shaderID);

    // Check compilation status
    GLint success;
    glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
    if (!success) {
        GLint logLength;
        glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &logLength);
        std::vector<GLchar> errorLog(logLength);
        glGetShaderInfoLog(shaderID, logLength, nullptr, errorLog.data());
        std::cerr << "Error compiling " << shaderType << " shader:\n" << errorLog.data() << std::endl;
        glDeleteShader(shaderID);
        return 0;
    }

    std::cout << "Shader compilation successful." << std::endl;

    return shaderID;
}

void ShaderProgram::checkCompileErrors(GLuint shader, const std::string& type) {
    GLint success;
    GLchar infoLog[1024];
    if (type != "PROGRAM") {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
