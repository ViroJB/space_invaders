#include "game.h"

// TODO if the renderer only takes in a gamestate, we could save that gamestate and redraw it, frame by frame
Game::Game() {
    m_glfwManager.initialize();
    m_window = m_glfwManager.createWindow(800, 600, "Space Invaders Clone");
    m_glewManager.initialize();

    m_vertices = getVertices();

    m_renderer = new Renderer();
    m_renderer->initializeVertexData(m_vertices);

    m_input = new Input(m_window);

    // create player
    m_player = new Player(&m_vertices[0], {0.5f, 0.1f, 0.2f, 1.0f});
}

void Game::spawnProjectile() {
    BoundingBox* playerBoundingBox = m_player->getGameObject()->getBoundingBox();
    float x = playerBoundingBox->x + playerBoundingBox->width / 2;
    float y = playerBoundingBox->y + playerBoundingBox->height;

    // center the projectile above the player, currently manually
    x -= 0.017f;
    y -= 0.11f;

    GameObject projectile = GameObject(x, y, &m_vertices[2]);
    projectile.calculateBoundingBox(&m_vertices[2]);
    projectile.setSpeed(5.0f);
    projectile.setColor({0.15f, 0.15f, 0.15f, 1.0f});
    m_projectiles.push_back(projectile);

    m_player->triggerShootingCooldown();
}

void Game::updateProjectiles(float deltaTime) {
    for (auto projectile = m_projectiles.begin(); projectile != m_projectiles.end();) {
        projectile->moveUp(deltaTime);

        if (projectile->getBoundingBox()->y > 0.0f) {
            projectile = m_projectiles.erase(projectile);  // erase returns the iterator to the next valid element
            // fmt::print("Projectile removed\n");
        } else {
            ++projectile;  // move to the next element
        }
    }
}

void Game::spawnEnemy() {
    Enemy enemy = Enemy(getRandomNumber(0.0f, 1.875f), 0.0f, &m_vertices[1]);
    enemy.setColor({getRandomNumber(0.0f, 1.0f), getRandomNumber(0.0f, 1.0f), getRandomNumber(0.0f, 1.0f), 1.0f});
    m_enemies.push_back(enemy);
}

void Game::updateEnemies(float deltaTime) {
    // enemies should never be 0 with this, so no zero check needed
    if (m_enemies.size() <= 9) {
        spawnEnemy();
    }

    // this feels weird
    for (auto enemy = m_enemies.begin(); enemy != m_enemies.end();) {
        // this could be further down in this loop, might save some iterations and be more logical?
        bool enemyErased = false;
        if (!m_projectiles.empty()) {
            for (auto projectile = m_projectiles.begin(); projectile != m_projectiles.end();) {
                // delete enemy and projectile on collition
                if (m_collisionDetection.isColliding(enemy->getGameObject(), &(*projectile))) {
                    enemy = m_enemies.erase(enemy);
                    projectile = m_projectiles.erase(projectile);
                    enemyErased = true;
                    break;  // enemy is also deleted, so fully breakout
                } else {
                    ++projectile;
                }
            }
        }

        if (enemyErased) {
            continue;
        }

        enemy->moveDown(deltaTime);

        // delete enemy if it goes off screen
        if (enemy->getBoundingBox()->y < -2.0f) {
            enemy = m_enemies.erase(enemy);  // erase returns the iterator to the next valid element
        } else {
            ++enemy;
        }
    }
}

void Game::updatePlayer(float deltaTime) {
    m_player->updateShootingCooldown(deltaTime);
}

void Game::run() {
    while (!glfwWindowShouldClose(m_window)) {
        // delta time
        Timer::updateDeltaTime();
        // fmt::print("delta time: {}\n", deltaTime);
        // fmt::print("FPS: {}\n", 1.0f / deltaTime);

        // process input
        if (m_input->isKeyPressed(Input::Key::UP) || m_input->isKeyPressed(Input::Key::W)) {
            m_player->moveUp(Timer::getDeltaTime());
        }
        if (m_input->isKeyPressed(Input::Key::DOWN) || m_input->isKeyPressed(Input::Key::S)) {
            m_player->moveDown(Timer::getDeltaTime());
        }
        if (m_input->isKeyPressed(Input::Key::LEFT) || m_input->isKeyPressed(Input::Key::A)) {
            m_player->moveLeft(Timer::getDeltaTime());
        }
        if (m_input->isKeyPressed(Input::Key::RIGHT) || m_input->isKeyPressed(Input::Key::D)) {
            m_player->moveRight(Timer::getDeltaTime());
        }
        if (m_input->isKeyPressed(Input::Key::SPACE)) {
            if (m_player->getShootingCooldown() <= 0.0f) {
                spawnProjectile();
            }
        }
        if (m_input->isKeyPressed(Input::Key::ESC)) {
            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }

        // update gamestate
        updateEnemies(Timer::getDeltaTime());
        updateProjectiles(Timer::getDeltaTime());
        updatePlayer(Timer::getDeltaTime());

        // render
        m_renderer->render(m_window, m_enemies, m_player->getGameObject(), m_projectiles);

        // todo add calculation to run at a certain fps
        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}

void Game::cleanup() {
    // glDeleteProgram(m_shaderProgram);
    glfwDestroyWindow(m_window);
    glfwTerminate();

    delete m_input;
    delete m_renderer;
    delete m_player;
}

std::vector<std::vector<float>> Game::getVertices() {
    std::vector<std::vector<float>> vertices;
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
        -0.985f, 1.0f,  0.0f, 1.0f, 0.0f, 0.0f, 1.0f,  // top, red
        -1.0f,   0.85f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // bottom left, green
        -0.97f,  0.85f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f   // bottom right, blue
    };
    vertices.push_back(playerVertices);
    vertices.push_back(enemyVertices);
    vertices.push_back(projectileVertices);

    return vertices;
}

float Game::getRandomNumber(float min, float max) {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(min, max);
    return dis(gen);
}