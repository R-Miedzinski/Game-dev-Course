#include <utility>
#include <glad/glad.h>

std::pair<GLuint, GLuint> CreateVertexBufferObject() {
 const float triangle[] = {
 // x y z
 -0.5f, -0.5f, 0.0f,
 0.5f, -0.5f, 0.0f,
 0.0f, 0.5f, 0.0f
 };
 
 GLuint vbo, vao;
 glGenBuffers(1, &vbo);
 glGenVertexArrays(1, &vao);
 glBindVertexArray(vao);
 glBindBuffer(GL_ARRAY_BUFFER, vbo);
 glBufferData(GL_ARRAY_BUFFER, sizeof(triangle), triangle, GL_STATIC_DRAW);
 
 glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
 glEnableVertexAttribArray(0);
 
 glBindBuffer(GL_ARRAY_BUFFER, 0);
 glBindVertexArray(0);
 
 return std::make_pair(vbo, vao);
}