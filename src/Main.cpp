#include <iostream>
#include <memory>
#include <functional>
#include "CKeyboardCtrl.hpp"
#include "CMouseCtrl.hpp"
#include "CUtility.hpp"
#include "CSpriteWrapper.hpp"

void mouseFunctionTesting()
{
    auto objMse = std::make_shared<GameEngine::CMouseCtrl>();

    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(1000, 1000), "MouseFunctionTestWindow");

    GameEngine::CUtility::setWindow(sharedPtrWindow);

    sf::Texture texture;
    texture.loadFromFile("assets/graphics/Ghost1_00.png");
    texture.setSmooth(true);

    GameEngine::CSpriteWrapper sprite;
    sprite.setTexture(texture);
    sprite.setScale(0.3f, 0.3f);
    sprite.setPosition(500, 500);
    GameEngine::CUtility::setOriginToCenter(sprite);

    auto subId = objMse->subscribeToMouseCtrl(sprite);
    objMse->lockObjectVisionOnMarker(subId, static_cast<sf::Vector2f>(sf::Mouse::getPosition()));

    auto fptrLeftClick = std::bind(&GameEngine::CMouseCtrl::chaseMarker, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Left, fptrLeftClick, subId);

    auto fptrRightClick = std::bind(&GameEngine::CMouseCtrl::stopChasingMarker, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Right, fptrRightClick, subId);

    objMse->freeUpKey(sf::Mouse::Left);

    auto fptrLeftClick2 = std::bind(&GameEngine::CMouseCtrl::lockObjectOnClick, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Left, fptrLeftClick2, subId);

    auto fptrMiddleClick = std::bind(&GameEngine::CMouseCtrl::unlockObjectOnClick, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Middle, fptrMiddleClick, subId);

    sharedPtrWindow->setFramerateLimit(120);
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
            if (event.type == sf::Event::MouseButtonPressed)
            {
                objMse->executeAction(event.mouseButton.button);
            }
        }
        objMse->setMarkerPos(subId, static_cast<sf::Vector2f>(sf::Mouse::getPosition()));
        objMse->runMainLoop();

        sharedPtrWindow->draw(sprite);
        sharedPtrWindow->display();
    }
}

void mouseCtrlTest()
{
    auto objMse = std::make_unique<GameEngine::CMouseCtrl>();
    objMse->mapKeyToAction(
        sf::Mouse::Left, [](int32_t)
        { std::cout << "Left click" << std::endl; },
        0);
    objMse->mapKeyToAction(
        sf::Mouse::Right, [](int32_t)
        { std::cout << "Right click" << std::endl; },
        0);
    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(800, 600), "MouseTestWindow");

    sf::Event event;
    sharedPtrWindow->setFramerateLimit(120);
    while (sharedPtrWindow->isOpen())
    {
        while (sharedPtrWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                sharedPtrWindow->close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                objMse->executeAction(event.mouseButton.button);
            }
        }
    }
}
void keyBoardCtrlTest()
{

    auto objKb = std::make_unique<GameEngine::CKeyboardCtrl>();
    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(800, 600), "KeyBoardTestWindow");

    sf::Event event;
    sharedPtrWindow->setFramerateLimit(120);
    objKb->mapKeyToAction(
        sf::Keyboard::Up, [](int32_t)
        { std::cout << "UP" << std::endl; },
        0);
    objKb->mapKeyToAction(
        sf::Keyboard::Up, [](int32_t)
        { std::cout << "DOWN" << std::endl; },
        0);
    objKb->mapKeyToAction(
        sf::Keyboard::Left, [](int32_t)
        { std::cout << "LEFT" << std::endl; },
        0);
    objKb->mapKeyToAction(
        sf::Keyboard::Right, [](int32_t)
        { std::cout << "RIGHT" << std::endl; },
        0);

    while (sharedPtrWindow->isOpen())
    {
        while (sharedPtrWindow->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                sharedPtrWindow->close();
            }
            if (event.type == sf::Event::KeyPressed)
            {
                objKb->executeAction(event.key.code);
            }
        }
    }
}

int main(int args, char **argsList)
{
    mouseFunctionTesting();
    return 0;
}
