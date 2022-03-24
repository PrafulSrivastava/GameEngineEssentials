#include <iostream>
#include <memory>
#include "CKeyboardCtrl.hpp"
    
using actionCallback = std::function<void()>;

void keyBoardCtrlTest(){

    auto objKb = std::make_unique<GameEngine::CKeyboardCtrl<sf::Keyboard::Key, actionCallback, GameEngine::eKeyBinding::oneToOne>>();
    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
            sf::VideoMode(800, 600), "TestWindow");

    sf::Event event;
    sharedPtrWindow->setFramerateLimit(120);
    objKb->mapKeyToAction(sf::Keyboard::Up, [](){std::cout<<"UP"<<std::endl;});
    objKb->mapKeyToAction(sf::Keyboard::Up, [](){std::cout<<"DOWN"<<std::endl;});
    objKb->mapKeyToAction(sf::Keyboard::Left, [](){std::cout<<"LEFT"<<std::endl;});
    objKb->mapKeyToAction(sf::Keyboard::Right, [](){std::cout<<"RIGHT"<<std::endl;});
    while (sharedPtrWindow->isOpen())
    {
        while (sharedPtrWindow->pollEvent(event))
        {
           if (event.type == sf::Event::Closed){                    
            sharedPtrWindow->close();
            }
            if (event.type == sf::Event::KeyPressed){
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){  
                    objKb->executeAction(sf::Keyboard::Up); 
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
                    objKb->executeAction(sf::Keyboard::Down); 
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
                    objKb->executeAction(sf::Keyboard::Left); 
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
                    objKb->executeAction(sf::Keyboard::Right); 
                } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)){
                    sharedPtrWindow->close();
                }                 
            }    
        }
           
    }
}

int main(int args, char** argsList)
{    
    keyBoardCtrlTest();
    return 0;
}





