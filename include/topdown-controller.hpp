#include <entt/entt.hpp>
#include <SFML/Graphics.hpp>

#include "system.hpp"
#include "event-system.hpp"
#include "components.hpp"
#include "event-router.hpp"
#include <iostream>


// Header code should be removed at some point

namespace Engine {
    class TopdownController : public System {
        entt::registry *registry;

        
        void handleInput(sf::Event &event) {
            auto playerView = registry->view<KeyboardControls>();
            switch (event.type) {
            case sf::Event::KeyPressed:
                for (auto [entity] : playerView.each()) {
                    registry->patch<VelocityIntent>(entity, [event](VelocityIntent &intent) {
                        switch(event.key.code) {
                        case sf::Keyboard::A:
                            intent.x -= 1;
                            break;
                        case sf::Keyboard::D:
                            intent.x += 1;
                            break;
                        case sf::Keyboard::W:
                            intent.y -= 1;
                            break;
                        case sf::Keyboard::S:
                            intent.y += 1;
                            break;
                        }
                    });

                    if (event.key.code == sf::Keyboard::Q) {
                        for (int i = 1; i < 10; i++) {
                            Scale size{i * 13 * 113 % 5 / 5.f + 0.25f, i * 17 * 243 % 5 / 5.f + 0.25f};
                            Position pos{(i * 1277 * 3929 * 2081) % 80 / 5.f + 20.25f, (i * 1459 * 3037 * 2503) % 40 / 5.f + 0.25f};

                            std::cout << "For i = " << i << ", pos: " << size.x << ", " << size.y << "\n";
                            
                            entt::entity ent = registry->create();


                            registry->emplace<Position>(ent, pos);
                            registry->emplace<Scale>(ent, size);
                            registry->emplace<Color>(ent, sf::Color(i * 11 * 17 * 113 % 255, i * 11 * 19 * 113 % 255, i * 11 * 23 * 113 % 255));
                            registry->emplace<Shape>(ent, Shape{Shapes::rectangle});
                            registry->emplace<Transformable>(ent, Transformable{std::make_shared<sf::Transform>()});
                            registry->emplace<Physical>(ent, Physical{b2BodyType::b2_dynamicBody});
                        }
                    }
                    
                    
                    //intent.x ;
                    //b2Body_ApplyLinearImpulse(bodyId, b2Vec2{0.5f, -9.8f}, b2Body_GetPosition(bodyId), true);
                    // sf::RectangleShape* shape = dynamic_cast<sf::RectangleShape*>(draw.drawable);
                    // shape->setPosition(
                    //     shape->getPosition()
                    //     - sf::Vector2f((event.key.code == sf::Keyboard::A) ? 10 : 0,
                    //                 (event.key.code == sf::Keyboard::W) ? 10 : 0)
                    //     + sf::Vector2f((event.key.code == sf::Keyboard::D) ? 10 : 0,
                    //                 (event.key.code == sf::Keyboard::S) ? 10 : 0)

                    // );
                }
                break;
            case sf::Event::KeyReleased:
                for (auto [entity] : playerView.each()) {
                    registry->patch<VelocityIntent>(entity, [event](VelocityIntent &intent) {
                        switch(event.key.code) {
                        case sf::Keyboard::A:
                            intent.x += 1;
                            break;
                        case sf::Keyboard::D:
                            intent.x -= 1;
                            break;
                        case sf::Keyboard::W:
                            intent.y += 1;
                            break;
                        case sf::Keyboard::S:
                            intent.y -= 1;
                            break;
                        }
                    });
                }
                break;
            default:
                //std::cout<<"\nUnhandled input event\n";
                break;
            }
        }


        void start(entt::registry &registry) {
            this->registry = &registry;
            EventRouter::getInstance().onInput.connect<&TopdownController::handleInput>(this);

            entt::entity character = registry.create();
            registry.emplace<KeyboardControls>(character);

            // sf::RectangleShape *gf = new sf::RectangleShape();
            Scale sz{1, 1};
            // gf->setSize(sf::Vector2f(sz.x * 100, sz.y * 100));
            // gf->setFillColor(sf::Color(255, 50, 255));
            // gf->setOrigin(sz.x * 50, sz.y * 50);
            // registry.emplace<Drawable>(character, gf);

            registry.emplace<Position>(character, Position{8.5f, 5.5f});
            registry.emplace<Scale>(character, sz);
            registry.emplace<Color>(character, sf::Color(255, 50, 255));
            registry.emplace<Shape>(character, Shape{Shapes::rectangle});
            registry.emplace<VelocityIntent>(character);
            registry.emplace<Transformable>(character, Transformable{std::make_shared<sf::Transform>()});
            registry.emplace<Physical>(character);

            entt::entity camera = registry.create();
            registry.emplace<Position>(camera);
            registry.emplace<Camera>(camera);
            registry.emplace<CameraFollowing>(character, camera);



            entt::entity ground = registry.create();

            // sf::RectangleShape *gf1 = new sf::RectangleShape();
            Scale sz1{100, 1.5f};
            // gf1->setSize(sf::Vector2f(sz1.x * 100, sz1.y * 100));
            // gf1->setFillColor(sf::Color(50, 255, 255));
            // gf1->setOrigin(sz1.x * 50, sz1.y * 50);
            // registry.emplace<Drawable>(ground, gf1);

            registry.emplace<Position>(ground, Position{5, 20});
            registry.emplace<Scale>(ground, sz1);
            registry.emplace<Color>(ground, sf::Color(50, 255, 255));
            registry.emplace<Shape>(ground, Shape{Shapes::rectangle});
            registry.emplace<Transformable>(ground, Transformable{std::make_shared<sf::Transform>()});
            registry.emplace<Physical>(ground, Physical{b2BodyType::b2_staticBody});

            
        }


        int i = 0;
        int lastiJump = 0;

        void update(entt::registry &registry) {
            for (auto [entity, intent, bodyId] : registry.view<VelocityIntent, b2BodyId>().each()) {
                
                if (abs(b2Body_GetLinearVelocity(bodyId).x) < 5.f) {
                    b2Body_ApplyLinearImpulseToCenter(bodyId, b2Vec2{intent.x * 5.f, 0}, true);
                }
                if (intent.y < 0 && i - lastiJump > 30) {
                    lastiJump = i;
                    b2Body_ApplyLinearImpulseToCenter(bodyId, b2Vec2{0, -32}, true);
                }

                Position bodyPos = registry.get<Position>(entity);
                CameraFollowing *cam = registry.try_get<CameraFollowing>(entity);
                if (cam != nullptr && registry.valid(cam->entity)) {
                    registry.patch<Position>(cam->entity, [bodyPos](Position &pos){
                        pos.x = bodyPos.x;
                        pos.y = bodyPos.y;
                    });
                }
            }



            i++;

            if (i % 4 == 0) {
                Scale size{i * 13 * 113 % 5 / 5.f + 0.25f, i * 17 * 243 % 5 / 5.f + 0.25f};
                Position pos{(i * 1277 * 3929 * 2081) % 80 / 5.f + 10.25f, (i * 1459 * 3037 * 2503) % 40 / 5.f + 0.25f};

                std::cout << "For i = " << i << ", pos: " << size.x << ", " << size.y << "\n";
                
                entt::entity ent = registry.create();

                // sf::RectangleShape *gf2 = new sf::RectangleShape();
                // gf2->setSize(sf::Vector2f(size.x * 100, size.y * 100));
                // gf2->setFillColor(sf::Color(i * 11 * 17 * 113 % 255, i * 11 * 19 * 113 % 255, i * 11 * 23 * 113 % 255));
                // gf2->setOrigin(size.x * 50, size.y * 50);
                // registry.emplace<Drawable>(ent, gf2);

                registry.emplace<Position>(ent, pos);
                registry.emplace<Scale>(ent, size);
                registry.emplace<Color>(ent, sf::Color(i * 11 * 17 * 113 % 255, i * 11 * 19 * 113 % 255, i * 11 * 23 * 113 % 255));
                registry.emplace<b2Rot>(ent);
                registry.emplace<Shape>(ent, Shape{Shapes::rectangle});
                registry.emplace<Transformable>(ent, Transformable{std::make_shared<sf::Transform>()});
                registry.emplace<Physical>(ent, Physical{b2BodyType::b2_dynamicBody});
            }
        }
    };
};