#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp> // Para glm::value_ptr
#include <iostream>

std::string readFile(const std::string& filePath);

class ShaderProgram {
public:
    ShaderProgram();
    ~ShaderProgram();

    void setInt(const char* name, int value);
    void setMat4(const std::string& name, const glm::mat4& mat);

    bool createShaderFromSource(const char* vertexShaderSource, const char* fragmentShaderSource);
    bool createShaderFromFile(const char* vertexShaderPath, const char* fragmentShaderPath);

    void use();
    void cleanup();
    GLuint getProgramID() const { return programID; }
    void setVec3(const std::string& name, const glm::vec3& value) const;
    void setFloat(const std::string& name, float value) const;
    void setInt(const std::string& name, int value);
    void setVec3(const std::string& name, const glm::vec3& value);
    void setFloat(const std::string& name, float value);
private:
    GLuint programID;
    GLuint vertexShaderID;
    GLuint fragmentShaderID;

    GLuint compileShader(const char* source, GLenum type);
    void checkCompileErrors(GLuint shader, const std::string& type);
};

#endif // SHADER_PROGRAM_H
