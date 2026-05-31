#ifndef SHADER
#define SHADER
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
