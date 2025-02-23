#include "Renderer.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

const int WIDTH = 800;
const int HEIGHT = 600;
glm::vec3 prevColor = glm::vec3(0.663f, 0.639f, 0.639f);;

// Vertex and Fragment Shader code as strings
const char* vertexShaderSource = R"(
#version 460 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;
out vec3 fragColor;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main() {
    fragColor = aColor;
    gl_Position = projection * view * model * vec4(aPos, 1.0);
})";

const char* fragmentShaderSource = R"(
#version 460 core
in vec3 fragColor;
out vec4 FragColor;
void main() {
    FragColor = vec4(fragColor, 1.0);
})";

Renderer::Renderer() : shaderProgram(0), VAO(0), VBO(0), EBO(0) {}

void Renderer::init() {
    std::cout << "Initializing Renderer..." << std::endl;

    shaderProgram = loadShaders(vertexShaderSource, fragmentShaderSource);
    if (shaderProgram == 0) {
        std::cerr << "Failed to load shaders!" << std::endl;
        return;
    }

    setupCube(glm::vec3(1.0f, 1.0f, 1.0f));

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);

    // Set up the view and projection matrices
    glm::mat4 view = glm::lookAt(glm::vec3(20.0f, 20.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

    glUseProgram(shaderProgram);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);
    glUseProgram(0);

    std::cout << "Renderer initialized successfully." << std::endl;
}

GLuint Renderer::loadShaders(const char* vertex_source, const char* fragment_source) {
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertex_source, nullptr);
    glCompileShader(vertexShader);

    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragment_source, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}

// void Renderer::setupCube() {
    
//     float vertices[] = {
//         // positions          // colors
//         -0.5f, -0.5f, -0.5f,  1.0f, 0.0f, 0.0f,
//          0.5f, -0.5f, -0.5f,  0.0f, 1.0f, 0.0f,
//          0.5f,  0.5f, -0.5f,  0.0f, 0.0f, 1.0f,
//         -0.5f,  0.5f, -0.5f,  1.0f, 1.0f, 0.0f,
//         -0.5f, -0.5f,  0.5f,  1.0f, 0.0f, 1.0f,
//          0.5f, -0.5f,  0.5f,  0.0f, 1.0f, 1.0f,
//          0.5f,  0.5f,  0.5f,  1.0f, 1.0f, 1.0f,
//         -0.5f,  0.5f,  0.5f,  0.0f, 0.0f, 0.0f
//     };

//     unsigned int indices[] = {
//         0, 1, 2, 2, 3, 0,
//         4, 5, 6, 6, 7, 4,
//         0, 1, 5, 5, 4, 0,
//         2, 3, 7, 7, 6, 2,
//         0, 3, 7, 7, 4, 0,
//         1, 2, 6, 6, 5, 1
//     };

//     glGenVertexArrays(1, &VAO);
//     glGenBuffers(1, &VBO);
//     glGenBuffers(1, &EBO);

//     glBindVertexArray(VAO);

//     glBindBuffer(GL_ARRAY_BUFFER, VBO);
//     glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

//     glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
//     glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

//     glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
//     glEnableVertexAttribArray(0);

//     glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
//     glEnableVertexAttribArray(1);

//     glBindVertexArray(0);
// }

void Renderer::setupCube(glm::vec3 color) {
    // Define the vertices for each face with the color passed in as parameters
    float faceVertices[] = {
        // Left Face (-X)
        -0.5f, -0.5f, -0.5f, color.r * 0.5f, color.g * 0.5f, color.b * 0.5f,  // Darkest
        -0.5f,  0.5f, -0.5f, color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, // Mid
        -0.5f,  0.5f,  0.5f, color.r, color.g, color.b,  // Lightest
        -0.5f, -0.5f,  0.5f, color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, // Mid

        // Right Face (+X)
        0.5f, -0.5f, -0.5f, color.r * 0.5f, color.g * 0.5f, color.b * 0.5f,  // Darkest
        0.5f,  0.5f, -0.5f, color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, // Mid
        0.5f,  0.5f,  0.5f, color.r, color.g, color.b,  // Lightest
        0.5f, -0.5f,  0.5f, color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, // Mid

        // Bottom Face (-Y)
        -0.5f, -0.5f, -0.5f, color.r * 0.5f, color.g * 0.5f, color.b * 0.5f,  // Darkest
        0.5f, -0.5f, -0.5f, color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, // Mid
        0.5f, -0.5f,  0.5f, color.r, color.g, color.b,  // Lightest
        -0.5f, -0.5f,  0.5f, color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, // Mid

        // Top Face (+Y)
        -0.5f, 0.5f, -0.5f, color.r, color.g, color.b,  // Lightest
        0.5f, 0.5f, -0.5f, color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, // Mid
        0.5f, 0.5f, 0.5f, color.r * 0.5f, color.g * 0.5f, color.b * 0.5f,  // Darkest
        -0.5f, 0.5f, 0.5f, color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, // Mid

        // Front Face (+Z)
        -0.5f, -0.5f, 0.5f, color.r * 0.5f, color.g * 0.5f, color.b * 0.5f,  // Darkest
        0.5f, -0.5f, 0.5f, color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, // Mid
        0.5f, 0.5f, 0.5f, color.r, color.g, color.b,  // Lightest
        -0.5f, 0.5f, 0.5f, color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, // Mid

        // Back Face (-Z)
        -0.5f, -0.5f, -0.5f, color.r * 0.5f, color.g * 0.5f, color.b * 0.5f,  // Darkest
        0.5f, -0.5f, -0.5f, color.r * 0.75f, color.g * 0.75f, color.b * 0.75f, // Mid
        0.5f, 0.5f, -0.5f, color.r, color.g, color.b,  // Lightest
        -0.5f, 0.5f, -0.5f, color.r * 0.75f, color.g * 0.75f, color.b * 0.75f  // Mid
    };

    // Define the indices for the cube (each set of 4 vertices makes a face)
    unsigned int faceIndices[] = {
        // Left Face
        0, 1, 2, 0, 2, 3,
        // Right Face
        4, 5, 6, 4, 6, 7,
        // Bottom Face
        8, 9, 10, 8, 10, 11,
        // Top Face
        12, 13, 14, 12, 14, 15,
        // Front Face
        16, 17, 18, 16, 18, 19,
        // Back Face
        20, 21, 22, 20, 22, 23
    };

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    // Vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(faceVertices), faceVertices, GL_STATIC_DRAW);

    // Element buffer (indices)
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndices), faceIndices, GL_STATIC_DRAW);

    // Set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // Position
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); // Color
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);
}

void Renderer::renderFace(const glm::vec3& position, const glm::vec3& color, int face) {
    if(prevColor != color) {
        setupCube(color);
        prevColor = color;
    }        
        
    glUseProgram(shaderProgram);

    // glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &color[0]);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(face * 6 * sizeof(unsigned int)));
    glBindVertexArray(0);
}



void Renderer::renderCube(const glm::vec3& position, const glm::vec3& color) {
    glUseProgram(shaderProgram);
    // printf("Rendering cube at (%f, %f, %f)\n", position.x, position.y, position.z);

    // Set the model matrix for the cube
    glm::mat4 model = glm::translate(glm::mat4(1.0f), position);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);

    // Set the color for the cube
    glUniform3fv(glGetUniformLocation(shaderProgram, "color"), 1, &color[0]);

    // Bind the VAO and draw the cube
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Renderer::cleanup() {
    // Cleanup OpenGL resources
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
}