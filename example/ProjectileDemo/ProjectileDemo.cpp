#include <iostream>
#include <memory>
#include <functional>
#include "CKeyboardCtrl.hpp"
#include "CUtility.hpp"
#include "CProjectiles.hpp"

constexpr auto Width = 800;
constexpr auto Offset = 50;
constexpr auto Height = 600;
constexpr auto FrameLimit = 120;
constexpr auto MaxAngle = 91;
constexpr auto WindowName = "ProjectileTesting";

void menu()
{
    std::cout << "Press Up to start the motion" << std::endl;
}

void spawnBoundry(std::vector<GameEngine::CEntityWrapper<sf::RectangleShape>> &boundry)
{
    GameEngine::CEntityWrapper<sf::RectangleShape> rect(0);
    rect.setSize({Offset, Height});
    rect.setFillColor(sf::Color::White);
    rect.setPosition(0, 0);
    boundry.push_back(rect);

    GameEngine::CEntityWrapper<sf::RectangleShape> rect2(180);
    rect2.setSize({Offset, Height});
    rect2.setFillColor(sf::Color::White);
    rect2.setPosition(Width - Offset, 0);
    boundry.push_back(rect2);

    GameEngine::CEntityWrapper<sf::RectangleShape> rect3(90);
    rect3.setSize({Width, Offset});
    rect3.setFillColor(sf::Color::White);
    rect3.setPosition(0, 0);
    boundry.push_back(rect3);

    GameEngine::CEntityWrapper<sf::RectangleShape> rect4(-90);
    rect4.setSize({Width, Offset});
    rect4.setFillColor(sf::Color::White);
    rect4.setPosition(0, Height - Offset);
    boundry.push_back(rect4);
}

void projectileTesting()
{
    std::vector<GameEngine::CEntityWrapper<sf::RectangleShape>> boundry;
    spawnBoundry(boundry);

    std::vector<std::shared_ptr<GameEngine::Projectile::CProjectile>> bullets;
    std::vector<std::function<void(float)>> fptrs;
    std::vector<float> angles;

    bullets.push_back(std::make_shared<GameEngine::Projectile::CProjectile>(15.f, 4, sf::Color::Cyan, 0, sf::Color::Red));
    bullets[0]->spawn({Width / 4, Height / 4}, 2);
    bullets[0]->setAttribute(GameEngine::Attribute::AttributeTypes::reflects);
    bullets[0]->setAttribute(GameEngine::Attribute::AttributeTypes::spins);
    bullets[0]->setAttribute(GameEngine::Attribute::AttributeTypes::fades);
    fptrs.push_back(std::bind(&GameEngine::Projectile::CProjectile::shoot, bullets[0], std::placeholders::_1));
    angles.push_back(rand() % MaxAngle);

    bullets.push_back(std::make_shared<GameEngine::Projectile::CProjectile>(20.f, 5, sf::Color::Red, 0, sf::Color::Red));
    bullets[1]->spawn({Width / 3, Height / 3}, 4);
    bullets[1]->setAttribute(GameEngine::Attribute::AttributeTypes::reflects);
    bullets[1]->setAttribute(GameEngine::Attribute::AttributeTypes::shrinks);
    fptrs.push_back(std::bind(&GameEngine::Projectile::CProjectile::shoot, bullets[1], std::placeholders::_1));
    angles.push_back(rand() % MaxAngle);

    bullets.push_back(std::make_shared<GameEngine::Projectile::CProjectile>(50.f, 3, sf::Color::Green, 0, sf::Color::Red));
    bullets[2]->spawn({Width / 2, Height / 2}, 6);
    bullets[2]->setAttribute(GameEngine::Attribute::AttributeTypes::reflects);
    bullets[2]->setAttribute(GameEngine::Attribute::AttributeTypes::expands);
    fptrs.push_back(std::bind(&GameEngine::Projectile::CProjectile::shoot, bullets[2], std::placeholders::_1));
    angles.push_back(rand() % MaxAngle);

    auto objKb = std::make_unique<GameEngine::Controller::CKeyboardCtrl<float>>();

    for (int i = 0; i < fptrs.size(); i++)
    {
        objKb->mapKeyToAction(
            sf::Keyboard::Up, fptrs[i], angles[i]);
    }

    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(Width, Height), WindowName);
    sharedPtrWindow->setFramerateLimit(FrameLimit);

    sf::Event event;

    while (sharedPtrWindow->isOpen())
    {
        sharedPtrWindow->clear(sf::Color::Black);
        while (sharedPtrWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                sharedPtrWindow->close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    sharedPtrWindow->close();
                }
                objKb->executeAction(event.key.code);
            }
        }

        for (auto &bullet : bullets)
        {
            if (!bullet->isActive())
            {
                continue;
            }
            for (auto &side : boundry)
            {
                if (!side.isActive())
                {
                    continue;
                }
                if (side.getGlobalBounds().intersects(bullet->getGlobalBounds()))
                {
                    bullet->onCollision(GameEngine::Attribute::AttributeTypes::reflects, side.getDirectionOfPerpendicular());
                }

                sharedPtrWindow->draw(side);
            }
            bullet->runMainLoop();
            sharedPtrWindow->draw(*bullet);
        }

        sharedPtrWindow->display();
    }
}

int main()
{
    srand(time(nullptr));
    menu();
    projectileTesting();
}