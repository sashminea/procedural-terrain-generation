#include <glm/glm.hpp>
#include "Chunk.h"
#include "../dependencies/FastNoise/FastNoise.h"
#include "World.h"
#include "Renderer.h"

// Constructor: Initialize the chunk with AIR blocks
Chunk::Chunk() {
    blocks.resize(CHUNK_WIDTH, std::vector<std::vector<Block>>(
        CHUNK_HEIGHT, std::vector<Block>(CHUNK_DEPTH, Block(AIR))));
}

Chunk::Chunk(World* world) : world(world) {
    blocks.resize(CHUNK_WIDTH, std::vector<std::vector<Block>>(
        CHUNK_HEIGHT, std::vector<Block>(CHUNK_DEPTH, Block(AIR))));
}

// Generate terrain for the chunk
// void Chunk::generateChunk(int startX, int startY, int startZ, FastNoise& noiseGen) {
//     for (int x = 0; x < CHUNK_WIDTH; x++) {
//         for (int z = 0; z < CHUNK_DEPTH; z++) {
//             int worldX = startX + x;  // Convert chunk coordinates to world coordinates
//             int worldZ = startZ + z;

//             for (int y = 0; y < CHUNK_HEIGHT; y++) {
//                 int worldY = startY + y;
                
//                 // Ensure we are within world bounds
//                 if (worldX >= 0 && worldX < WORLD_WIDTH &&
//                     worldY >= 0 && worldY < WORLD_HEIGHT &&
//                     worldZ >= 0 && worldZ < WORLD_DEPTH) {

//                     // Assign block from world terrain
//                     blocks[x][y][z] = world->terrain[worldX][worldY][worldZ];
//                 } else {
//                     blocks[x][y][z] = Block(AIR);  // Default to AIR if out of bounds
//                 }
//             }
//         }
//     }
// }


// // Render the chunk
// void Chunk::renderChunk(Renderer& renderer, int chunkX, int chunkY, int chunkZ) {
//     for (int x = 0; x < CHUNK_WIDTH; x++) {
//         for (int z = 0; z < CHUNK_DEPTH; z++) {
//             for (int y = 0; y < CHUNK_HEIGHT; y++) {
//                 if (blocks[x][y][z].type != AIR) {
//                     glm::vec3 color;
//                     if (blocks[x][y][z].type == STONE) {
//                         color = glm::vec3(0.5f, 0.5f, 0.5f);  // Stone color
//                     } else if (blocks[x][y][z].type == GRASS) {
//                         color = glm::vec3(0.0f, 1.0f, 0.0f);  // Grass color
//                     }

//                     // Render at world position (adjusted by chunk's position)
//                     glm::vec3 worldPos = glm::vec3(chunkX + x, chunkY + y, chunkZ + z);
//                     renderer.renderCube(worldPos, color);
//                 }
//             }
//         }
//     }
// }




// // Generate terrain for the chunk
// void Chunk::generateChunk(int startX, int startY, int startZ, FastNoise& noiseGen) {
//     for (int x = 0; x < CHUNK_WIDTH; x++) {
//         for (int z = 0; z < CHUNK_DEPTH; z++) {
//             // Use Perlin noise to determine the height for the chunk
//             float height = noiseGen.GetNoise(startX + x, startZ + z) * 5.0f + startY;

//             for (int y = 0; y < CHUNK_HEIGHT; y++) {
//                 if (y < height) {
//                     blocks[x][y][z] = Block(STONE);
//                 } else if (y == height) {
//                     blocks[x][y][z] = Block(GRASS);
//                 } else {
//                     blocks[x][y][z] = Block(AIR);
//                 }
//             }
//         }
//     }
// }

