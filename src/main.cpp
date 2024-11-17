#include <utility>
#include <iostream>

#include <glad/glad.h>
#include <graphics.h>
#include <ShaderProgram.h>

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

int main()
{
    // timestep setup
    sf::Clock timer = sf::Clock();

    double t = 0.0;
    const double dt = 1.0 / 60.0;

    double currentTime = timer.restart().asSeconds();
    double accumulator = 0.0;

    // SFML window setup
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
    std::string testVertShader = ReadShaderSource("src/graphics/shaders/test_vertex.vert");

    std::string testFragShader = ReadShaderSource("src/graphics/shaders/test_fragment.frag");

    GLuint debugTexture = CreateTexture("src/graphics/textures/grass_debug.jpg");

    float points[] = {
        // x y z
        -0.75f, 0.75f, 0.0f, 0.25f, 2.0f/3.0f,
        0.75f, 0.75f, 0.0f, 0.5f, 2.0f/3.0f,
        0.75f, -0.75f, 0.0f, 0.5f, 1.0f/3.0f,
        -0.75f, -0.75f, 0.0f, 0.25f, 1.0f/3.0f,
    };

    ShaderProgram cubeShader;

    cubeShader.AddVertexShader(testVertShader);
    cubeShader.AddFragmentShader(testFragShader);
    cubeShader.CreateProgram();

    std::pair<GLuint, GLuint> arrayBuffers = CreateVertexBufferObject(points, sizeof(points));

    GLuint vbo = arrayBuffers.first;
    GLuint vao = arrayBuffers.second;

    // application open
    while (window.isOpen())
    {
        double newTime = timer.getElapsedTime().asSeconds();
        double frameTime = newTime - currentTime;

        currentTime = newTime;

        accumulator += frameTime;

        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::Resized)
            {
                glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));
            }
        }

        while (accumulator >= dt)
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            cubeShader.Use();

            glm::mat4 model = glm::mat4(1.0f);
            model = glm::rotate(model, glm::radians(180.0f * static_cast<float>(sin(t))), glm::vec3(1.0f, 0.0f, 0.0f)) * glm::rotate(model, glm::radians(180.0f * static_cast<float>(cos(t))), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 view = glm::mat4(1.0f);
            view = glm::lookAt(glm::vec3(0.0f, 0.0f, -3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

            glm::mat4 projection = glm::mat4(1.0f);
            projection = glm::perspective(glm::radians(45.0f), static_cast<float>(window.getSize().x)/static_cast<float>(window.getSize().y), 0.1f, 100.0f);

            glm::mat4 mvp = projection * view * model;


            cubeShader.SetMat4("mvp", model);

            cubeShader.SetTeture("texture1", debugTexture);

            glBindVertexArray(vao);
            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

            accumulator -= dt;
            t += dt;
        }

        // render loop finish, display state
        window.display();
    }

    // application cleanup
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);

    return 0;
}