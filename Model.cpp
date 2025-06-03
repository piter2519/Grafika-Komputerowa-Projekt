// === Model.cpp (poprawiony tangent/bitangent) ===
#include "Model.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"
#include <unordered_map>
#include <iostream>

struct VertexKey {
    int vIdx, tIdx, nIdx;
    bool operator==(const VertexKey& other) const {
        return vIdx == other.vIdx && tIdx == other.tIdx && nIdx == other.nIdx;
    }
};

namespace std {
    template <>
    struct hash<VertexKey> {
        size_t operator()(const VertexKey& k) const {
            return ((std::hash<int>()(k.vIdx) ^ (std::hash<int>()(k.tIdx) << 1)) >> 1) ^ (std::hash<int>()(k.nIdx) << 1);
        }
    };
}

Model::Model(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

    if (!warn.empty()) std::cout << "[TINYOBJ WARNING] " << warn << std::endl;
    if (!err.empty()) std::cerr << "[TINYOBJ ERROR] " << err << std::endl;
    if (!ret) throw std::runtime_error("Failed to load obj model: " + path);

    std::unordered_map<VertexKey, GLuint> vertexMap;
    std::vector<glm::vec3> positions;
    std::vector<glm::vec2> texcoords;
    std::vector<glm::vec3> normals;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            VertexKey key{ index.vertex_index, index.texcoord_index, index.normal_index };
            if (vertexMap.count(key) == 0) {
                glm::vec3 pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
                };
                glm::vec2 uv = { 0.0f, 0.0f };
                if (index.texcoord_index >= 0 && !attrib.texcoords.empty()) {
                    uv = {
                        attrib.texcoords[2 * index.texcoord_index + 0],
                        1.0f - attrib.texcoords[2 * index.texcoord_index + 1] // odwrócenie osi Y
                    };
                }
                glm::vec3 norm = { 0.0f, 1.0f, 0.0f };
                if (index.normal_index >= 0 && !attrib.normals.empty()) {
                    norm = {
                        attrib.normals[3 * index.normal_index + 0],
                        attrib.normals[3 * index.normal_index + 1],
                        attrib.normals[3 * index.normal_index + 2]
                    };
                }

                positions.push_back(pos);
                texcoords.push_back(uv);
                normals.push_back(norm);

                GLuint newIndex = static_cast<GLuint>(positions.size() - 1);
                vertexMap[key] = newIndex;
                indices.push_back(newIndex);
            }
            else {
                indices.push_back(vertexMap[key]);
            }
        }
    }

    std::vector<glm::vec3> tangents(positions.size(), glm::vec3(0.0f));
    std::vector<glm::vec3> bitangents(positions.size(), glm::vec3(0.0f));

    for (size_t i = 0; i < indices.size(); i += 3) {
        GLuint i0 = indices[i];
        GLuint i1 = indices[i + 1];
        GLuint i2 = indices[i + 2];

        const glm::vec3& p0 = positions[i0];
        const glm::vec3& p1 = positions[i1];
        const glm::vec3& p2 = positions[i2];
        const glm::vec2& uv0 = texcoords[i0];
        const glm::vec2& uv1 = texcoords[i1];
        const glm::vec2& uv2 = texcoords[i2];

        glm::vec3 edge1 = p1 - p0;
        glm::vec3 edge2 = p2 - p0;
        glm::vec2 deltaUV1 = uv1 - uv0;
        glm::vec2 deltaUV2 = uv2 - uv0;

        float f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

        glm::vec3 tangent = f * (deltaUV2.y * edge1 - deltaUV1.y * edge2);
        glm::vec3 bitangent = -f * (-deltaUV2.x * edge1 + deltaUV1.x * edge2); // odwrócony bitangent

        tangents[i0] += tangent; bitangents[i0] += bitangent;
        tangents[i1] += tangent; bitangents[i1] += bitangent;
        tangents[i2] += tangent; bitangents[i2] += bitangent;
    }

    for (size_t i = 0; i < positions.size(); ++i) {
        vertices.push_back(positions[i].x);
        vertices.push_back(positions[i].y);
        vertices.push_back(positions[i].z);
        vertices.push_back(texcoords[i].x);
        vertices.push_back(texcoords[i].y);
        vertices.push_back(normals[i].x);
        vertices.push_back(normals[i].y);
        vertices.push_back(normals[i].z);
        vertices.push_back(tangents[i].x);
        vertices.push_back(tangents[i].y);
        vertices.push_back(tangents[i].z);
        vertices.push_back(bitangents[i].x);
        vertices.push_back(bitangents[i].y);
        vertices.push_back(bitangents[i].z);
    }

    vao.Bind();
    vbo = VBO(vertices.data(), vertices.size() * sizeof(GLfloat));
    ebo = EBO(indices.data(), indices.size() * sizeof(GLuint));

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(11 * sizeof(float)));
    glEnableVertexAttribArray(4);

    vao.Unbind();
    vbo->Unbind();
    ebo->Unbind();
}

void Model::Draw(Shader& shader) {
    shader.Activate();
    vao.Bind();
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    vao.Unbind();
}
