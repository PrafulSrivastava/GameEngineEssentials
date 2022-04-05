#include <iostream>
#include <memory>
#include <functional>
#include "CKeyboardCtrl.hpp"
#include "CUtility.hpp"
#include "CEntityWrapper.hpp"

constexpr auto Width = 800;
constexpr auto Height = 600;
constexpr auto FrameLimit = 120;
constexpr auto WindowName = "KeyBoardTesting";

void menu()
{
    std::cout << "This is Demo to demostrate the keyborad button mapping:" << std::endl;
    std::cout << "Press Up (to show mapping of multiple functions to a single key)" << std::endl;
    std::cout << "Left and Right (to show one to one mapping)" << std::endl;
}

void keyBoardCtrlTest()
{
    auto objKb = std::make_unique<GameEngine::Controller::CKeyboardCtrl<uint8_t>>();
    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(Width, Height), WindowName);

    sf::Event event;
    uint8_t tempVal = 0;
    sharedPtrWindow->setFramerateLimit(FrameLimit);
    objKb->mapKeyToAction(
        sf::Keyboard::Up, [](int32_t)
        { std::cout << "UP" << std::endl; },
        tempVal);
    objKb->mapKeyToAction(
        sf::Keyboard::Up, [](int32_t)
        { std::cout << "DOWN" << std::endl; },
        tempVal);
    objKb->mapKeyToAction(
        sf::Keyboard::Left, [](int32_t)
        { std::cout << "LEFT" << std::endl; },
        tempVal);
    objKb->mapKeyToAction(
        sf::Keyboard::Right, [](int32_t)
        { std::cout << "RIGHT" << std::endl; },
        tempVal);

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

int main()
{
    menu();
    keyBoardCtrlTest();
}