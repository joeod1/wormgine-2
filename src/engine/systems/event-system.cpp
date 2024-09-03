#include <entt/entt.hpp>
#include <SFML/Window.hpp>

#include "system.hpp"
#include "event-router.hpp"
#include "event-system.hpp"


Engine::EventSystem::EventSystem(std::shared_ptr<sf::RenderWindow> window) {
    this->window = window;
    window->setKeyRepeatEnabled(false);
}

void Engine::EventSystem::update(entt::registry &registry) {
    sf::Event event;
    while (window->pollEvent(event)) {

        if (event.type == sf::Event::EventType::Closed) {
            window->close();
        }

        EventRouter::getInstance().onInput(event);
    }
}