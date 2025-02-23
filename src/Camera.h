#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
public:
    // Camera attributes
    glm::vec3 position;
    glm::vec3 front;
    glm::vec3 up;
    glm::vec3 right;
    glm::vec3 worldUp;
    
    // Euler Angles
    float yaw;
    float pitch;
    
    // Camera settings
    float movementSpeed;
    float mouseSensitivity;
    float zoom;
    
    // Constructor
    Camera(glm::vec3 startPosition, glm::vec3 startUp, float startYaw, float startPitch);

    // Returns the view matrix
    glm::mat4 GetViewMatrix();

    // Processes input received from a mouse input system.
    void ProcessMouseMovement(float xOffset, float yOffset, bool constrainPitch = true);
    // Processes input received from a keyboard input system.
    void ProcessKeyboard(const char direction, float deltaTime);

     // Get the current chunk coordinates based on camera position
    glm::vec3 GetChunkCoordinates() const;

private:
    // Updates camera vectors based on Euler angles
    void UpdateCameraVectors();
};

#endif
