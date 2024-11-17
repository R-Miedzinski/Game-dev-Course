#include <utility>
#include <iostream>

#include <glad/glad.h>
#include <graphics.h>
#include <ShaderProgram.h>
#include <Cube.h>
#include <Camera.h>

#include <SFML/Graphics.hpp>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

int main()
{
    // timestep setup
    sf::Clock timer = sf::Clock();

    double t = 0.0;
    const double dt = 1.0 / 60.0;
    float velocity = dt * 0.2f;

    double currentTime = timer.restart().asSeconds();
    double accumulator = 0.0;

    // SFML window setup
    sf::ContextSettings contextSettings;
    contextSettings.depthBits = 24;
    contextSettings.sRgbCapable = false;
    contextSettings.minorVersion = 3;
    contextSettings.majorVersion = 3;

    sf::Vector2i mousePosition = sf::Mouse::getPosition();

    sf::Window window(sf::VideoMode(800, 600), "OpenGL", sf::Style::Default, contextSettings);
    window.setActive(true);

    gladLoadGL();
    glViewport(0, 0, static_cast<GLsizei>(window.getSize().x), static_cast<GLsizei>(window.getSize().y));

    // Camera setup
    const glm::vec3 initialPosition = glm::vec3(2.0f, 2.0f, 2.0f);
    const glm::vec3 initialFront = glm::vec3(-0.5f, -0.5f, -0.5f);

    Camera camera(initialPosition, initialFront, 0.0f, 0.0f, window.getSize());

    // shader setup
    std::string testVertShader = ReadShaderSource("src/graphics/shaders/test_vertex.vert");

    std::string testFragShader = ReadShaderSource("src/graphics/shaders/test_fragment.frag");

    ShaderProgram cubeShader;

    cubeShader.AddVertexShader(testVertShader);
    cubeShader.AddFragmentShader(testFragShader);
    cubeShader.CreateProgram();

    Cube cube("src/graphics/textures/grass_debug.jpg");

    glEnable(GL_DEPTH_TEST);

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
                sf::Vector2 windowSize = window.getSize();
                glViewport(0, 0, static_cast<GLsizei>(windowSize.x), static_cast<GLsizei>(windowSize.y));
                camera.ResizeWindow(windowSize);
            }
        }

        // Camera rotation
        const sf::Vector2i newMousePosition = sf::Mouse::getPosition();
        camera.Rotate(newMousePosition - mousePosition);
        mousePosition = newMousePosition;

        //  Camera Move
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
            camera.MoveForward(velocity);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            camera.MoveBackward(velocity);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
            camera.MoveLeft(velocity);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
            camera.MoveRight(velocity);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
            camera.MoveUp(velocity);
        } 
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) {
            camera.MoveDown(velocity);
        }

        while (accumulator >= dt)
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            cubeShader.Use();

            glm::mat4 model = glm::mat4(1.0f);
            glm::mat4 view = camera.View();
            glm::mat4 projection = camera.Projection();

            glm::mat4 mvp = projection * view * model;

            cubeShader.SetMat4("mvp", mvp);

            cubeShader.SetTeture("texture1", cube.Texture());

            glBindVertexArray(cube.Vao());
            glDrawArrays(GL_TRIANGLES, 0, cube.vertices());

            accumulator -= dt;
            t += dt;
        }

        // render loop finish, display state
        window.display();
    }

    return 0;
}