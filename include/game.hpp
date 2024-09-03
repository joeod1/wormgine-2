#include <string>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <entt/entt.hpp>

#include "system.hpp"


namespace Engine {
    class Game {
    public:
        std::string name;
        std::string loadPath = "./scene.json";

        std::shared_ptr<sf::RenderWindow> window;

        entt::registry registry;
        std::vector<std::unique_ptr<System>> systems;


        /**
        * Initializes the game with the current loadPath, `./scene.json` by default.
        **/
        Game();

        /** 
         * Initializes the game with a specific scenepath `path`.
         **/
        Game(std::string path);


        void update();
        
    };
};
