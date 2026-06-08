#ifndef SHADER_H
#define SHADER_H
#include <glad/glad.h>

#include <string>
class Shader {
    public:
        // Constructor
        Shader(const std::string& vertPath, const std::string& fragPath);
        ~Shader(); // destructor
        void use();
        unsigned int ID;
};

#endif
