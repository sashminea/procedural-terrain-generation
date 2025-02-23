#include "World.h"
#include <iostream>
#include "Renderer.h"
#include "Chunk.h"
#include "Windows.h"
#include <sys/stat.h> // POSIX/Windows mkdir
#include <filesystem>
#ifdef _WIN32
    #include <direct.h>  // Windows-specific mkdir
#else
    #include <sys/stat.h>  // Linux/macOS mkdir
    #include <sys/types.h>
#endif
namespace fs = std::filesystem;

void createWorldsDirectory() {
    #ifdef _WIN32
        _mkdir("../worlds");  // Windows
    #else
        mkdir("../worlds", 0777); // Linux/macOS
    #endif
}

bool directoryExists(const std::string& path) {
    struct stat info;
    bool exists = (stat(path.c_str(), &info) == 0 && (info.st_mode & S_IFDIR));
    std::cout << "Checking directory: " << path << " -> " << (exists ? "Exists" : "Doesn't Exist") << std::endl;
    return exists;
}


// Constructor: Initialize terrain vector
World::World() {

    seed = 0;  // Default seed for flat world

    noiseGen.SetNoiseType(FastNoise::Perlin);
    noiseGen.SetFrequency(0.1f);
    noiseGen.SetSeed(seed);

    // Initialize terrain size
    terrain.resize(WORLD_WIDTH, std::vector<std::vector<Block>>(
        WORLD_HEIGHT, std::vector<Block>(WORLD_DEPTH, Block(AIR))));

    // Initialize chunk array
    // chunks.resize(WORLD_WIDTH / WORLD_CHUNK_WIDTH, std::vector<Chunk>(WORLD_DEPTH / WORLD_CHUNK_DEPTH));

    chunk = new Chunk(this);
    
    lastChunkPos = glm::vec3(-999, -999, -999);  // Invalid starting value
    
    generateTerrain();
    // loadWorldFromFile("../worlds/0.world");
}

int World::getSeed() {
    return seed;
}

void World::setSeed(int newSeed) {
    seed = newSeed;
    noiseGen.SetSeed(seed);
    generateTerrain();
}

void World::generateFlatWorld() {

  for (int x = 0; x < WORLD_WIDTH; x++) {
        for (int z = 0; z < WORLD_DEPTH; z++) {
            for (int y = 0; y < WORLD_HEIGHT; y++) {
                terrain[x][y][z] = (y == WORLD_HEIGHT - 1) ? Block(GRASS) : Block(DIRT);
                printf("Flat - Block at (%d, %d, %d): %s\n", x, y, z, terrain[x][y][z].getTypeString());
            }
        }
    }


    std::cout << "Flat world generated at height " << WORLD_HEIGHT << std::endl;
}


bool World::loadWorldFromFile(const std::string& filename, Camera &camera) {

    clearTerrain();

    std::string fullPath = "../worlds/" + filename;  // Load from "../worlds/"
    
    std::ifstream file(fullPath);
    if (!file) {
        std::cout << "Failed to load world: " << fullPath << std::endl;
        generateFlatWorld();
        return false;
    }

    int width, height, depth;
    file >> width >> height >> depth;

    // Make sure the dimensions match the expected ones
    if (width != WORLD_WIDTH || height != WORLD_HEIGHT || depth != WORLD_DEPTH) {
        std::cerr << "World dimensions mismatch!" << std::endl;
        generateFlatWorld();
        return false;
    }


    // Read the block types into the terrain
    for (int x = 0; x < width; ++x) {
        for (int y = 0; y < height; ++y) {
            for (int z = 0; z < depth; ++z) {
                int blockType;
                file >> blockType;
                terrain[x][y][z] = Block(static_cast<BlockType>(blockType)); // Assuming BlockType is an enum
                printf("Loaded Block at (%d, %d, %d): %s\n", x, y, z, terrain[x][y][z].getTypeString());
            }
        }
    }

    std::cout << "World loaded from file." << std::endl;
    spawnAboveGrass(camera);

    return true;
}

void World::saveWorldToFile() {
    // Ensure "worlds/" directory exists
    if (!directoryExists("../worlds")) {
        createWorldsDirectory();
    }

    std::string filename = std::to_string(seed) + ".world";
    std::string fullPath = "../worlds/" + filename;

    // No need to check for existing file; just overwrite it
    std::ofstream file(fullPath, std::ios::trunc);  // Use trunc to overwrite the file
    if (!file) {
        std::cout << "Failed to save world: " << fullPath << std::endl;
        return;
    }

    // Example: Save terrain dimensions
    file << WORLD_WIDTH << " " << WORLD_HEIGHT << " " << WORLD_DEPTH << std::endl;

    // Iterate over terrain and save each block type (simplified)
    for (int x = 0; x < WORLD_WIDTH; ++x) {
        for (int y = 0; y < WORLD_HEIGHT; ++y) {
            for (int z = 0; z < WORLD_DEPTH; ++z) {
                // Save the block type (e.g., BlockType might be an enum or int)
                file << terrain[x][y][z].getTypeInt() << " ";
            }
            file << std::endl; // Separate each layer of blocks
        }
    }

    std::cout << "World saved to file." << std::endl;
    file.close();
}


void World::clearTerrain() {
    // Reset the terrain to an empty 3D vector
    terrain.clear();  // Clear the existing data in the vector
    terrain.shrink_to_fit();  // Free the unused memory

    // Recreate the terrain with new dimensions
    terrain = std::vector<std::vector<std::vector<Block>>>(WORLD_WIDTH, 
           std::vector<std::vector<Block>>(WORLD_HEIGHT, 
           std::vector<Block>(WORLD_DEPTH, Block()))); // Default Block constructor is called

    std::cout << "Terrain cleared and reinitialized!" << std::endl;
}


void World::generateTerrain() {

    clearTerrain();

    for (int x = 0; x < WORLD_WIDTH; x++) {
        for (int z = 0; z < WORLD_DEPTH; z++) {
            int height = static_cast<int>(std::floor(noiseGen.GetNoise(x, z) * 5.0f + WORLD_HEIGHT / 4));

            for (int y = 0; y <= height; y++) {
                terrain[x][y][z] = (y == height) ? Block(GRASS) : Block(DIRT);
                printf("Block at (%d, %d, %d): %s\n", x, y, z, terrain[x][y][z].getTypeString());
            }
        }
    }
}


void World::spawnAboveGrass(Camera &camera) {
    // Calculate the middle coordinates
    int midX = WORLD_WIDTH / 2;
    int midZ = WORLD_DEPTH / 2;

    // Start searching from the middle and move downwards until we find the first grass block
    for (int y = WORLD_HEIGHT - 1; y >= 0; --y) {
        Block currentBlock = terrain[midX][y][midZ]; // Check the block at the middle position in Y

        if (currentBlock.getTypeInt() == BlockType::GRASS) {
            // Set camera position just above the grass block (4 units higher)
            camera.position = glm::vec3(midX, y + 4.0f, midZ); // Adjust y position to be above the grass block
            std::cout << "Spawned above grass block at (" << midX << ", " << y << ", " << midZ << ")\n";
            return; // Exit the function after finding a valid spawn point
        }
    }

    // If no grass block was found
    std::cout << "No grass block found for spawn!" << std::endl;
}

















void World::renderWorld(Renderer& renderer, Camera& camera) {
    glm::vec3 cameraChunk = camera.GetChunkCoordinates();

    for (int x = 0; x < WORLD_WIDTH; x++) {
        for (int z = 0; z < WORLD_DEPTH; z++) {
            for (int y = 0; y < WORLD_HEIGHT; y++) {
                if (terrain[x][y][z].type != AIR) {

                    // Check if the block is within the chunk's render distance
                    if ((fabs(x - cameraChunk.x) <= CHUNK_DEPTH) &&
                        (fabs(y - cameraChunk.y) <= CHUNK_HEIGHT) &&
                        (fabs(z - cameraChunk.z) <= CHUNK_DEPTH)) {

                        glm::vec3 color = terrain[x][y][z].getBlockColor();


                        // **Only render a face if there's air next to it**
                        if (x == 0 || terrain[x - 1][y][z].type == AIR) // Left (-X)
                            renderer.renderFace(glm::vec3(x, y, z), color, Renderer::LEFT_FACE);

                        if (x == WORLD_WIDTH - 1 || terrain[x + 1][y][z].type == AIR) // Right (+X)
                            renderer.renderFace(glm::vec3(x, y, z), color, Renderer::RIGHT_FACE);

                        if (y == 0 || terrain[x][y - 1][z].type == AIR) // Bottom (-Y)
                            renderer.renderFace(glm::vec3(x, y, z), color, Renderer::BOTTOM_FACE);

                        if (y == WORLD_HEIGHT - 1 || terrain[x][y + 1][z].type == AIR) // Top (+Y)
                            renderer.renderFace(glm::vec3(x, y, z), color, Renderer::TOP_FACE);

                        if (z == 0 || terrain[x][y][z - 1].type == AIR) // Front (-Z)
                            renderer.renderFace(glm::vec3(x, y, z), color, Renderer::FRONT_FACE);

                        if (z == WORLD_DEPTH - 1 || terrain[x][y][z + 1].type == AIR) // Back (+Z)
                            renderer.renderFace(glm::vec3(x, y, z), color, Renderer::BACK_FACE);
                    }
                }
            }
        }
    }
}



// // Display terrain for debugging
void World::displayTerrain() {
    for (int x = 0; x < WORLD_WIDTH; x++) {
        for (int z = 0; z < WORLD_DEPTH; z++) {
            int height = 0;
            for (int y = 0; y < WORLD_HEIGHT; y++) {
                if (terrain[x][y][z].type != AIR) {
                    height = y;
                    break;
                }
            }
            std::cout << "Block at (" << x << ", " << height << ", " << z << "): "
                      << terrain[x][height][z].getTypeString() << std::endl;
        }
    }
}
