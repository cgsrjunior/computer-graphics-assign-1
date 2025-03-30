#include "Camera.h"

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

void Camera::updateCameraVectors() {
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

void Camera::centerOnObject(const glm::vec3& center, float radius) {
    // Calcula a distância necessária para o objeto caber no view frustum
    float fovY = glm::radians(Zoom);
    float distance = radius / std::tan(fovY * 0.5f);
    
    // Adiciona uma margem de 20%
    distance *= 1.2f;
    
    // Posiciona a câmera olhando para o centro do objeto
    Position = center - glm::vec3(0.0f, 0.0f, distance);
    Front = glm::normalize(center - Position);
    updateCameraVectors();
}