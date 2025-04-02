#include "Camera.h"
#include <glm/gtx/string_cast.hpp>

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch) : 
    Front(glm::vec3(0.0f, 0.0f, -1.0f)), 
    MovementSpeed(2.5f), 
    MouseSensitivity(0.1f), 
    Zoom(45.0f)
{
    Position = position;
    WorldUp = up;
    Yaw = yaw;
    Pitch = pitch;
    updateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(Position, Position + Front, Up);
}

void Camera::ProcessKeyboard(int direction, float deltaTime) {
    float velocity = MovementSpeed * deltaTime;
    
    if (direction == 0)      // Frente (W)
        Position += Front * velocity;
    if (direction == 1)      // Trás (S)
        Position -= Front * velocity;
    if (direction == 2)      // Esquerda (A)
        Position -= Right * velocity;
    if (direction == 3)      // Direita (D)
        Position += Right * velocity;
    if (direction == 4)      // Cima (Space)
        Position += Up * velocity;
    if (direction == 5)      // Baixo (Left Shift)
        Position -= Up * velocity;
}

void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    Yaw += xoffset;
    Pitch += yoffset;

    // Limita o pitch para evitar flip
    if (constrainPitch) {
        if (Pitch > 89.0f)
            Pitch = 89.0f;
        if (Pitch < -89.0f)
            Pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::ProcessMouseScroll(float yoffset) {
    Zoom -= yoffset;
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}

void Camera::rotateLocalX(float angle) {
    // Roll - rotação em torno do eixo Front (Z local)
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), Front);
    // Aplica a rotação aos vetores Right e Up
    Right = glm::normalize(glm::vec3(rotation * glm::vec4(Right, 0.0f)));
    Up = glm::normalize(glm::vec3(rotation * glm::vec4(Up, 0.0f)));
    // Garante ortogonalidade (opcional, mas recomendado)
    Up = glm::normalize(glm::cross(Front, Right));
    Right = glm::normalize(glm::cross(Up, Front));
}

void Camera::rotateLocalY(float angle) {
    // Pitch - rotação em torno do eixo Right (X local)
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), Right);
    Front = glm::normalize(glm::vec3(rotation * glm::vec4(Front, 0.0f)));
    Up = glm::normalize(glm::cross(Right, Front)); // Mantém Up perpendicular
}

void Camera::rotateLocalZ(float angle) {
    // Yaw - rotação em torno do eixo Up (Y local)
    glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(angle), Up);
    Front = glm::normalize(glm::vec3(rotation * glm::vec4(Front, 0.0f)));
    Right = glm::normalize(glm::cross(Front, Up)); // Recalcula Right
}


// Implementações similares para Y e Z
void Camera::updateCameraVectors()
{
    // Calcula novo vetor Front
    glm::vec3 front;
    front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    front.y = sin(glm::radians(Pitch));
    front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
    Front = glm::normalize(front);
    
    // Recalcula Right e Up
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

// Implementações similares para Y e Z
void Camera::updateCameraRotationOnAxis()
{
    // Garante que todos os vetores estão normalizados e ortogonais
    Front = glm::normalize(Front);
    Right = glm::normalize(glm::cross(Front, WorldUp));
    Up = glm::normalize(glm::cross(Right, Front));
}

void Camera::centerOnObject(const glm::vec3& center, float radius) {
    // Calcula distância baseada no tamanho do objeto e FOV
    float fovRad = glm::radians(Zoom);
    float distance = (radius * 2.0f) / tan(fovRad * 0.5f);
    
    // Posiciona a câmera atrás do objeto
    Position = center - glm::vec3(0.0f, 0.0f, distance);
    
    // Ajusta a altura para uma visualização melhor
    Position.y = center.y + radius * 0.5f;
    
    // Mantém o foco no centro do objeto
    Front = glm::normalize(center - Position);
    WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    
    updateCameraVectors();
}