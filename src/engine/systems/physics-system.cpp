#include "physics-system.hpp"
#include "components.hpp"

#include <entt/entt.hpp>
#include <box2d/math_functions.h>
#include <iostream>

void Engine::PhysicsSystem::start(entt::registry &registry) {
    worldDef = b2DefaultWorldDef();
    worldDef.gravity = (b2Vec2){0.0f, 9.8f};
    
    worldId = b2CreateWorld(&worldDef);

    registry.on_construct<Physical>().connect<&PhysicsSystem::generateWorldProp>(this);
}

void Engine::PhysicsSystem::update(entt::registry &registry) {
    b2World_Step(worldId, 0.0166f, 4);
    b2ContactEvents events = b2World_GetContactEvents(worldId);
    
    // for (int i = 0; i < events.beginCount; i++) {
    //     std::cout << "Collision! \n";

    //     // events.beginEvents[i].shapeIdA// ->shapeIdA;
    // }

    for (auto [entity, bodyId] : registry.view<b2BodyId>().each()) {
        b2Vec2 pos = b2Body_GetPosition(bodyId);
        b2Rot rot = b2Body_GetRotation(bodyId);

        // b2ContactData contact;
        // b2Body_GetContactData(bodyId, &contact, 100);
        

        registry.patch<Position>(entity, [pos](Position &ipos) { ipos.x = pos.x; ipos.y = pos.y; });
        registry.emplace_or_replace<b2Rot>(entity, rot);
    }
}

void Engine::PhysicsSystem::generateWorldProp(entt::registry &registry, entt::entity entity) {
    auto [pos, sz, sh, phys] = registry.try_get<Position, Scale, Shape, Physical>(entity);

    b2BodyDef bodyDef = b2DefaultBodyDef();
    bodyDef.type = phys->type;
    bodyDef.position = (b2Vec2){pos->x, pos->y};

    b2BodyId bodyId = b2CreateBody(worldId, &bodyDef);
    registry.emplace<b2BodyId>(entity, bodyId);

    b2Polygon physShape;
    switch (sh->shape) {
    case Shapes::rectangle:
        physShape = b2MakeBox(sz->x, sz->y);
        break;
    case Shapes::circle:
        // To be implmented
    default:
        physShape = b2MakeBox(sz->x, sz->y);
    }

    b2ShapeDef shapeDef = b2DefaultShapeDef();
    b2CreatePolygonShape(bodyId, &shapeDef, &physShape);
}