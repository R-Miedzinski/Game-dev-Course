#include <iostream>

#include <glad/glad.h>

GLuint CreateShader(const GLchar *shaderSource, GLenum shaderType) {
    const GLuint shaderId = glCreateShader(shaderType);
    if(!shaderId) {
        std::cerr << "Error in creating shader: " << shaderType << std::endl << shaderId << "shaderId not created" << std::endl;
        return 0; // null handle
    }

    try {
        glShaderSource(shaderId, 1, &shaderSource, nullptr);
    } catch(const char* errorMessage) {
        std::cerr << "Error occured in loading shader source: " << errorMessage << std::endl << "For: " << shaderType << "; " << shaderId << std::endl; 
    }

    try {
        glCompileShader(shaderId);
    } catch(const char* errorMessage) {
        std::cerr << "Error occured in compiling shader: " << errorMessage << std::endl << "For: " << shaderType << "; " << shaderId << std::endl; 
    }

    return shaderId;
}