#include <iostream>
#include <memory>
#include "CKeyboardCtrl.hpp"
#include "CMouseCtrl.hpp"
#include "CUtility.hpp"


void mouseFunctionTesting()
{
    auto objMse = std::make_unique<GameEngine::CMouseCtrl>();
    
    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(1000, 1000), "MouseFunctionTestWindow");

    sf::Texture texture;
    texture.loadFromFile("assets/graphics/Ghost1_00.png");
    texture.setSmooth(true);

    sf::Sprite sprite;
    sprite.setTexture(texture);
    sprite.setScale(0.3f, 0.3f);
    sprite.setPosition(500, 500);
    GameEngine::CUtility::setOriginToCenter(sprite);
    objMse->lockObjectVisionOnCursor(sprite);
    

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
        }
        objMse->setVisionOnCursor();

        sharedPtrWindow->draw(sprite);
        sharedPtrWindow->display();
    }
}

void mouseCtrlTest()
{
    auto objMse = std::make_unique<GameEngine::CMouseCtrl>();
    objMse->mapKeyToAction(sf::Mouse::Left, []()
                          { std::cout << "Left click" << std::endl; });
    objMse->mapKeyToAction(sf::Mouse::Right, []()
                          { std::cout << "Right click" << std::endl; });
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
            if (event.type == sf::Event::MouseButtonPressed){
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
    objKb->mapKeyToAction(sf::Keyboard::Up, []()
                          { std::cout << "UP" << std::endl; });
    objKb->mapKeyToAction(sf::Keyboard::Up, []()
                          { std::cout << "DOWN" << std::endl; });
    objKb->mapKeyToAction(sf::Keyboard::Left, []()
                          { std::cout << "LEFT" << std::endl; });
    objKb->mapKeyToAction(sf::Keyboard::Right, []()
                          { std::cout << "RIGHT" << std::endl; });
    
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
