#ifndef OBJ_LOADER_H
#define OBJ_LOADER_H

#include "mesh.h"
#include <string>

Mesh loadMeshFromObj(const std::string& filepath);

#endif
