#include <glad/glad.h>

GLuint CreateShader(const GLchar *shaderSource, GLenum shaderType) {
    const GLuint shaderId = glCreateShader(shaderType);
    if(!shaderId) {
    return 0; // null handle
    }

    glShaderSource(shaderId, 1, &shaderSource, nullptr);
    glCompileShader(shaderId);

    // error handling

    return shaderId;
}