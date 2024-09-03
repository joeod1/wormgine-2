#ifndef components_hpp
#define components_hpp

#include <box2d/box2d.h>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics.hpp>

struct Position {
    float x = 0, y = 0;
};

struct Scale {
    float x = 1, y = 1;
};

struct Velocity {
    float x, y;
};

enum Shapes {
    circle=0,
    rectangle = 1,
};

struct Shape {
    Shapes shape;
};

struct Color {
    sf::Color color;
};

struct Controllable {
    bool controller = false;
    int controllerId = -1;
};

struct KeyboardControls {
};

struct VelocityIntent {
    float x=0, y=0;
};

struct Transformable {
    std::shared_ptr<sf::Transform> transform;
};

struct Drawable {
    std::shared_ptr<sf::Drawable> drawable;
};

struct Physical {
    b2BodyType type = b2BodyType::b2_dynamicBody;
};

struct CameraFollowing {
    entt::entity entity;
};

struct Camera {};

struct OnCollisionBegin {
    entt::delegate<void(entt::registry &,entt::entity, b2ContactBeginTouchEvent)> callback;
};

struct OnCollisionEnd {
    entt::delegate<void(entt::registry &,entt::entity, b2ContactEndTouchEvent)> callback;
};

#endif