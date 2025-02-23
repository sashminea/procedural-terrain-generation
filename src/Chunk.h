#pragma once
#include <vector>
#include "Block.h"
#include "Renderer.h"
#include "../dependencies/FastNoise/FastNoise.h"

const int CHUNK_WIDTH = 12;  // Width of a chunk
const int CHUNK_HEIGHT = 8;  // Height of a chunk
const int CHUNK_DEPTH = 12;  // Depth of a chunks
const int CHUNK_SIZE = 8;

class World;

class Chunk {
public:
    // 3D array to hold blocks within a chunk
    std::vector<std::vector<std::vector<Block>>> blocks;
    World* world;
    // Constructor
    Chunk();
    Chunk(World* world); 

    // Generate the terrain within this chunk based on Perlin noise or terrain data
    void generateChunk(int startX, int startY, int startZ, FastNoise& noiseGen);

    // Render the chunk (will be implemented in the renderer)
    void renderChunk(Renderer& renderer, int startX, int startY, int startZ);
};
