#ifndef event_router_hpp
#define event_router_hpp

#include <entt/entt.hpp>
#include <SFML/Window.hpp>

namespace Engine {

    class EventRouter {
    public:
        static EventRouter& getInstance() {
            static EventRouter instance;
            return instance;
        }

        entt::delegate<void(sf::Event&)> onInput{};
    };
};

#endif