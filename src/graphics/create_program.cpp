#include <iostream>

#include <glad/glad.h>

GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = 0) 
{
    const GLuint programId = glCreateProgram();
    if(!programId) {
        std::cerr << "Error occured in creating shader program!" << std::endl; 
        return 0; // null handle
    }

    try{
        glAttachShader(programId, vertexShader);
    } catch (const char* errorMessage) {
        std::cerr << "Error occured in attaching vertex shader: " << errorMessage << std::endl; 
    }

    try {
        glAttachShader(programId, fragmentShader);
    } catch (const char* errorMessage) {
        std::cerr << "Error occured in attaching fragment shader: " << errorMessage << std::endl; 
    }
    
    if(geometryShader) {
        try {
            glAttachShader(programId, geometryShader);
        } catch (const char* errorMessage) {
            std::cerr << "Error occured in attaching geometry shader: " << errorMessage << std::endl; 
        }
    } 

    try {
        glLinkProgram(programId);
    } catch (const char* errorMessage) {
        std::cerr << "Error in linking shader program " << programId << ": " << errorMessage << std::endl; 
        return 0;
    }

    return programId;
}