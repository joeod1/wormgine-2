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
    registry.on_destroy<b2BodyId>().connect<&PhysicsSystem::destroyBody>(this);
}

void Engine::PhysicsSystem::update(entt::registry &registry) {
    b2World_Step(worldId, 0.0166f * 2, 4);
    b2ContactEvents events = b2World_GetContactEvents(worldId);
    // b2World_GetBodyEvents(worldId).moveEvents[0].;
    
    // for (int i = 0; i < events.beginCount; i++) {
    //     auto body = b2Shape_GetBody(events.beginEvents[i].shapeIdA);
    //     auto bodyB = b2Shape_GetBody(events.beginEvents[i].shapeIdB);
    //     void *entityptr = b2Body_GetUserData(body);
    //     if (entityptr == nullptr) continue;

    //     auto *entity = static_cast<entt::entity*>(entityptr);
    //     if (registry.valid(*entity)) {
    //         auto callback = registry.try_get<OnCollisionBegin>(*entity);
    //         entt::entity ent2 = *static_cast<entt::entity*>(b2Body_GetUserData(b2Shape_GetBody(events.beginEvents[i].shapeIdB)));
    //         callback->callback(ent2, events.beginEvents[i]);
    //     }
    // }
    updatePositions(registry);
    sendContactEvents(registry);
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

    registry.emplace_or_replace<b2Rot>(entity);
}

void Engine::PhysicsSystem::destroyBody(entt::registry &registry, entt::entity entity) {
    b2BodyId *bodyId = registry.try_get<b2BodyId>(entity);
    if (bodyId == nullptr) return;

    if (b2Body_IsValid(*bodyId)) {
        b2DestroyBody(*bodyId);
    }
    // registry.remove<b2BodyId>(entity);
}

entt::entity Engine::PhysicsSystem::getEntity(void* userData) {
    return *static_cast<entt::entity*>(userData);
}

entt::entity Engine::PhysicsSystem::getEntity(b2BodyId bodyId) {
    return getEntity(b2Body_GetUserData(bodyId));
}

entt::entity Engine::PhysicsSystem::getEntity(b2ShapeId shapeId) {
    return getEntity(b2Shape_GetBody(shapeId));
}

void Engine::PhysicsSystem::updatePositions(entt::registry &registry) {
    // Get a list of all the movements that occurred
    auto [events, ct] = b2World_GetBodyEvents(worldId);

    for (int i = 0; i < ct; i++) {
        // Get the entity corresponding to each movement
        b2BodyMoveEvent event = events[i];
        if (!b2Body_IsValid(event.bodyId)) continue;
        entt::entity entity = getEntity(event.userData);

        // Ensure the entity is valid. Delete if it isn't
        if (registry.valid(entity)) {
            // Ensure the entity still has position. Delete if it doesn't
            auto pos = registry.try_get<Position>(entity);
            if (pos == nullptr) { destroyBody(registry, entity); continue; }

            // Update the position and rotation of the entity
            registry.patch<Position>(entity, 
                [event](Position &pos){
                    pos.x = event.transform.p.x; 
                    pos.y = event.transform.p.y;
                });
            registry.emplace_or_replace<b2Rot>(entity, event.transform.q);

        } else {
            b2DestroyBody(event.bodyId);
        }
    }
}

void Engine::PhysicsSystem::updatePositionsFull(entt::registry &registry) {
    for (auto [entity, bodyId] : registry.view<b2BodyId>().each()) {
        if (!b2Body_IsValid(bodyId)) continue;

        b2Vec2 pos = b2Body_GetPosition(bodyId);
        b2Rot rot = b2Body_GetRotation(bodyId);

        registry.patch<Position>(entity, [pos](Position &ipos) { ipos.x = pos.x; ipos.y = pos.y; });
        registry.emplace_or_replace<b2Rot>(entity, rot);
    }
}

void Engine::PhysicsSystem::sendContactEvents(entt::registry &registry) {
    auto events = b2World_GetContactEvents(worldId);

    // Contact beginning
    for (int i = 0; i < events.beginCount; i++) {
        b2ContactBeginTouchEvent event = events.beginEvents[i];
        if (!b2Shape_IsValid(event.shapeIdA) || !b2Shape_IsValid(event.shapeIdB)) continue;
        entt::entity A = getEntity(event.shapeIdA);
        entt::entity B = getEntity(event.shapeIdB);

        auto aEvent = registry.try_get<OnCollisionBegin>(A);
        if (aEvent != nullptr) {
            aEvent->callback(registry, B, event);
        }

        auto bEvent = registry.try_get<OnCollisionBegin>(B);
        if (bEvent != nullptr) {
            bEvent->callback(registry, A, event);
        }
    }

    // Contact ending
    for (int i = 0; i < events.endCount; i++) {
        b2ContactEndTouchEvent event = events.endEvents[i];
        if (!b2Shape_IsValid(event.shapeIdA) || !b2Shape_IsValid(event.shapeIdB)) continue;
        entt::entity A = getEntity(event.shapeIdA);
        entt::entity B = getEntity(event.shapeIdB);

        auto aEvent = registry.try_get<OnCollisionEnd>(A);
        if (aEvent != nullptr) {
            aEvent->callback(registry, B, event);
        }

        auto bEvent = registry.try_get<OnCollisionEnd>(B);
        if (bEvent != nullptr) {
            bEvent->callback(registry, A, event);
        }
    }
}
