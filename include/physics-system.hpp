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

        /* Updates Position and b2Rot for all possessing entities based on movement events. */
        void updatePositions(entt::registry &);

        /* Updates Position and b2Rot for all entities that have a valid bodyId. */
        void updatePositionsFull(entt::registry &);

        /* Sends beginning, ending, and hitting contact events to entities with callbacks for each. */
        void sendContactEvents(entt::registry &);

    public:
        /* Initializes the box2d world. */
        void start(entt::registry &);

        /* Steps the box2d world forward, updates the relevant components, and calls callbacks. */
        void update(entt::registry &);

        /* Generates the physics object and assigns a bodyId component to the given entity. This should not be called directly. */
        void generateWorldProp(entt::registry &, entt::entity);

        /* Destroys a body */
        void destroyBody(entt::registry &, entt::entity);


        /* Casts a void* pointer into an entity. */
        entt::entity getEntity(void*);

        /* Turns the userData of a box2d body into an entity. */
        entt::entity getEntity(b2BodyId);

        /* Returns the userData of the shape's body casted as an entity. */
        entt::entity getEntity(b2ShapeId);
    };
};

#endif