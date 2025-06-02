// === PBRMaterial.h ===
#ifndef PBR_MATERIAL_H
#define PBR_MATERIAL_H
#include <iostream>
#include <string>
#include <glad/glad.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


struct PBRMaterial {
    GLuint albedo;
    GLuint normal;
    GLuint roughness;
};

GLuint loadTexturePBR(const char* path, GLenum forceFormat = 0) {
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int width, height, channels;
    unsigned char* data = stbi_load(path, &width, &height, &channels, 0);
    if (data) {
        std::cout << "Loaded texture: " << path << " (" << width << "x" << height << ", " << channels << " channels)" << std::endl;
        
        GLenum format;
        if (forceFormat) {
            format = forceFormat;
        } else if (channels == 1) {
            format = GL_RED;
        } else if (channels == 2) {
            format = GL_RG;
        } else if (channels == 3) {
            format = GL_RGB;
        } else if (channels == 4) {
            format = GL_RGBA;
        } else {
            std::cerr << "Unsupported texture format: " << path << " (channels: " << channels << ")" << std::endl;
            stbi_image_free(data);
            glDeleteTextures(1, &tex);
            return 0;
        }
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        
        // Sprawdź błędy OpenGL
        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL error loading texture " << path << ": " << err << std::endl;
        }
    }
    else {
        std::cerr << "Failed to load texture: " << path << std::endl;
        glDeleteTextures(1, &tex);
        return 0;
    }
    stbi_image_free(data);
    return tex;
}

PBRMaterial loadPBRMaterial(const std::string& directory, const std::string& baseName) {
    PBRMaterial material;
    material.albedo = loadTexturePBR((directory + baseName + "_basecolor.jpg").c_str());
    material.normal = loadTexturePBR((directory + baseName + "_normal.png").c_str());
    material.roughness = loadTexturePBR((directory + baseName + "_roughness.png").c_str());
    return material;
}

#endif
