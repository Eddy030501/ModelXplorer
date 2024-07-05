#pragma once
#ifndef TEXTURE_H
#define TEXTURE_H

#include <string>
#include <vector>
#include <assimp/types.h> // Incluye este archivo si usas Assimp directamente

struct Texture {
    unsigned int id;
    std::string type;
    aiString path; // Usa aiString en lugar de std::string si usas Assimp directamente
};

class TextureLoader {
public:
    static std::vector<unsigned int> LoadTextures(const std::vector<std::string>& texturePaths, const std::string& directory);
};

#endif // TEXTURE_H
