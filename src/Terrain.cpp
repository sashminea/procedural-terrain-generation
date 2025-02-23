#include "Terrain.h"
#include <cmath>  // for sin function

Terrain::Terrain(int width, int depth, float scale)
    : m_width(width), m_depth(depth), m_scale(scale) {
    Generate();
}

void Terrain::Generate() {
    m_vertices.clear();
    
    for (int z = 0; z < m_depth; ++z) {
        for (int x = 0; x < m_width; ++x) {
            float height = GetHeight(x, z);
            m_vertices.push_back(glm::vec3(x * m_scale, height, z * m_scale));
        }
    }
}

float Terrain::GetHeight(float x, float z) {
    // A simple sine wave function for height, can be replaced with Perlin noise for better results
    return std::sin(x * 0.1f) * std::sin(z * 0.1f) * 2.0f;
}

std::vector<glm::vec3> Terrain::GetVertices() const {
    return m_vertices;
}
