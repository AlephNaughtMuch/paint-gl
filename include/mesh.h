#ifndef MESH_H
#define MESH_H

#include "vertex.h"
#include <vector>

struct Mesh {
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    unsigned int VAO = 0;
    unsigned int VBO = 0;
    unsigned int EBO = 0;
    ~Mesh();
};

void setupMesh(Mesh *mesh);

#endif
