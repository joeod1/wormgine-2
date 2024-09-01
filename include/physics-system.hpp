#ifndef physics_system_hpp
#define physics_system_hpp

#include <entt/entt.hpp>
#include <box2d/box2d.h>

#include "system.hpp"


namespace Engine {
    class PhysicsSystem : public System {
    private:
        b2WorldDef worldDef;
        b2WorldId worldId;

    public:
        void start(entt::registry &);
        void update(entt::registry &);
        void generateWorldProp(entt::registry &, entt::entity);
    };
};

#endif