# GameEngineEssentials
Reusable Game Engine Modules are stored here. 

# Steps to build:

```bash
mkdir build && cd build
cmake ../
make -j8
```

## Keyboard Controller:
* Supports one to one mapping of keys to action.
* Supports many to one mapping of keys to action.
* Usage: 
```cpp
 using callback = std::function<void()>;
 auto kbCtrlPtr = std::make_unique<GameEngine::CKeyboardCtrl<sf::Keyboard::Key, callback, GameEngine::eKeyBinding::oneToOne>>();
 kbCtrlPtr->mapKeyToAction(sf::Keyboard::Up, [](){ std::cout<<"UP is pressed"<<std::endl; });
 kbCtrlPtr->executeAction(sf::Keyboard::Up); 

 Console Output: UP is pressed

