#include <string>
#include <SFML/Window.hpp>
#include <entt/entt.hpp>

#include "game.hpp"

#include "system.hpp"
#include "renderer.hpp"
#include "event-system.hpp"
#include "topdown-controller.hpp"
#include "physics-system.hpp"


Engine::Game::Game() {
    registry = entt::registry();

    Renderer *renderer = new Renderer(loadPath);
    window = renderer->getWindow();
    systems.push_back(renderer);
    systems.push_back(new PhysicsSystem());
    systems.push_back(new EventSystem(window));
    systems.push_back(new TopdownController());

    for (auto sys: systems) {
        sys->start(registry);
    }

    update();
}

/** 
 * Initializes the game with a specific scenepath `path`.
 **/
Engine::Game::Game(std::string path) {
    loadPath = path;
    Game();
}


void Engine::Game::update() {
    while (window->isOpen()) {
        
        for (auto sys: systems) {
            sys->update(registry);
        }
    }
}
        