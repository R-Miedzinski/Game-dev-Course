#include <utility>

#include <glad/glad.h>
#include <graphics.h>

#include <SFML/Graphics.hpp>


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

    // shader setup
    const char* testPathVert = "src/graphics/shaders/test_vertex.vert";
    const char* testPathFrag = "src/graphics/shaders/test_fragment.frag";
    std::string testVertShader = ReadShaderSource(testPathVert);
    std::string testFragShader = ReadShaderSource(testPathFrag);

    float points[] = {
    // x y z
    0.75f, -0.75f, 0.0f,
    0.75f, 0.75f, 0.0f,
    -0.75f, 0.75f, 0.0f,
    -0.75f, -0.75f, 0.0f,
    };

    GLuint vertexShaderId = CreateShader(testVertShader.c_str(), GL_VERTEX_SHADER);
    GLuint fragmentShaderId = CreateShader(testFragShader.c_str(), GL_FRAGMENT_SHADER);

    GLuint programId = CreateProgram(vertexShaderId, fragmentShaderId);
    std::pair<GLuint, GLuint> arrayBuffers = CreateVertexBufferObject(points, sizeof(points));

    GLuint vbo = arrayBuffers.first;
    GLuint vao = arrayBuffers.second;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized) {
                glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));
            }
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