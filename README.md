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
 auto kbCtrlPtr = std::make_unique<GameEngine::CKeyboardCtrl<uint8_t>>();
 uint8_t tempId = 0;
 kbCtrlPtr->mapKeyToAction(sf::Keyboard::Up, [](){ std::cout<<"UP is pressed"<<std::endl; &tempId}, );
 kbCtrlPtr->executeAction(sf::Keyboard::Up); 
```
 Console Output: UP is pressed

 ## Mouse Controller:
* Supports one to one mapping of buttons to action.
* Supports many to one mapping of buttons to action.
* Usage: 
```cpp
 using callback = std::function<void()>;
 int32_t tempVal = 0;
 auto mseCtrlPtr = std::make_unique<GameEngine::CMouseCtrl>();
 mseCtrlPtr->mapKeyToAction(sf::Mouse::Left, [](){ std::cout << "Left click is pressed" << std::endl; }, &tempVal);
 mseCtrlPtr->executeAction(sf::Mouse::Left); 
```
 Console Output: Left click is pressed
  ## Projectiles:
* Supports all shapes(all edges will be of same size).
* Supports thickness and border color
* Supports following attributes
  * Reflection
  * Disappearing
  * Has a tail
  * Explosive
  * Fluid
  * Spining
  * Obeying Gravity
  * Enlarging
  * Shrinking
  * Dispersing
  
* Usage: 
```cpp
//Param order {Edge length, Edge count, Color of bullet, Border Width, Color of Border}
auto projectile = std::make_shared<GameEngine::Projectile::CProjectile>(6.f, 4, sf::Color::Cyan, 2, sf::Color::Red));
//Param order {bitset of attributes, Position, Velocity}
//Order of attributes is same as mentioned in description
//1 is on , 0 is off
auto objKb = std::make_unique<GameEngine::Controller::CKeyboardCtrl<float>>();
projectile->spawn({1, 0, 0, 0, 0, 1, 0, 0, 1, 0}, {400, 100}, 5);
auto fptr = (std::bind(&GameEngine::Projectile::CProjectile::shoot, projectile, std::placeholders::_1));
float angleToShootBullet = rand() % 91;
objKb->mapKeyToAction(sf::Keyboard::Up, fptr, angleToShootBullet);
```
 Console Output: A Cyan Bullet of square shape with border size 2 and color Red is shot at "angleToShootBullet" with velocity 5

