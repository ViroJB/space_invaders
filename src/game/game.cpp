#include "game.h"

Game::Game() {
    m_glfwManager.initialize();

    m_window = m_glfwManager.createWindow(800, 600, "Space Invaders Clone");

    m_glewManager.initialize();

    m_renderer = new Renderer();
    m_renderer->initializeVertexData();

    m_input = new Input(m_window);

    glEnable(GL_BLEND);                                 // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // enable transparency

    // note temp stuff
    m_enemies.push_back(GameObject(0.0f, 0.0f, {0.0f, 1.0f, 0.0f, 1.0f}));
    m_enemies.push_back(GameObject(-0.4f, 0.0f, {1.0f, 0.0f, 0.0f, 1.0f}));
    m_enemies.push_back(GameObject(0.4f, 0.0f, {0.0f, 0.0f, 1.0f, 1.0f}));

    m_player = new GameObject(0.0f, 0.0f, {0.5f, 0.1f, 0.2f, 1.0f});
}

void Game::run() {
    while (!glfwWindowShouldClose(m_window)) {
        // get input
        if (m_input->isKeyPressed(Input::Key::UP)) {
            fmt::print("UP key pressed\n");
            m_player->moveBy(glm::vec3(0.0f, 0.01f, 0.0f));
        }
        if (m_input->isKeyPressed(Input::Key::DOWN)) {
            fmt::print("DOWN key pressed\n");
            m_player->moveBy(glm::vec3(0.0f, -0.01f, 0.0f));
        }
        if (m_input->isKeyPressed(Input::Key::LEFT)) {
            fmt::print("LEFT key pressed\n");
            m_player->moveBy(glm::vec3(-0.01f, 0.0f, 0.0f));
        }
        if (m_input->isKeyPressed(Input::Key::RIGHT)) {
            fmt::print("RIGHT key pressed\n");
            m_player->moveBy(glm::vec3(0.01f, 0.0f, 0.0f));
        }
        if (m_input->isKeyPressed(Input::Key::ESC)) {
            fmt::print("ESC key pressed\n");

            m_renderer->printCurrentMatrix(m_player);

            // glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }

        // update gamestate

        // render
        m_renderer->render(m_window, m_enemies, m_player);

        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
}

void Game::cleanup() {
    // m_bufferManager.cleanup(m_VAO, m_VBO);
    // glDeleteProgram(m_shaderProgram);
    glfwDestroyWindow(m_window);
    glfwTerminate();
}