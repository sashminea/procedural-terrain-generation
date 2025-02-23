#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Chunk.h"


// Constructor with vectors
Camera::Camera(glm::vec3 startPosition, glm::vec3 startUp, float startYaw, float startPitch)
    : front(glm::vec3(0.0f, 0.0f, -1.0f)), 
      movementSpeed(5.0f), mouseSensitivity(0.1f), zoom(45.0f) {
    position = startPosition;
    worldUp = startUp;
    yaw = startYaw;
    pitch = startPitch;
    UpdateCameraVectors();
}

// Returns the view matrix
glm::mat4 Camera::GetViewMatrix() {
    return glm::lookAt(position, position + front, up);
}

// Processes mouse movement
void Camera::ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch) {
    xOffset *= mouseSensitivity;
    yOffset *= mouseSensitivity;

    yaw += xOffset;
    pitch += yOffset;

    if (constrainPitch) {
        if (pitch > 89.0f) pitch = 89.0f;
        if (pitch < -89.0f) pitch = -89.0f;
    }

    UpdateCameraVectors();
}

// Processes keyboard input
void Camera::ProcessKeyboard(const char direction, float deltaTime) {
    float velocity = movementSpeed * deltaTime;

    if (direction == 'W') position += front * velocity;
    if (direction == 'S') position -= front * velocity;
    if (direction == 'A') position -= right * velocity;
    if (direction == 'D') position += right * velocity;
}

// Updates camera vectors based on Euler angles
void Camera::UpdateCameraVectors() {
    glm::vec3 newFront;
    newFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    newFront.y = sin(glm::radians(pitch));
    newFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front = glm::normalize(newFront);

    right = glm::normalize(glm::cross(front, worldUp));  
    up = glm::normalize(glm::cross(right, front));
}

glm::vec3 Camera::GetChunkCoordinates() const {
    // int chunkX = static_cast<int>(position.x) / CHUNK_DEPTH;
    // int chunkY = static_cast<int>(position.y) / CHUNK_DEPTH;  // Adjust for height if needed
    // int chunkZ = static_cast<int>(position.z) / CHUNK_DEPTH;
    
    float  chunkX = (position.x);
    float  chunkY = (position.y);  // Adjust for height if needed
    float  chunkZ = (position.z);

    return glm::vec3(chunkX, chunkY, chunkZ);
}
