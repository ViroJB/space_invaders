#include "game.h"

// TODO if the renderer only takes in a gamestate, we could save that gamestate and redraw it, frame by frame
Game::Game() {
    m_glfwManager.initialize();
    m_window = m_glfwManager.createWindow(800, 600, "Space Invaders Clone");
    m_glewManager.initialize();

    // note more temp stuff
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
    m_vertices.push_back(playerVertices);
    m_vertices.push_back(enemyVertices);
    m_vertices.push_back(projectileVertices);

    m_renderer = new Renderer();
    m_renderer->initializeVertexData(m_vertices);

    m_input = new Input(m_window);

    glEnable(GL_BLEND);                                 // enable blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);  // enable transparency

    // note temp stuff
    m_player = new Player(&m_vertices[0], {0.5f, 0.1f, 0.2f, 1.0f});
}

void Game::spawnProjectile() {
    // create random number between -1.0 and 1.0
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disPositionX(0.0f, 1.875f);
    std::uniform_real_distribution<float> disPositionY(-2.0f, 0.0f);
    std::uniform_real_distribution<float> disColor(0.0f, 1.0f);

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

    // print boundingbox of projectile
    // auto projectileBoundingBox = projectile.getBoundingBox();
    // fmt::print("Projectile bounding box: x: {}, y: {}, width: {}, height: {}\n", projectileBoundingBox->x, projectileBoundingBox->y,
    //            projectileBoundingBox->width, projectileBoundingBox->height);
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
    // create random number between -1.0 and 1.0
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> disPositionX(0.0f, 1.875f);
    std::uniform_real_distribution<float> disPositionY(-2.0f, 0.0f);
    std::uniform_real_distribution<float> disColor(0.0f, 1.0f);

    Enemy enemy = Enemy(disPositionX(gen), 0.0f, &m_vertices[1]);
    enemy.setColor({disColor(gen), disColor(gen), disColor(gen), 1.0f});
    m_enemies.push_back(enemy);
}

void Game::updateEnemies(float deltaTime) {
    if (m_enemies.size() <= 9) {
        spawnEnemy();
    }

    // this feels so wrong...
    for (auto enemy = m_enemies.begin(); enemy != m_enemies.end();) {
        enemy->moveDown(deltaTime);

        if (enemy->getBoundingBox()->y < -2.0f) {
            enemy = m_enemies.erase(enemy);  // erase returns the iterator to the next valid element
            // fmt::print("Enemy removed\n");
        } else {
            ++enemy;  // move to the next element
        }
    }
}

void Game::updatePlayer(float deltaTime) {
    m_player->updateShootingCooldown(deltaTime);
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
            m_player->moveUp(deltaTime);
        }
        if (m_input->isKeyPressed(Input::Key::DOWN) || m_input->isKeyPressed(Input::Key::S)) {
            m_player->moveDown(deltaTime);
        }
        if (m_input->isKeyPressed(Input::Key::LEFT) || m_input->isKeyPressed(Input::Key::A)) {
            m_player->moveLeft(deltaTime);
        }
        if (m_input->isKeyPressed(Input::Key::RIGHT) || m_input->isKeyPressed(Input::Key::D)) {
            m_player->moveRight(deltaTime);
        }
        if (m_input->isKeyPressed(Input::Key::SPACE)) {
            if (m_player->getShootingCooldown() <= 0.0f) {
                spawnProjectile();
            }
            // printAllBoundingBoxes();
        }
        if (m_input->isKeyPressed(Input::Key::ESC)) {
            fmt::print("ESC key pressed\n");

            // printAllBoundingBoxes();
            // m_renderer->printCurrentMatrix(m_player->getGameObject());
            // m_renderer->printCurrentMatrix(&m_enemies.front());
            // print bounding box of player
            // auto playerBoundingBox = m_player->getGameObject()->getBoundingBox();
            // fmt::print("Player bounding box: x: {}, y: {}, width: {}, height: {}\n", playerBoundingBox->x, playerBoundingBox->y,
            //            playerBoundingBox->width, playerBoundingBox->height);

            glfwSetWindowShouldClose(m_window, GLFW_TRUE);
        }

        // update gamestate
        checkForBoxCollisions();
        updateEnemies(deltaTime);
        updateProjectiles(deltaTime);
        updatePlayer(deltaTime);

        // render
        m_renderer->render(m_window, m_enemies, m_player->getGameObject(), m_projectiles);

        // std::this_thread::sleep_for(std::chrono::milliseconds(10));
        lastFrameTime = currentTime;
    }
}

bool Game::checkForPreciseCollision(GameObject& object1, GameObject& object2) {
    // Get the transformation matrices for both objects
    glm::mat4 transform1 = object1.getModelMatrix();
    glm::mat4 transform2 = object2.getModelMatrix();

    // Get and transform vertices
    auto object1Vertices = object1.getVertices();
    auto object2Vertices = object2.getVertices();

    std::vector<glm::vec2> transformedVertices1 = transformVertices(*object1Vertices, transform1);
    std::vector<glm::vec2> transformedVertices2 = transformVertices(*object2Vertices, transform2);

    bool collision = checkForSATCollision(transformedVertices1, transformedVertices2);

    return collision;
}

bool Game::checkForSATCollision(const std::vector<glm::vec2>& vertices1, const std::vector<glm::vec2>& vertices2) {
    std::vector<glm::vec2> axes;

    // Get the axes from the first polygon
    for (size_t i = 0; i < vertices1.size(); ++i) {
        glm::vec2 p1 = vertices1[i];
        glm::vec2 p2 = vertices1[(i + 1) % vertices1.size()];
        glm::vec2 edge = p2 - p1;
        glm::vec2 axis(-edge.y, edge.x);  // Perpendicular to edge
        axis = glm::normalize(axis);
        axes.push_back(axis);
    }

    // Get the axes from the second polygon
    for (size_t i = 0; i < vertices2.size(); ++i) {
        glm::vec2 p1 = vertices2[i];
        glm::vec2 p2 = vertices2[(i + 1) % vertices2.size()];
        glm::vec2 edge = p2 - p1;
        glm::vec2 axis(-edge.y, edge.x);  // Perpendicular to edge
        axis = glm::normalize(axis);
        axes.push_back(axis);
    }

    // Check for overlap on all axes
    for (const auto& axis : axes) {
        if (!overlapOnAxis(vertices1, vertices2, axis)) {
            return false;  // No collision
        }
    }

    return true;  // Collision detected
}

bool Game::overlapOnAxis(const std::vector<glm::vec2>& vertices1, const std::vector<glm::vec2>& vertices2, const glm::vec2& axis) {
    float min1 = FLT_MAX, max1 = -FLT_MAX;
    float min2 = FLT_MAX, max2 = -FLT_MAX;

    for (const auto& vertex : vertices1) {
        float projection = glm::dot(vertex, axis);
        min1 = std::min(min1, projection);
        max1 = std::max(max1, projection);
    }

    for (const auto& vertex : vertices2) {
        float projection = glm::dot(vertex, axis);
        min2 = std::min(min2, projection);
        max2 = std::max(max2, projection);
    }

    return !(max1 < min2 || max2 < min1);
}

std::vector<glm::vec2> Game::transformVertices(const std::vector<float>& vertices, const glm::mat4& transform) {
    std::vector<glm::vec2> transformedVertices;
    for (size_t i = 0; i < vertices.size(); i += 7) {  // assuming vertices are stored as x, y, z, r, g, b, a
        glm::vec4 vertex(vertices[i], vertices[i + 1], vertices[i + 2], 1.0f);
        glm::vec4 transformedVertex = transform * vertex;
        transformedVertices.push_back(glm::vec2(transformedVertex));
    }
    return transformedVertices;
}

void Game::checkForBoxCollisions() {
    // check for collisions between enemies and projectiles
    for (auto enemy = m_enemies.begin(); enemy != m_enemies.end();) {
        bool enemyErased = false;
        for (auto projectile = m_projectiles.begin(); projectile != m_projectiles.end();) {
            if (enemy->getBoundingBox()->x < projectile->getBoundingBox()->x + projectile->getBoundingBox()->width &&
                enemy->getBoundingBox()->x + enemy->getBoundingBox()->width > projectile->getBoundingBox()->x &&
                enemy->getBoundingBox()->y < projectile->getBoundingBox()->y + projectile->getBoundingBox()->height &&
                enemy->getBoundingBox()->y + enemy->getBoundingBox()->height > projectile->getBoundingBox()->y) {
                // fmt::print("Collision detected\n");
                if (checkForPreciseCollision(*enemy->getGameObject(), *projectile)) {
                    // fmt::print("Precise collision detected\n");
                    enemy = m_enemies.erase(enemy);
                    projectile = m_projectiles.erase(projectile);
                    enemyErased = true;
                }
                break;  // Exit inner loop since the current enemy is erased
            } else {
                ++projectile;
            }
        }
        if (!enemyErased) {
            ++enemy;
        }
    }
}

void Game::cleanup() {
    // m_bufferManager.cleanup(m_VAO, m_VBO);
    // glDeleteProgram(m_shaderProgram);
    glfwDestroyWindow(m_window);
    glfwTerminate();
}