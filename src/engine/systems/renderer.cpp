#include "renderer.hpp"
#include <string>
#include <format>

#define PI 3.14159265f

void Engine::Renderer::init() {
    primaryFont.loadFromFile("Tuffy_Bold_Font/Tuffy_Bold.ttf");

    window = std::make_shared<sf::RenderWindow>(sf::VideoMode(1920, 1080), title, sf::Style::Fullscreen, sf::ContextSettings(0U, 0U, 6U, 1U, 1U, 0U, false));
    window->setFramerateLimit(60);
    primaryView.setSize(1920, 1080);
}

void Engine::Renderer::start(entt::registry &registry) {
    registry.on_construct<Shape>().connect<&Engine::Renderer::generateShape>(this);
}

void Engine::Renderer::update(entt::registry &registry) {
    render(registry);
};

void Engine::Renderer::generateShape(entt::registry &registry, entt::entity entity) {
    auto [scale, shape, position, color] = registry.try_get<Scale, Shape, Position, Color>(entity);

    // registry.emplace<Transformable>(entity, std::make_shared<sf::Transform>(sf::Vector2f(position->x, position->y)));

    std::shared_ptr<sf::Drawable> drawnShape;
    switch (shape->shape) {
    case Shapes::rectangle:
        std::shared_ptr<sf::RectangleShape> rect = std::make_shared<sf::RectangleShape>(
            sf::Vector2f(scale->x * 100, scale->y * 100)
        );
        rect->setFillColor(color->color);
        rect->setOrigin(sf::Vector2f(scale->x * 50, scale->y * 50));
        drawnShape = rect;
    }

    registry.emplace_or_replace<Drawable>(entity, drawnShape);
}

void Engine::Renderer::render(entt::registry &registry) {
    window->clear();

    float dt = abs(lastFrameTime - clock.getElapsedTime().asSeconds());
    clock.restart();

    sf::Text fps;
    static std::string output;
    static int i = 0;
    i++;
    if (i % 10 == 0) {
        output = std::format("fps: {}", 1.f/dt);
    }

    // std::sprintf(out, "%d", 1.f/dt)
    fps.setFont(primaryFont);
    fps.setString(output);
    fps.setCharacterSize(48);
    fps.setFillColor(sf::Color::White);
    fps.setPosition(100, 100);

    auto entity = registry.view<Camera, Position>().front();
    auto pos = registry.get<Position>(entity);
    float y = pos.y * 50;
    y -= pos.y * 1080 / 100;
    primaryView.setCenter(pos.x * 50, y);
    window->setView(primaryView);

    auto view = registry.view<Drawable, Position, Scale, b2Rot>();

    for (auto [entity, draw, pos, sz, rot]: view.each()) {
        sf::Transform transform;
        transform.translate((pos.x) * 50, (pos.y) * 50);
        transform.rotate(atan2(rot.s, rot.c) * 180 / PI);
        window->draw(*draw.drawable, transform);
    }

    window->draw(fps);


    window->display();
}


std::shared_ptr<sf::RenderWindow> Engine::Renderer::getWindow() {
    std::cout << window->isOpen();
    return window;
}