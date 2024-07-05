#include "Texture.h"
#include <iostream>
#include <glad/glad.h>
#include <stb_image.h>

std::vector<unsigned int> TextureLoader::LoadTextures(const std::vector<std::string>& texturePaths, const std::string& directory) {
    std::vector<unsigned int> textureIDs;

    for (const auto& path : texturePaths) {
        std::string filename = directory + '/' + path;

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

            // Configurar opciones de la textura
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else {
            std::cout << "Failed to load texture: " << path << std::endl;
            stbi_image_free(data);
            textureID = 0; // Indicar falla de carga con ID 0 (o manejar el error según tu necesidad)
        }

        textureIDs.push_back(textureID);
    }

    return textureIDs;
}
