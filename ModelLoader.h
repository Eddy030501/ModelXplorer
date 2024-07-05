#pragma once

#include "Texture.h"
#include <string>
#include <vector>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Mesh.h"
#include "ShaderProgram.h"
#include <glm/glm.hpp>

class ModelLoader {
public:
    ModelLoader();
    ~ModelLoader();

    bool LoadModel(const std::string& filePath);
    void Draw(ShaderProgram& shader);
    void SetModelPosition(const glm::vec3& position);
    glm::vec3 GetModelPosition() const;
    void SetModelRotation(const glm::vec3& rotation);
    glm::vec3 GetModelRotation() const;
    void SetModelScale(const glm::vec3& scale); // Nuevo método para establecer la escala del modelo
    glm::vec3 GetModelScale() const; // Nuevo método para obtener la escala del modelo
    bool IsModelLoaded() const;
    std::string GetModelPath() const;

private:
    std::string modelDirectory;
    std::string modelPath;
    void processNode(aiNode* node, const aiScene* scene);
    Mesh processMesh(aiMesh* mesh, const aiScene* scene);
    void setupMeshes(const aiScene* scene);
    std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName);
    unsigned int TextureFromFile(const char* path, const std::string& directory);

    GLuint VAO, VBO, EBO;
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> loadedTextures;
    std::vector<Mesh> meshes;
    glm::vec3 modelPosition;
    glm::vec3 modelRotation;
    glm::vec3 modelScale; // Nuevo miembro para almacenar la escala del modelo
};
