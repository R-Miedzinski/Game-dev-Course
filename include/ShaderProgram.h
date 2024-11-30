#pragma once
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#include <string>

class ShaderProgram
{
public:
	ShaderProgram();
	ShaderProgram(const ShaderProgram &) = delete;
	ShaderProgram &operator=(const ShaderProgram &) = delete;
	ShaderProgram(ShaderProgram &&rhs) noexcept;
	ShaderProgram &operator=(ShaderProgram &&rhs) noexcept;
	~ShaderProgram();

	void Use();

	void SetInt(const std::string_view name, int value);
	void SetTexture(const std::string_view name, int texture);
	void SetMat4(const std::string_view name, const glm::mat4 &value);

	void AddVertexShader(std::string shaderSource);
	void AddFragmentShader(std::string shaderSource);
	void CreateProgram();

private:
	GLuint m_programId;

	GLuint i_vertexShader;
	std::string s_vertexShaderSource;

	GLuint i_fragmentShader;
	std::string s_fragmentShaderSource;

	GLuint i_geometryShader;
	std::string s_geometryShaderSource;

	GLuint CreateShader(const GLchar *shaderSource, GLenum shaderType);
};