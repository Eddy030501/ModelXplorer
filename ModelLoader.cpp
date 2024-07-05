#include "ModelLoader.h"
#include "Texture.h"
#include <iostream>
#include <glad/glad.h>
#include <stb_image.h>

ModelLoader::ModelLoader()
    : VAO(0), VBO(0), EBO(0), modelPosition(0.0f), modelRotation(0.0f), modelScale(1.0f) {}

ModelLoader::~ModelLoader() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    std::cout << "ModelLoader cleaned up." << std::endl;
}

bool ModelLoader::LoadModel(const std::string& filePath) {
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(filePath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
        std::cerr << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return false;
    }

    std::cout << "Model loaded successfully: " << filePath << std::endl;

    // Guardar la ruta del modelo cargado
    modelPath = filePath;

    // Obtener el directorio del modelo
    modelDirectory = filePath.substr(0, filePath.find_last_of("/\\") + 1);

    processNode(scene->mRootNode, scene);
    setupMeshes(scene);

    // Asegurarse de que la escala se configure correctamente para un skybox
    modelScale = glm::vec3(1.0f); // Escala predeterminada para skybox

    std::cout << "Mesh setup completed." << std::endl;

    return true;
}

void ModelLoader::Draw(ShaderProgram& shader) {
    shader.use();

    // Construir la matriz de modelo
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, modelPosition); // Aplicar la posición del modelo

    // Aplicar las rotaciones del modelo
    model = glm::rotate(model, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Rotación inicial
    model = glm::rotate(model, glm::radians(modelRotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
    model = glm::rotate(model, glm::radians(modelRotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
    model = glm::rotate(model, glm::radians(modelRotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

    // Aplicar la escala del modelo
    model = glm::scale(model, modelScale);

    // Pasar la matriz de modelo al shader
    shader.setMat4("model", model);

    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].Draw(shader);
    }
}
void ModelLoader::SetModelScale(const glm::vec3& scale) {
    modelScale = scale;
}

glm::vec3 ModelLoader::GetModelScale() const {
    return modelScale;
}


void ModelLoader::SetModelRotation(const glm::vec3& rotation) {
    modelRotation = rotation;
}

glm::vec3 ModelLoader::GetModelRotation() const {
    return modelRotation;
}

void ModelLoader::SetModelPosition(const glm::vec3& position) {
    modelPosition = position;
}

glm::vec3 ModelLoader::GetModelPosition() const {
    return modelPosition;
}

bool ModelLoader::IsModelLoaded() const {
    return !modelPath.empty();
}

std::string ModelLoader::GetModelPath() const {
    return modelPath;
}

void ModelLoader::processNode(aiNode* node, const aiScene* scene) {
    for (unsigned int i = 0; i < node->mNumMeshes; i++) {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++) {
        processNode(node->mChildren[i], scene);
    }
}

Mesh ModelLoader::processMesh(aiMesh* mesh, const aiScene* scene) {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
        Vertex vertex;
        glm::vec3 vector;

        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;

        if (mesh->HasNormals()) {
            vector.x = mesh->mNormals[i].x;
            vector.y = mesh->mNormals[i].y;
            vector.z = mesh->mNormals[i].z;
            vertex.Normal = vector;
        }

        if (mesh->mTextureCoords[0]) {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else {
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);
        }

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++) {
            indices.push_back(face.mIndices[j]);
        }
    }

    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    return Mesh(vertices, indices, textures);
}

void ModelLoader::setupMeshes(const aiScene* scene) {
    for (unsigned int i = 0; i < meshes.size(); i++) {
        meshes[i].setupMesh();
    }
}

std::vector<Texture> ModelLoader::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName) {
    std::vector<Texture> textures;

    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++) {
        aiString str;
        mat->GetTexture(type, i, &str);

        bool skip = false;
        for (unsigned int j = 0; j < loadedTextures.size(); j++) {
            if (std::strcmp(loadedTextures[j].path.C_Str(), str.C_Str()) == 0) {
                textures.push_back(loadedTextures[j]);
                skip = true;
                break;
            }
        }

        if (!skip) {
            Texture texture;
            texture.id = TextureFromFile(str.C_Str(), modelDirectory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            loadedTextures.push_back(texture);
        }
    }

    return textures;
}

unsigned int ModelLoader::TextureFromFile(const char* path, const std::string& directory) {
    std::string filename = std::string(path);
    filename = directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data) {
        GLenum format;
        if (nrComponents == 1) {
            format = GL_RED;
        }
        else if (nrComponents == 3) {
            format = GL_RGB;
        }
        else if (nrComponents == 4) {
            format = GL_RGBA;
        }

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else {
        std::cout << "Failed to load texture: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
