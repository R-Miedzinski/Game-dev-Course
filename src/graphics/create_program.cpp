#include <glad/glad.h>

GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = 0) 
{
    const GLuint programId = glCreateProgram();
    if(!programId) {
    return 0; // null handle
    }

    glAttachShader(programId, vertexShader);
    glAttachShader(programId, fragmentShader);
    if(geometryShader) glAttachShader(programId, geometryShader);

    glLinkProgram(programId);

    return programId;
}