#include <iostream>
#include <memory>
#include <functional>
#include "CKeyboardCtrl.hpp"
#include "CUtility.hpp"
#include "CProjectiles.hpp"

constexpr auto Width = 800;
constexpr auto Height = 600;
constexpr auto Scale = 0.3f;
constexpr auto FrameLimit = 120;
constexpr auto Angle = 10;
constexpr auto WindowName = "ObjectRotationTesting";
constexpr auto TexturePath = "../../assets/graphics/Ghost1_00.png";
GameEngine::CEntityWrapper<sf::Sprite> sprite;

void menu()
{
    std::cout << "Press Up to rotate the Object" << std::endl;
    std::cout << "Press S to switch origin between centre of top-left of an object" << std::endl;
}

void switchOrigin(float rotation)
{
    auto rect = sprite.getLocalBounds();
    (rotation > 0) ? sprite.setOrigin(rect.left, rect.top) : sprite.setOrigin(GameEngine::CUtility::getCentreForShape(rect));
}

void rotationTesting()
{
    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(Width, Height), WindowName);

    GameEngine::CUtility::setWindow(sharedPtrWindow);

    sf::Texture texture;
    texture.loadFromFile(TexturePath);
    texture.setSmooth(true);

    sprite.setTexture(texture);
    sprite.setScale(Scale, Scale);
    sprite.setPosition(Width / 2, Height / 2);

    sharedPtrWindow->setFramerateLimit(FrameLimit);

    auto objKb = std::make_unique<GameEngine::Controller::CKeyboardCtrl<float>>();
    float angle = Angle;
    float rotation = -1;

    objKb->mapKeyToAction(
        sf::Keyboard::Up, [&](float ang)
        { sprite.rotate(ang); },
        angle);

    auto fptr = std::bind(switchOrigin, std::placeholders::_1);
    objKb->mapKeyToAction(sf::Keyboard::S, fptr, rotation);

    while (sharedPtrWindow->isOpen())
    {
        sf::Event event;
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
                if (event.key.code == sf::Keyboard::S)
                {
                    rotation *= -1;
                }
                objKb->executeAction(event.key.code);
            }
        }

        sharedPtrWindow->draw(sprite);
        sharedPtrWindow->display();
    }
}

int main()
{
    srand(time(nullptr));
    menu();
    rotationTesting();
}