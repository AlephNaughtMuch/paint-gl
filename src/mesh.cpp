#include "mesh.h"
#include "vertex.h"
#include <cstddef>
#include <glad/glad.h>
#include <glm/ext/vector_float3.hpp>


void setupMesh(Mesh *mesh) {
    // Define Vertex Buffers and Objects
    glGenVertexArrays(1, &mesh->VAO);
    glGenBuffers(1, &mesh->VBO);
    glGenBuffers(1, &mesh->EBO);

    // Bind VAO first
    glBindVertexArray(mesh->VAO);

    // Bind the VBO second and send data
    glBindBuffer(GL_ARRAY_BUFFER, mesh->VBO);
    glBufferData(
        GL_ARRAY_BUFFER,
        mesh->vertices.size() * sizeof(Vertex),
        mesh->vertices.data(),
        GL_STATIC_DRAW
    );

    // Bind the EBO and send data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->EBO);
    glBufferData(
        GL_ELEMENT_ARRAY_BUFFER,
        mesh->indices.size() * sizeof(unsigned int),
        mesh->indices.data(),
        GL_STATIC_DRAW
    );

    // Init Vertex Attribute Pointers for each
    // Vertex attribute.

    // Position
    glVertexAttribPointer(
        0,
        glm::vec3::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*) offsetof(Vertex, position)
    );
    glEnableVertexAttribArray(0);

    // Normal
    glVertexAttribPointer(
        1,
        glm::vec3::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*) offsetof(Vertex, normal)
    );
    glEnableVertexAttribArray(1);

    // UV
    glVertexAttribPointer(
        2,
        glm::vec2::length(),
        GL_FLOAT,
        GL_FALSE,
        sizeof(Vertex),
        (void*) offsetof(Vertex, uv)
    );
    glEnableVertexAttribArray(2);


    // Unbind buffers
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


}

Mesh::~Mesh() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}
