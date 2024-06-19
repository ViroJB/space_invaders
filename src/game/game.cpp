#include "game.h"

Game::Game() {
    m_glfwManager.initialize();

    m_window = m_glfwManager.createWindow(800, 600, "Space Invaders Clone");

    m_glewManager.initialize();

    // more temp stuff
    std::vector<float> playerVertices = {
        // position            // color + alpha
        -0.875f, 1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top, red
        -1.00f,  0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // bottom left, green
        -0.75f,  0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f   // bottom right, blue
    };
    std::vector<float> enemyVertices = {
        // position            // color + alpha
        -0.875f, 0.75f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // bottom, red
        -1.00f,  1.0f,  0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top left, green
        -0.75f,  1.0f,  0.0f, 0.0f, 0.0f, 1.0f, 1.0f   // top right, blue
    };
    std::vector<float> projectileVertices = {
        // position            // color + alpha
        -0.875f, 1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top, red
        -1.00f,  0.75f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // bottom left, green
        -0.75f,  0.75f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f   // bottom right, blue
    };
    m_vertices.push_back(playerVertices);
    m_vertices.push_back(enemyVertices);
    m_vertices.push_back(projectileVertices);

    m_renderer = new Renderer();
    m_renderer->initializeVertexData(m_vertices);

    m_input = new Input(m_window);

    glEnable(GL_BLEND);                                 // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // enable transparency

    // note temp stuff
    m_player = new GameObject(0.875f, -1.75f, {0.5f, 0.1f, 0.2f, 1.0f});
    m_player->setSpeed(5.0f);
    m_player->calculateBoundingBox(m_vertices[0]);
}

void Game::spawnProjectile() {
    // create random number between -1.0 and 1.0
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disPositionX(0.0f, 1.875f);
    std::uniform_real_distribution<float> disPositionY(-2.0f, 0.0f);
    std::uniform_real_distribution<float> disColor(0.0f, 1.0f);

    GameObject projectile = GameObject(m_player->getBoundingBox().x, m_player->getBoundingBox().y, {0.1f, 0.1f, 0.1f, 1.0f});
    projectile.calculateBoundingBox(m_vertices[2]);
    projectile.setSpeed(5.0f);
    m_projectiles.push_back(projectile);
}

void Game::updateProjectiles(float deltaTime) {
    for (auto projectile = m_projectiles.begin(); projectile != m_projectiles.end();) {
        projectile->moveUp(deltaTime);

        if (projectile->getBoundingBox().y > 0.0f) {
            projectile = m_projectiles.erase(projectile);  // erase returns the iterator to the next valid element
            fmt::print("Projectile removed\n");
        } else {
            ++projectile;  // move to the next element
        }
    }
}

void Game::spawnEnemy() {
    // create random number between -1.0 and 1.0
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disPositionX(0.0f, 1.875f);
    std::uniform_real_distribution<float> disPositionY(-2.0f, 0.0f);
    std::uniform_real_distribution<float> disColor(0.0f, 1.0f);

    GameObject enemy = GameObject(disPositionX(gen), 0.0f, {disColor(gen), disColor(gen), disColor(gen), 1.0f});
    enemy.calculateBoundingBox(m_vertices[1]);
    m_enemies.push_back(enemy);
}

void Game::updateEnemies(float deltaTime) {
    if (m_enemies.size() <= 9) {
        spawnEnemy();
    }

    // this feels so wrong...
    for (auto enemy = m_enemies.begin(); enemy != m_enemies.end();) {
        enemy->moveDown(deltaTime);

        if (enemy->getBoundingBox().y < -2.0f) {
            enemy = m_enemies.erase(enemy);  // erase returns the iterator to the next valid element
            fmt::print("Enemy removed\n");
        } else {
            ++enemy;  // move to the next element
        }
    }
}

void Game::run() {
    std::chrono::time_point<std::chrono::high_resolution_clock> lastFrameTime = std::chrono::high_resolution_clock::now();

    while (!glfwWindowShouldClose(m_window)) {
        // delta time
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> elapsed = currentTime - lastFrameTime;
        float deltaTime = elapsed.count();
        // fmt::print("delta time: {}\n", deltaTime);
        // fmt::print("FPS: {}\n", 1.0f / deltaTime);

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
            spawnProjectile();
            printAllBoundingBoxes();
        }
        if (m_input->isKeyPressed(Input::Key::ESC)) {
            fmt::print("ESC key pressed\n");

            m_renderer->printCurrentMatrix(m_player);
            m_renderer->printCurrentMatrix(&m_enemies.front());
            // print bounding box of player
            auto playerBoundingBox = m_player->getBoundingBox();
            fmt::print("Player bounding box: x: {}, y: {}, width: {}, height: {}\n", playerBoundingBox.x, playerBoundingBox.y,
                       playerBoundingBox.width, playerBoundingBox.height);

            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }

        // update gamestate
        updateEnemies(deltaTime);
        updateProjectiles(deltaTime);

        // render
        m_renderer->render(m_window, m_enemies, m_player, m_projectiles);

        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        lastFrameTime = currentTime;
    }
}

void Game::printAllBoundingBoxes() {
    // print bounding box of player
    auto playerBoundingBox = m_player->getBoundingBox();
    fmt::print("Player bounding box: x: {}, y: {}, width: {}, height: {}\n", playerBoundingBox.x, playerBoundingBox.y,
               playerBoundingBox.width, playerBoundingBox.height);

    // print bounding box of enemies
    for (auto& enemy : m_enemies) {
        auto enemyBoundingBox = enemy.getBoundingBox();
        fmt::print("Enemy bounding box: x: {}, y: {}, width: {}, height: {}\n", enemyBoundingBox.x, enemyBoundingBox.y,
                   enemyBoundingBox.width, enemyBoundingBox.height);
    }

    // print bounding box of projectiles
    for (auto& projectile : m_projectiles) {
        auto projectileBoundingBox = projectile.getBoundingBox();
        fmt::print("Projectile bounding box: x: {}, y: {}, width: {}, height: {}\n", projectileBoundingBox.x, projectileBoundingBox.y,
                   projectileBoundingBox.width, projectileBoundingBox.height);
    }
}

void Game::cleanup() {
    // m_bufferManager.cleanup(m_VAO, m_VBO);
    // glDeleteProgram(m_shaderProgram);
    glfwDestroyWindow(m_window);
    glfwTerminate();
}