#ifndef RENDERER_H
#define RENDERER_H

#include "../include/glad/glad.h"
#include "../dependencies/glfw/include/glfw3.h"
#include <glm/glm.hpp>


class Renderer {
public:
    Renderer();
    void init();
    void renderCube(const glm::vec3& position, const glm::vec3& color);
    void renderFace(const glm::vec3& position, const glm::vec3& color, int face);
    void cleanup();
    GLuint shaderProgram;

    enum Face {
        LEFT_FACE, RIGHT_FACE,
        BOTTOM_FACE, TOP_FACE,
        BACK_FACE, FRONT_FACE
    };

private:
    GLuint VAO, VBO, EBO;
    GLuint loadShaders(const char* vertex_file_path, const char* fragment_file_path);
    void setupCube(glm::vec3 color);
};

#endif
