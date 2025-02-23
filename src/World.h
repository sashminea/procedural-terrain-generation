#ifndef WORLD_H
#define WORLD_H

#include <vector>
#include "Block.h"
#include "../dependencies/FastNoise/FastNoise.h"
#include "Chunk.h"  
#include "Camera.h"
#include <fstream>
#include <iostream>
#include <string>



const int WORLD_WIDTH = 64;
const int WORLD_HEIGHT = 16;
const int WORLD_DEPTH = 64;

class Renderer;  // Forward declaration


class World {
public:
    World();   
    Chunk* chunk;
    Camera* camera;
    void generateTerrain();  // Generate the terrain for the whole world
    void renderWorld(Renderer& renderer, Camera& camera);  // Render the world using the renderer

    bool loadWorldFromFile(const std::string& filename, Camera& camera);
    void saveWorldToFile();
    void setSeed(int newSeed);
    int getSeed();
    void generateChunks();  // Generate the chunks (new function to generate chunks)
    // void renderChunks(Renderer& renderer);  // Render all the chunks (new function for rendering chunks)
    void generateFlatWorld();
    void displayTerrain();  // Display the terrain (you can implement this or remove if unnecessary)
    void clearTerrain();
    void spawnAboveGrass(Camera &camera);

    const std::vector<std::vector<std::vector<Block>>>& getTerrain() const;  

    FastNoise noiseGen;
    std::vector<std::vector<std::vector<Block>>> terrain;  // Holds blocks for the whole world
    // std::vector<std::vector<Chunk>> chunks;  // Holds the chunks within the world

    private:
    glm::vec3 lastChunkPos;  // Track last known chunk position
    int seed;
};

#endif
