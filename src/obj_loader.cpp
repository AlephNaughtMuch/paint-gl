#include "obj_loader.h"
#include "mesh.h"
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <iostream>
#include <string>
#include <vector>

Mesh loadMeshFromObj(const std::string &filepath) {

    // Init Mesh
    Mesh loadedMesh;

    // Open the file
    std::ifstream file(filepath);

    // Check if the file was opened successfully
    if (!file.is_open()) {
        std::cerr << "Error: Could not open the given obj file!" << std::endl;
        return loadedMesh;
    }

    // Init the vectors to temporarily populate to create a mesh
    // after the obj reading is done.
    std::string line;
    std::vector<glm::vec3> vertices;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> uvs;

    // Store the face lines just as strings for now
    // to safeguard against rogue face lines in the obj
    std::vector<std::string> faceLines;

    // Read the obj line-by-line
    while (std::getline(file, line)) {
        // Convert to a istringstream
        std::istringstream iss(line);

        // Grab the identifier token to find what line
        // we are on
        std::string identifier;
        iss >> identifier;

        if (identifier == "v") {
            float x;
            float y;
            float z;
            iss >> x >> y >> z;
            vertices.push_back(glm::vec3(x, y, z));
        } else if (identifier == "vt") {
            float u;
            float v;
            iss >> u >> v;
            uvs.push_back(glm::vec2(u,v));
        } else if (identifier == "vn") {
            float x;
            float y;
            float z;
            iss >> x >> y >> z;
            normals.push_back(glm::vec3(x, y, z));
        } else if (identifier == "f") {
            std::string faceLine;
            std::getline(iss, faceLine);
            faceLines.push_back(faceLine);
        }
    }

    // Go over the face lines
    for (const auto& str: faceLines) {
        std::istringstream ss(str);
        std::string token;
        std::vector<Vertex> faceVertices;

        // Token is something like "1/2/3" or "1//3"
        while (ss >> token) {
            std::istringstream tt(token);

            // Grab the indices from the face line
            std::string posIndex;
            std::string uvIndex;
            std::string normalIndex;

            std::getline(tt, posIndex, '/');
            std::getline(tt, uvIndex, '/');
            std::getline(tt, normalIndex, '/');

            // Look up position, uv and normal from the temp
            // vectors we populated above and create a new
            // Vertex object.
            Vertex currentVertex;
            if (!posIndex.empty()) {
                currentVertex.position = vertices[std::stoi(posIndex) - 1];
            }
            if (!normalIndex.empty()) {
                currentVertex.normal   = normals[std::stoi(normalIndex) - 1];
            }
            if (!uvIndex.empty()) {
                currentVertex.uv       = uvs[std::stoi(uvIndex) - 1];
            }

            // Push the current vertex in the faceVertices vector
            // in case we need to triangulate later.
            faceVertices.push_back(currentVertex);
        }

        // Get number of faces
        unsigned int faceNum = faceVertices.size();

        // Triangulate
        for (unsigned int i = 1; i < faceNum - 1; ++i) {
            loadedMesh.vertices.push_back(faceVertices[0]);
            loadedMesh.indices.push_back(loadedMesh.vertices.size() - 1);
            loadedMesh.vertices.push_back(faceVertices[i]);
            loadedMesh.indices.push_back(loadedMesh.vertices.size() - 1);
            loadedMesh.vertices.push_back(faceVertices[i + 1]);
            loadedMesh.indices.push_back(loadedMesh.vertices.size() - 1);
        }
    }
    return loadedMesh;
}
