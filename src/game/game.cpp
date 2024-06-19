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

    // create random number between -1.0 and 1.0
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    // note temp stuff
    m_enemies.push_back(GameObject(dis(gen), 0.0f, {0.0f, 1.0f, 0.0f, 1.0f}));
    m_enemies.push_back(GameObject(dis(gen), 0.0f, {1.0f, 0.0f, 0.0f, 1.0f}));
    m_enemies.push_back(GameObject(dis(gen), 0.0f, {0.0f, 0.0f, 1.0f, 1.0f}));

    m_player = new GameObject(0.0f, 0.0f, {0.5f, 0.1f, 0.2f, 1.0f});
    m_player->setSpeed(10.0f);
}

void Game::updateEnemies(float deltaTime) {
    // create random number between -1.0 and 1.0
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disPosition(-1.0f, 1.0f);
    std::uniform_real_distribution<float> disColor(0.0f, 1.0f);

    if (m_enemies.size() <= 9) {
        m_enemies.push_back(GameObject(disPosition(gen), 0.0f, {disColor(gen), disColor(gen), disColor(gen), 1.0f}));
    }

    for (auto& enemy : m_enemies) {
        enemy.moveDown(deltaTime);
    }
}

void Game::run() {
    std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(m_window)) {
        // delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastFrameTime;
        float deltaTime = elapsed.count();
        fmt::print("delta time: {}\n", deltaTime);
        fmt::print("FPS: {}\n", 1.0f / deltaTime);

        // process input
        if (m_input->isKeyPressed(Input::Key::UP) || m_input->isKeyPressed(Input::Key::W)) {
            fmt::print("UP key pressed\n");
            m_player->moveUp(deltaTime);
        }
        if (m_input->isKeyPressed(Input::Key::DOWN) || m_input->isKeyPressed(Input::Key::S)) {
            fmt::print("DOWN key pressed\n");
            m_player->moveDown(deltaTime);
        }
        if (m_input->isKeyPressed(Input::Key::LEFT) || m_input->isKeyPressed(Input::Key::A)) {
            fmt::print("LEFT key pressed\n");
            m_player->moveLeft(deltaTime);
        }
        if (m_input->isKeyPressed(Input::Key::RIGHT) || m_input->isKeyPressed(Input::Key::D)) {
            fmt::print("RIGHT key pressed\n");
            m_player->moveRight(deltaTime);
        }
        if (m_input->isKeyPressed(Input::Key::SPACE)) {
            fmt::print("SPACE key pressed\n");
        }
        if (m_input->isKeyPressed(Input::Key::ESC)) {
            fmt::print("ESC key pressed\n");

            m_renderer->printCurrentMatrix(m_player);

            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }

        // update gamestate
        updateEnemies(deltaTime);

        // render
        m_renderer->render(m_window, m_enemies, m_player);

        std::this_thread::sleep_for(std::chrono::milliseconds(10));
        lastFrameTime = currentTime;
    }
}

void Game::cleanup() {
    // m_bufferManager.cleanup(m_VAO, m_VBO);
    // glDeleteProgram(m_shaderProgram);
    glfwDestroyWindow(m_window);
    glfwTerminate();
}