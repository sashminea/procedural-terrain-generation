#include <iostream>
#include <glad/glad.h>
#include "../dependencies/glfw/include/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "World.h"
#include "Renderer.h"
#include "Chunk.h"


bool wireframeMode = false;

// Camera instance
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);

World world;
Chunk chunk;

// Window size
const int WIDTH = 1280;
const int HEIGHT = 720;

// Callback function for handling window resizing
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

float lastX = 400, lastY = 300;
bool firstMouse = true;


// Mouse input callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (firstMouse) {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xOffset = xpos - lastX;
    float yOffset = lastY - ypos;  // Reversed since y-coordinates go from bottom to top
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xOffset, yOffset);
}

void processInput(GLFWwindow *window, float deltaTime) {
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard('W', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard('S', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard('A', deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard('D', deltaTime);

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) {
        wireframeMode = !wireframeMode;
        glPolygonMode(GL_FRONT_AND_BACK, wireframeMode ? GL_LINE : GL_FILL);
    }

    // Handle key presses for world generation, saving, loading, and exiting
    if (glfwGetKey(window, GLFW_KEY_0) == GLFW_PRESS) {
        // Generate a new world with a random seed
        world.setSeed(rand() % 10000);  // Generate random seed between 0 and 9999
        std::cout << "Generated new world with seed: " << world.getSeed() << std::endl;
        world.generateTerrain();
    }

    if (glfwGetKey(window, GLFW_KEY_5) == GLFW_PRESS) {
        // Save the current world
        world.saveWorldToFile();
    }

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        // Load or generate the flat world
        if (!world.loadWorldFromFile("../worlds/0.world", camera)) {
            std::cout << "Flat world not found, generating one..." << std::endl;
            world.generateFlatWorld();
            world.saveWorldToFile(); // Save the generated flat world
        }
    }


    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        // Terminate the application
        glfwSetWindowShouldClose(window, true);
        std::cout << "Terminating application..." << std::endl;
    }
}


int main() {
    std::cout << "Starting application..." << std::endl;

    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW!" << std::endl;
        return -1;
    }
    std::cout << "GLFW initialized successfully." << std::endl;

    // Set OpenGL version (4.6 Core Profile)
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a window
    std::cout << "Creating GLFW window..." << std::endl;
    GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Minecraft Clone", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return -1;
    }

    std::cout << "GLFW window created successfully." << std::endl;

    glfwMakeContextCurrent(window);

    // Load OpenGL functions using GLAD
    std::cout << "Initializing GLAD..." << std::endl;
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD!" << std::endl;
        return -1;
    }
    std::cout << "GLAD initialized successfully." << std::endl;

    // Set viewport and register window resize callback
    glViewport(0, 0, WIDTH, HEIGHT);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);

    // Disable mouse cursor (to lock it in the center)
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Create the world and chunk manager
    std::cout << "Creating world and chunks..." << std::endl;
    


    // Create the Renderer instance
    std::cout << "Creating renderer..." << std::endl;
    Renderer renderer;
    renderer.init();  // Ensure the renderer is initialized

    // Main render loop
    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    world.spawnAboveGrass(camera);

    std::cout << "Entering main render loop..." << std::endl;
    while (!glfwWindowShouldClose(window)) {
        // Calculate delta time
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // Process input
        processInput(window, deltaTime);

        // Clear screen with black color
        glClearColor(0.53f, 0.81f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);  // Also clear depth buffer

        // Get view and projection matrices
        glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 projection = glm::perspective(glm::radians(camera.zoom), (float)WIDTH / (float)HEIGHT, 0.1f, 100.0f);

        // Pass the matrices to the shader program
        glUseProgram(renderer.shaderProgram);
        glUniformMatrix4fv(glGetUniformLocation(renderer.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(renderer.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

        

        // Generate chunks around the player's position and render
        world.renderWorld(renderer, camera);  // Render the visible chunks

        // Swap buffers and poll for events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    renderer.cleanup();  // Ensure cleanup is called
    glfwDestroyWindow(window);
    glfwTerminate();
    std::cout << "Application terminated successfully." << std::endl;
    return 0;
}
