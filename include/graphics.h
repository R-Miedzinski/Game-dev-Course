#include <utility>
#include <string>

#include <glad/glad.h>
#include <SFML/Graphics.hpp>

#ifndef __graphics__
#define __graphics__
#endif

std::string ReadShaderSource(const char *source);
GLuint CreateTexture(const std::string& path);

GLuint CreateShader(const GLchar *shaderSource, GLenum shaderType);
GLuint CreateProgram(GLuint vertexShader, GLuint fragmentShader, GLuint geometryShader = 0);
std::pair<GLuint, GLuint> CreateVertexBufferObject(float *points, int size);