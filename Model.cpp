// === Model.cpp ===
#define TINYOBJLOADER_IMPLEMENTATION
#include "Model.h"
#include "tiny_obj_loader.h"
#include <iostream>

Model::Model(const std::string& path) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str());

    if (!warn.empty()) std::cout << "[TINYOBJ WARNING] " << warn << std::endl;
    if (!err.empty()) std::cerr << "[TINYOBJ ERROR] " << err << std::endl;
    if (!ret) throw std::runtime_error("Failed to load obj model: " + path);

    std::cout << "Loading model: " << path << std::endl;
    std::cout << "Shapes found: " << shapes.size() << std::endl;

    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            // Dodaj werteksy (pozycje)
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
            vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);
            
            // Dodaj tekstury UV (jeśli dostępne)
            if (index.texcoord_index >= 0 && !attrib.texcoords.empty()) {
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(0.0f);
            }
            
            // Dodaj normalne (jeśli dostępne)
            if (index.normal_index >= 0 && !attrib.normals.empty()) {
                vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
                vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
            } else {
                vertices.push_back(0.0f);
                vertices.push_back(1.0f);
                vertices.push_back(0.0f);
            }
            
            // Dodaj tangent i bitangent (wypełnione zerami na razie)
            vertices.push_back(1.0f); vertices.push_back(0.0f); vertices.push_back(0.0f); // tangent
            vertices.push_back(0.0f); vertices.push_back(0.0f); vertices.push_back(1.0f); // bitangent
            
            // Dodaj kolejny indeks (po prostu kolejny numer werteksu)
            indices.push_back((GLuint)indices.size());
        }
    }

    std::cout << "Vertices: " << vertices.size() << ", Indices: " << indices.size() << std::endl;    vao.Bind();
    vbo = VBO(vertices.data(), vertices.size() * sizeof(GLfloat));
    ebo = EBO(indices.data(), indices.size() * sizeof(GLuint));

    // Position (location 0)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    // Texture coordinates (location 1)
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    // Normals (location 2)
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(5 * sizeof(float)));
    glEnableVertexAttribArray(2);
    
    // Tangent (location 3)
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(8 * sizeof(float)));
    glEnableVertexAttribArray(3);
    
    // Bitangent (location 4)
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