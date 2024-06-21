#include "app.h"

App::App() {
    Game* game = new Game;
    game->run();
    game->cleanup();
}

int main() {
    auto seed = static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count());
    std::default_random_engine gen2(seed);

    App* app = new App;
    return 0;
}