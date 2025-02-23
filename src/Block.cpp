#include "Block.h"

glm::vec3 Block::getBlockColor() const {
    switch (type) {
        case GRASS:
            return glm::vec3(0.2f, 1.0f, 0.2f); // Green color for grass
        case STONE:
            return glm::vec3(0.663f, 0.639f, 0.639f); // Stone color
        case DIRT:
            return glm::vec3(0.6f, 0.3f, 0.1f); // Dirt color
        case WATER:
            return glm::vec3(0.0f, 0.0f, 1.0f); // Water color (blue)
        default:
            return glm::vec3(1.0f, 1.0f, 1.0f); // Default color for air or unknown block
    }
}

const char* Block::getTypeString() const {
    switch (type) {
        case AIR: return "Air";
        case GRASS: return "Grass";
        case STONE: return "Stone";
        case DIRT: return "Dirt";
        case WATER: return "Water";
        default: return "Unknown";
    }
}
