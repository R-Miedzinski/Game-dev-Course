#include "ShaderProgram.h"

#include <iostream>
#include <assert.h>

ShaderProgram::ShaderProgram() {
	m_programId = 0;

	i_vertexShader = 0;
	s_vertexShaderSource = "";

	i_fragmentShader = 0;
	s_fragmentShaderSource = "";

	i_geometryShader = 0;
	s_geometryShaderSource = "";
}

ShaderProgram::ShaderProgram(ShaderProgram &&rhs) noexcept
	: m_programId(std::exchange(rhs.m_programId, 0)),
	s_vertexShaderSource(std::exchange(rhs.s_vertexShaderSource, "")),
	i_vertexShader(std::exchange(rhs.i_vertexShader, 0)),
	s_fragmentShaderSource(std::exchange(rhs.s_fragmentShaderSource, "")),
	i_fragmentShader(std::exchange(rhs.i_fragmentShader, 0)),
	s_geometryShaderSource(std::exchange(rhs.s_geometryShaderSource, "")),
	i_geometryShader(std::exchange(rhs.i_geometryShader, 0))
{
}

ShaderProgram &ShaderProgram::operator=(ShaderProgram &&rhs) noexcept
{
	if (&rhs == this)
	{
		return *this;
	}

	m_programId = std::exchange(rhs.m_programId, 0);
	
	s_vertexShaderSource = std::exchange(rhs.s_vertexShaderSource, "");
	i_vertexShader = std::exchange(rhs.i_vertexShader, 0);

	s_fragmentShaderSource = std::exchange(rhs.s_fragmentShaderSource, "");
	i_fragmentShader = std::exchange(rhs.i_fragmentShader, 0);

	s_geometryShaderSource = std::exchange(rhs.s_geometryShaderSource, "");
	i_geometryShader = std::exchange(rhs.i_geometryShader, 0);

	return *this;
}

ShaderProgram::~ShaderProgram() {
	if (m_programId != 0) {
    	glDeleteProgram(m_programId);
		m_programId = 0;
	}

	if (i_vertexShader != 0) {
		glDeleteShader(i_vertexShader);
		i_vertexShader = 0;
	}

	if (i_fragmentShader != 0) {
	    glDeleteShader(i_fragmentShader);
		i_fragmentShader = 0;
	}

	if (i_geometryShader != 0) {
	    glDeleteShader(i_geometryShader);
		i_geometryShader = 0;
	}
}

void ShaderProgram::Use() {
	if (m_programId != 0) {
		glUseProgram(m_programId);
	}
}

void ShaderProgram::SetTeture(const std::string_view name, int texture) {
	if (m_programId != 0) {
		GLuint texLocation = glGetUniformLocation(m_programId, name.data());
		glUniform2d(texLocation, 1, texture);
	}
}

void ShaderProgram::SetMat4(const std::string_view name, const glm::mat4 &value) {
	if (m_programId != 0) {
		GLuint location = glGetUniformLocation(m_programId, name.data());
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(value));
	}
}

void ShaderProgram::AddVertexShader(std::string shaderSource) {
	s_vertexShaderSource = shaderSource;
	i_vertexShader = CreateShader(s_vertexShaderSource.c_str(), GL_VERTEX_SHADER);
}

void ShaderProgram::AddFragmentShader(std::string shaderSource) {
	s_fragmentShaderSource = shaderSource;
	i_fragmentShader = CreateShader(s_fragmentShaderSource.c_str(), GL_FRAGMENT_SHADER);
}

void ShaderProgram::CreateProgram() {
    m_programId = glCreateProgram();
    if (!m_programId)
    {
        std::cerr << "Error occured in creating shader program!" << std::endl;
        return; // null handle
    }

    try
    {
        glAttachShader(m_programId, i_vertexShader);
    }
    catch (const char *errorMessage)
    {
        std::cerr << "Error occured in attaching vertex shader: " << errorMessage << std::endl;
    }

    try
    {
        glAttachShader(m_programId, i_fragmentShader);
    }
    catch (const char *errorMessage)
    {
        std::cerr << "Error occured in attaching fragment shader: " << errorMessage << std::endl;
    }

    if (i_geometryShader)
    {
        try
        {
            glAttachShader(m_programId, i_geometryShader);
        }
        catch (const char *errorMessage)
        {
            std::cerr << "Error occured in attaching geometry shader: " << errorMessage << std::endl;
        }
    }

    try
    {
        glLinkProgram(m_programId);
    }
    catch (const char *errorMessage)
    {
        std::cerr << "Error in linking shader program " << m_programId << ": " << errorMessage << std::endl;
    }
}

GLuint ShaderProgram::CreateShader(const GLchar *shaderSource, GLenum shaderType)
{
    const GLuint shaderId = glCreateShader(shaderType);
    if (!shaderId)
    {
        std::cerr << "Error in creating shader: " << shaderType << std::endl
                  << shaderId << "shaderId not created" << std::endl;
        return 0; // null handle
    }

    try
    {
        glShaderSource(shaderId, 1, &shaderSource, nullptr);
    }
    catch (const char *errorMessage)
    {
        std::cerr << "Error occured in loading shader source: " << errorMessage << std::endl
                  << "For: " << shaderType << "; " << shaderId << std::endl;
    }

    try
    {
        glCompileShader(shaderId);
    }
    catch (const char *errorMessage)
    {
        std::cerr << "Error occured in compiling shader: " << errorMessage << std::endl
                  << "For: " << shaderType << "; " << shaderId << std::endl;
    }

    return shaderId;
}