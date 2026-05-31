#include "shader.h"
#include <GL/gl.h>
#include <GL/glext.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>


// Constructor
Shader::Shader(const std::string& vertPath, const std::string& fragPath) {
    // Create debug variables to debug any
    // compilation errors
    int success = 0;
    char infoLog[1024];

    // Load vertex shader in a string
    std::ifstream file(vertPath);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string vertexShaderSource = buffer.str();

    // Load fragment shader in a string
    file = std::ifstream(fragPath);
    buffer.clear();
    buffer.str("");
    buffer << file.rdbuf();
    std::string fragmentShaderSource = buffer.str();

    // Create the shaders
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    // Compile vertex shader
    const char* src = vertexShaderSource.c_str();
    glShaderSource(
        vertexShader,
        1,
        &src,
        NULL
    );
    glCompileShader(vertexShader);
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::VERTEX::COMPILATION_FAILED\n" <<
            infoLog << std::endl;
    }

    // Compile fragment shader
    src = fragmentShaderSource.c_str();
    glShaderSource(
        fragmentShader,
        1,
        &src,
        NULL
    );
    glCompileShader(fragmentShader);
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER::FRAGMENT::COMPILATION_FAILED\n" <<
            infoLog << std::endl;
    }

    // Link shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "ERROR:SHADER_LINK::SHADER_PROGRAM::LINKING_FAILED\n" <<
            infoLog << std::endl;
    }

    // Delete the shaders now that they are linked
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

// Destructor
Shader::~Shader() {
    glDeleteProgram(ID);
};

// Methods
void Shader::use() {
    glUseProgram(ID);
}
