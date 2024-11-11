#include <utility>

#include <glad/glad.h>
#include <graphics.h>

#include <SFML/Graphics.hpp>

const GLchar* vertexSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"void main() {\n"
" gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"}";

const GLchar* fragmentSource = 
"#version 330 core\n"
"out vec4 fragmentColor;\n"
"void main() {\n"
" fragmentColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
"}";

int main()
{
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.sRgbCapable = false;
    contextSettings.minorVersion = 3;
    contextSettings.majorVersion = 3;
    
    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, contextSettings);
    window.setActive(true);
    
    gladLoadGL();
    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

    GLuint vertexShaderId = CreateShader(vertexSource, GL_VERTEX_SHADER);
    GLuint fragmentShaderId = CreateShader(fragmentSource, GL_FRAGMENT_SHADER);


    GLuint programId = CreateProgram(vertexShaderId, fragmentShaderId);
    std::pair<GLuint, GLuint> arrayBuffers = CreateVertexBufferObject();

    GLuint vbo = arrayBuffers.first;
    GLuint vao = arrayBuffers.second;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    glUseProgram(programId);
    glBindVertexArray(vao);
    
    glDrawArrays(GL_TRIANGLES, 0, 3);
    window.display();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glDeleteProgram(programId);
    glDeleteShader(vertexShaderId);
    glDeleteShader(fragmentShaderId);

    return 0;
}