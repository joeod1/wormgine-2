#ifndef system_hpp
#define system_hpp
#include <entt/entt.hpp>


namespace Engine {
    class System {
    public:
        virtual void start(entt::registry &registry) {};
        virtual void update(entt::registry &registry) {};
    };
};
#endif