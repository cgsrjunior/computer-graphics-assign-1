#include "Model.h"
#include "Shader.h"
#include <fstream>
#include <sstream>
#include <iostream>

Model::Model(const std::string& filepath) {
    loadModel(filepath);
}

void Model::loadModel(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filepath << std::endl;
        return;
    }

    std::string line;
    std::string objectName;
    int triangleCount = 0;
    int materialCount = 0;
    Material currentMaterial;
    std::vector<Vertex> currentVertices;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string prefix;
        iss >> prefix;

        if (prefix == "Object") {
            iss >> prefix >> prefix; // Skip "name ="
            iss >> objectName;
            std::cout << "Loading object: " << objectName << std::endl;
        }
        else if (prefix == "#") {
            iss >> prefix >> prefix >> triangleCount; // triangles = <num>
        }
        else if (prefix == "Material") {
            iss >> prefix >> materialCount; // count = <num>
        }
        else if (prefix == "ambient") {
            iss >> prefix >> currentMaterial.ambient.r >> currentMaterial.ambient.g >> currentMaterial.ambient.b;
        }
        else if (prefix == "diffuse") {
            iss >> prefix >> currentMaterial.diffuse.r >> currentMaterial.diffuse.g >> currentMaterial.diffuse.b;
        }
        else if (prefix == "specular") {
            iss >> prefix >> currentMaterial.specular.r >> currentMaterial.specular.g >> currentMaterial.specular.b;
        }
        else if (prefix == "material") {
            iss >> prefix >> currentMaterial.shine;
        }
        else if (prefix == "v0" || prefix == "v1" || prefix == "v2") {
            Vertex vertex;
            iss >> vertex.Position.x >> vertex.Position.y >> vertex.Position.z
                >> vertex.Normal.x >> vertex.Normal.y >> vertex.Normal.z;
            
            int materialIndex;
            iss >> materialIndex;
            
            // Usamos a cor difusa como cor do vértice (pode ajustar)
            vertex.Color = currentMaterial.diffuse;
            
            currentVertices.push_back(vertex);
        }
        else if (prefix == "face") {
            // Quando encontramos uma nova face normal, podemos finalizar o triângulo atual
            if (currentVertices.size() >= 3) {
                meshes.emplace_back(currentVertices, currentMaterial);
                currentVertices.clear();
            }
        }
    }

    // Adiciona o último triângulo se houver
    if (!currentVertices.empty()) {
        meshes.emplace_back(currentVertices, currentMaterial);
    }

    std::cout << "Loaded " << meshes.size() << " meshes" << std::endl;

    //Create bbox
    calculateBoundingBox();
}

void Model::Draw() const {
    for (const auto& mesh : meshes) {
        mesh.Draw();
    }
}

// No método Draw() da classe Model
void Model::Draw(Shader& shader) const {
    for (const auto& mesh : meshes) {
        mesh.Draw(shader);
    }
}

void Model::calculateBoundingBox() {
    if (meshes.empty()) return;

    // Inicializa com valores extremos
    minBounds = glm::vec3(std::numeric_limits<float>::max());
    maxBounds = glm::vec3(std::numeric_limits<float>::lowest());

    // Percorre todas as malhas e vértices
    for (const auto& mesh : meshes) {
        for (const auto& vertex : mesh.getVertices()) {
            // Atualiza mínimos
            minBounds.x = std::min(minBounds.x, vertex.Position.x);
            minBounds.y = std::min(minBounds.y, vertex.Position.y);
            minBounds.z = std::min(minBounds.z, vertex.Position.z);
            
            // Atualiza máximos
            maxBounds.x = std::max(maxBounds.x, vertex.Position.x);
            maxBounds.y = std::max(maxBounds.y, vertex.Position.y);
            maxBounds.z = std::max(maxBounds.z, vertex.Position.z);
        }
    }

    // Calcula o raio do bounding sphere
    glm::vec3 size = maxBounds - minBounds;
    boundingRadius = glm::length(size) * 0.5f;
}