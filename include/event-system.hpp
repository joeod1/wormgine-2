#ifndef event_system_hpp
#define event_system_hpp

#include <entt/entt.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "system.hpp"

namespace Engine {

    class EventSystem : public System {
        std::shared_ptr<sf::RenderWindow> window;
    public:

        EventSystem(std::shared_ptr<sf::RenderWindow>);
        void update(entt::registry &);
    };
};

#endif