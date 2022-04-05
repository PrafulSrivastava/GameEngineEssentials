#include <iostream>
#include <memory>
#include <functional>
#include "CMouseCtrl.hpp"
#include "CUtility.hpp"
#include "CEntityWrapper.hpp"

constexpr auto Width = 800;
constexpr auto Height = 600;
constexpr auto Scale = 0.3f;
constexpr auto FrameLimit = 120;
constexpr auto WindowName = "MouseTesting";
constexpr auto TexturePath = "../../assets/graphics/Ghost1_00.png";
std::shared_ptr<GameEngine::Controller::CMouseCtrl> objMse;
int32_t subId;

void menu()
{
    std::cout << "This is Demo to demostrate the following:" << std::endl;
    std::cout << "1) Tracking the mouse pointer." << std::endl;
    std::cout << "2) Locking an object on click." << std::endl;
    std::cout << "3) Making an object chase the mouse pointer" << std::endl;
    std::cout << "Press Middle click to switch between mode (2) and (3)" << std::endl;
    std::cout << "Press Left Click to activate control" << std::endl;
    std::cout << "Press Right Click to deactivate control" << std::endl;
}

void chaseMouse()
{
    auto fptrLeftClick = std::bind(&GameEngine::Controller::CMouseCtrl::chaseMarker, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Left, fptrLeftClick, subId);

    auto fptrRightClick = std::bind(&GameEngine::Controller::CMouseCtrl::stopChasingMarker, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Right, fptrRightClick, subId);
}

void lockEntity()
{
    auto fptrLeftClick = std::bind(&GameEngine::Controller::CMouseCtrl::lockObjectOnClick, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Left, fptrLeftClick, subId);

    auto fptrRightClick = std::bind(&GameEngine::Controller::CMouseCtrl::unlockObjectOnClick, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Right, fptrRightClick, subId);
}

void switchMouseFunctionality(int32_t switchChoice)
{
    objMse->freeUpKey(sf::Mouse::Left);
    objMse->freeUpKey(sf::Mouse::Right);

    (switchChoice > 0) ? chaseMouse() : lockEntity();
}

void mouseFunctionTesting()
{
    objMse = std::make_shared<GameEngine::Controller::CMouseCtrl>();

    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(Width, Height), WindowName);
    GameEngine::CUtility::setWindow(sharedPtrWindow);

    sf::Texture texture;
    texture.loadFromFile(TexturePath);
    texture.setSmooth(true);

    GameEngine::CEntityWrapper<sf::Sprite> sprite;
    sprite.setTexture(texture);
    sprite.setScale(Scale, Scale);
    sprite.setPosition(Width / 2, Height / 2);
    sprite.setOrigin(GameEngine::CUtility::getCentreForShape(sprite.getLocalBounds()));

    int32_t switchChoice = -1;
    auto fptrMiddleClick = std::bind(switchMouseFunctionality, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Middle, fptrMiddleClick, switchChoice);

    subId = objMse->subscribeToMouseCtrl(sprite);
    objMse->lockObjectVisionOnMarker(subId, static_cast<sf::Vector2f>(sf::Mouse::getPosition()));

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
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {

                if (event.mouseButton.button == sf::Mouse::Middle)
                {
                    switchChoice *= -1;
                }
                objMse->executeAction(event.mouseButton.button);
            }
        }
        objMse->setMarkerPos(subId, static_cast<sf::Vector2f>(sf::Mouse::getPosition()));
        objMse->runMainLoop();

        sharedPtrWindow->draw(sprite);
        sharedPtrWindow->display();
    }
}

int main()
{
    menu();
    mouseFunctionTesting();
}