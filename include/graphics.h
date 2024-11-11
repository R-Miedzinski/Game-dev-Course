#include <utility>
#include <glad/glad.h>

#ifndef __graphics__
#define __graphics__
#endif

std::string ReadShaderSource(const char* source);
GLuint CreateShader(const GLchar *shaderSource, GLenum shaderType);
GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = 0);
std::pair<GLuint, GLuint> CreateVertexBufferObject(float* points, int size);