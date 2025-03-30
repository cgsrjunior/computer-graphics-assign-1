#pragma once
#include "config.h"
#include "Vertex.h"
#include "Material.h"

class Shader;

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const Material& material);
    void Draw() const;
    void Draw(Shader& shader) const;
    // Adicione isto para permitir acesso aos vértices
    const std::vector<Vertex>& getVertices() const { return vertices; }
    
private:
    unsigned int VAO, VBO;
    std::vector<Vertex> vertices;
    Material material;
    
    void setupMesh();
};