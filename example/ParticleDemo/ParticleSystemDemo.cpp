#include <iostream>
#include <memory>
#include <functional>
#include "CUtility.hpp"
#include "CEntityWrapper.hpp"
#include "CExplosion.hpp"
#include "CKeyboardCtrl.hpp"
#include <type_traits>

constexpr float Width = 800;
constexpr float Height = 800;
constexpr auto Scale = 0.3f;
constexpr auto FrameLimit = 120;
constexpr auto MOVE_OFFSET = 10;
constexpr auto WindowName = "ParticleTesting";
constexpr auto TexturePath = "../../assets/graphics/Ghost1_00.png";

using Action = std::function<void(sf::CircleShape)>;

void menu()
{
    std::cout << "This is Demo to demostrate Particle generation" << std::endl;
    std::cout << "Move the left object and try to collide with right object" << std::endl;
    std::cout << "UP arrow --> To Move UP" << std::endl;
    std::cout << "DOWN arrow --> To Move DOWN" << std::endl;
    std::cout << "RIGHT arrow --> To Move RIGHT" << std::endl;
    std::cout << "LEFT arrow --> To Move LEFT" << std::endl;
}

class Testing
{
public:
    Testing()
    {
        // window
        sharedPtrWindow = std::make_shared<sf::RenderWindow>(sf::VideoMode(Width, Height), WindowName);
        GameEngine::CUtility::setWindow(sharedPtrWindow);
        sharedPtrWindow->setFramerateLimit(120);
        GameEngine::CUtility::setWindow(sharedPtrWindow);

        // Keyboard
        sharedPtrKb = std::make_shared<GameEngine::Controller::CKeyboardCtrl<uint8_t>>();
        uint8_t tempVal = 0;
        sharedPtrWindow->setFramerateLimit(FrameLimit);
        sharedPtrKb->mapKeyToAction(
            sf::Keyboard::Up, std::bind(&Testing::setUp, this),
            tempVal);
        sharedPtrKb->mapKeyToAction(
            sf::Keyboard::Down, std::bind(&Testing::setDown, this),
            tempVal);
        sharedPtrKb->mapKeyToAction(
            sf::Keyboard::Left, std::bind(&Testing::setLeft, this),
            tempVal);
        sharedPtrKb->mapKeyToAction(
            sf::Keyboard::Right, std::bind(&Testing::setRight, this),
            tempVal);

        // Feature
        try
        {
            texture.loadFromFile("assets/graphics/snake.png");
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }

        initPolice();
        initenemy();

        sharedPtrExplosion = std::make_shared<GameEngine::Particle::CExplosion<sf::CircleShape, Action>>(enemy);
        sharedPtrExplosion->initialize(&texture);
        // sharedPtrExplosion->initialize(sf::Color(251, 183, 65));
    }

    void initPolice()
    {
        police.setPosition(200, 300);
        police.setRadius(50);
        police.setFillColor(sf::Color::White);

        sf::FloatRect gb = police.getLocalBounds();
        police.setOrigin(gb.width / 2, gb.height / 2);
    }

    void initenemy()
    {
        enemy.setPosition(500, 300);
        enemy.setRadius(50);
        // enemy.setFillColor(sf::Color::Red);
        enemy.setTexture(&texture);

        sf::FloatRect gb = enemy.getLocalBounds();
        enemy.setOrigin(gb.width / 2, gb.height / 2);
    }

    void setUp()
    {
        police.setPosition(police.getPosition().x, police.getPosition().y - MOVE_OFFSET);
    }

    void setDown()
    {
        police.setPosition(police.getPosition().x, police.getPosition().y + MOVE_OFFSET);
    }
    void setLeft()
    {
        police.setPosition(police.getPosition().x - MOVE_OFFSET, police.getPosition().y);
    }
    void setRight()
    {
        police.setPosition(police.getPosition().x + MOVE_OFFSET, police.getPosition().y);
    }

    void draw(sf::CircleShape shape)
    {
        sharedPtrWindow->draw(shape);
    }

    bool checkCollision()
    {
        return police.getGlobalBounds().intersects(enemy.getGlobalBounds()) && enemy.isActive();
    }

    void run()
    {
        while (sharedPtrWindow->isOpen())
        {
            sf::Event event;
            sharedPtrWindow->clear(sf::Color::Cyan);

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

                    else
                    {
                        sharedPtrKb->executeAction(event.key.code);
                    }
                }
                if (event.type == sf::Event::MouseButtonPressed)
                {
                }
            }

            if (checkCollision())
            {
                flag = true;
            }

            if (flag)
            {
                sharedPtrExplosion->setAttribute(GameEngine::Attribute::AttributeTypes::fades, enemy);
                sharedPtrExplosion->Explode(210, std::bind(&Testing::draw, this, std::placeholders::_1));
            }
            sharedPtrWindow->draw(police);
            sharedPtrWindow->draw(enemy);

            sharedPtrWindow->display();
        }
    }

private:
    std::shared_ptr<sf::RenderWindow> sharedPtrWindow;
    std::shared_ptr<GameEngine::Particle::CExplosion<sf::CircleShape, Action>> sharedPtrExplosion;
    std::shared_ptr<GameEngine::Controller::CKeyboardCtrl<uint8_t>> sharedPtrKb;
    GameEngine::CEntityWrapper<sf::CircleShape> police, enemy;
    bool flag = false;
    sf::Texture texture;
};

int main()
{
    menu();
    Testing test;
    test.run();
}