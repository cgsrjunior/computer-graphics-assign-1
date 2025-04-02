#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <string>

class Camera {
public:
    // Construtor com vetores
    Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 3.0f), 
           glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), 
           float yaw = -90.0f, float pitch = 0.0f);
    
    // Retorna a matriz view calculada
    glm::mat4 GetViewMatrix();
    glm::mat4 GetViewMatrix(glm::vec3 targetPos);
    
    // Processa entrada do teclado
    void ProcessKeyboard(int direction, float deltaTime);
    
    // Processa entrada do mouse
    void ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch = true);
    
    // Processa scroll do mouse (zoom)
    void ProcessMouseScroll(float yoffset);
    void centerOnObject(const glm::vec3& center, float radius);

    // Getters
    inline glm::vec3 GetPosition() const { return Position; };
    inline glm::vec3 GetFront() const { return Front; };
    inline float GetZoom() const { return Zoom; };

    // Setters
    inline void SetPosition(glm::vec3 pos) { Position = pos; };
    inline void SetFront(glm::vec3 pos) { Front = pos; };
    inline void SetUp(glm::vec3 pos) { Up = pos; };
    inline void SetRight(glm::vec3 pos) { Right = pos; };
    inline void SetWorldUp(glm::vec3 pos) { WorldUp = pos; };

    std::string getPositionInfo() const {
        return "Pos: (" + std::to_string(Position.x) + ", " 
                      + std::to_string(Position.y) + ", " 
                      + std::to_string(Position.z) + ")";
    }
    
    std::string getOrientationInfo() const {
        return "Front: (" + std::to_string(Front.x) + ", " 
                         + std::to_string(Front.y) + ", " 
                         + std::to_string(Front.z) + ")";
    }

    // Rotação em torno dos eixos locais
    void rotateLocalX(float angle);
    void rotateLocalY(float angle);
    void rotateLocalZ(float angle);
    void updateCameraRotationOnAxis();
    void UpdateCameraPosition(glm::vec3 targetPos);

private:
    // Atualiza os vetores da câmera
    void updateCameraVectors();
    
    // Atributos da câmera
    glm::vec3 Position;
    glm::vec3 Front;
    glm::vec3 Up;
    glm::vec3 Right;
    glm::vec3 WorldUp;
    
    // Ângulos de Euler
    float Yaw;
    float Pitch = 30.0f;
    float radius = 5.0f; // Distância fixa entre câmera e objeto
    
    // Opções da câmera
    float MovementSpeed;
    float MouseSensitivity;
    float Zoom;
};