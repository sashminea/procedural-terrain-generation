#ifndef BLOCK_H
#define BLOCK_H
#include <string>
#include <glm/glm.hpp>

// Define block types
enum BlockType {
    AIR,
    GRASS,
    STONE,
    DIRT,
    WATER
};

class Block {
public:
    BlockType type;

    // Constructor
    Block(BlockType type = AIR) : type(type) {}
    glm::vec3 getBlockColor() const;


    // Return a string representation of the block
    const char* getTypeString() const;
    const int getTypeInt() const { return type; };
};

#endif
