#pragma once

#include <vector>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shaderClass.h"
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include <optional>
#include <unordered_map>

class Model {
public:
    VAO vao;
    std::optional<VBO> vbo;
    std::optional<EBO> ebo;
    std::vector<GLfloat> vertices;
    std::vector<GLuint> indices;

    Model(const std::string& path);
    void Draw(Shader& shader);

private:
    void computeTangents();
};