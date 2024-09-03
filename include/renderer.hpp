#include <SFML/System.hpp>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "system.hpp"
#include "components.hpp"
#include <iostream>
#include <box2d/box2d.h>


namespace Engine {
    class Renderer : public System {
    private:
        //std::map<entt::entity, sf::Shape> shapes;

        void render(entt::registry &registry);
        void generateShape(entt::registry &registry, entt::entity entity);

        void init();

    public:
        std::shared_ptr<sf::RenderWindow> window;
        std::string title = "Game Window";

        float lastFrameTime = 0;
        sf::Clock clock;
        sf::Font primaryFont;

        sf::View primaryView;

        Renderer() {
            init();
        }

        Renderer(std::string ititle) {
            title = ititle;
            init();
        }

        std::shared_ptr<sf::RenderWindow> getWindow();

        void start(entt::registry &registry);
        void update(entt::registry &registry);
    };
};