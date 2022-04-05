#include <iostream>
#include <memory>
#include <functional>
#include "CKeyboardCtrl.hpp"
#include "CUtility.hpp"
#include "CEntityWrapper.hpp"

constexpr auto Width = 800;
constexpr auto Height = 600;
constexpr auto FrameLimit = 120;
constexpr auto WindowName = "ColorTesting";

void menu()
{
    std::cout << "All colors can be broken down into R,B,G and A components" << std::endl;
    std::cout << "Press T to increase or R to reduce Red component of base Color" << std::endl;
    std::cout << "Press N to increase or B to reduce Blue component of base Color" << std::endl;
    std::cout << "Press H to increase or G to reduce Green component of base Color" << std::endl;
    std::cout << "Press S to increase or A to reduce Alpha/Transparency component of base Color" << std::endl;
}

void colorControl()
{
    GameEngine::CEntityWrapper<sf::RectangleShape> rect(0);
    rect.setSize({Width / 4, Height / 2});
    rect.setPosition(Width / 2, Height / 2);
    rect.setOrigin(GameEngine::CUtility::getCentreForShape(rect.getGlobalBounds()));

    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(Width, Height), WindowName);
    GameEngine::CUtility::setWindow(sharedPtrWindow);
    sharedPtrWindow->setFramerateLimit(FrameLimit);

    auto objKb = std::make_unique<GameEngine::Controller::CKeyboardCtrl<sf::Color>>();
    sf::Color color = sf::Color(255, 255, 255, 255);

    objKb->mapKeyToAction(
        sf::Keyboard::R, [&](sf::Color clr)
        {
            color.r--;
            rect.setFillColor(color); },
        color);

    objKb->mapKeyToAction(
        sf::Keyboard::B, [&](sf::Color clr)
        {
            color.b--;
            rect.setFillColor(color); },
        color);

    objKb->mapKeyToAction(
        sf::Keyboard::G, [&](sf::Color /*clr*/)
        {
            color.g--;
            rect.setFillColor(color); },
        color);

    objKb->mapKeyToAction(
        sf::Keyboard::A, [&](sf::Color /*clr*/)
        {
            color.a--;
            rect.setFillColor(color); },
        color);

    objKb->mapKeyToAction(
        sf::Keyboard::T, [&](sf::Color /*clr*/)
        {
            color.r++;
            rect.setFillColor(color); },
        color);

    objKb->mapKeyToAction(
        sf::Keyboard::N, [&](sf::Color /*clr*/)
        {
            color.b++;
            rect.setFillColor(color); },
        color);

    objKb->mapKeyToAction(
        sf::Keyboard::H, [&](sf::Color /*clr*/)
        {
            color.g++;
            rect.setFillColor(color); },
        color);

    objKb->mapKeyToAction(
        sf::Keyboard::S, [&](sf::Color /*clr*/)
        {
            color.a++;
            rect.setFillColor(color); },
        color);

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
                objKb->executeAction(event.key.code);
            }
        }

        sharedPtrWindow->draw(rect);
        sharedPtrWindow->display();
    }
}

int main()
{
    srand(time(nullptr));
    menu();
    colorControl();
}