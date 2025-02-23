#ifndef TERRAIN_H
#define TERRAIN_H

#include <glm/glm.hpp>
#include <vector>
#include <iostream>

class Terrain {
public:
    Terrain(int width, int depth, float scale);
    void Generate();
    std::vector<glm::vec3> GetVertices() const;

private:
    int m_width, m_depth;
    float m_scale;
    std::vector<glm::vec3> m_vertices;

    float GetHeight(float x, float z);
};

#endif
