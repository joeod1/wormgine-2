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
    b2World_Step(worldId, 0.0166f * 2, 4);
    b2ContactEvents events = b2World_GetContactEvents(worldId);
    
    for (int i = 0; i < events.beginCount; i++) {
        // std::cout << "Collision! \n";
        auto body = b2Shape_GetBody(events.beginEvents[i].shapeIdA);
        auto bodyB = b2Shape_GetBody(events.beginEvents[i].shapeIdB);
        void *entityptr = b2Body_GetUserData(body);
        if (entityptr == nullptr) continue;

        auto *entity = static_cast<entt::entity*>(entityptr);
        if (registry.valid(*entity)) {
            auto callback = registry.try_get<OnCollisionBegin>(*entity);
            // callback->callback(*static_cast<entt::entity*>(b2Body_GetUserData(b2Shape_GetBody(events.beginEvents[i].shapeIdB))), events.beginEvents[i]);
            // registry.patch<Position>(entity, [](auto &pos) {});
            std::cout<< "Popped";
            // b2Body_ApplyLinearImpulseToCenter(body, b2Vec2{0, -5}, true);
            // b2Body_ApplyLinearImpulseToCenter(bodyB, b2Vec2{0, -5}, true);
            // registry.patch<Position>(*entity, [](Position &pos) { pos.y -=100; });
            //registry.try_get<Position>
        }
    }

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

    bodyDef.userData = new entt::entity{entity};

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