#pragma once
#include "config.h"
#include "Mesh.h"

class Shader;

class Model {
public:
    Model();
    Model(const std::string& filepath);
    void Draw() const;
    void Draw(Shader& shader) const;
    glm::vec3 getMinBounds() const { return minBounds; }
    glm::vec3 getMaxBounds() const { return maxBounds; }
    glm::vec3 getCenter() const { return (minBounds + maxBounds) * 0.5f; }
    float getBoundingRadius() const { return boundingRadius; }
    void calculateBoundingBox();
    inline void loadModelFromFile(const std::string& filepath) { loadModel(filepath); };

    
private:
    glm::vec3 minBounds;
    glm::vec3 maxBounds;
    float boundingRadius;
    std::vector<Mesh> meshes;
    void loadModel(const std::string& filepath);
};