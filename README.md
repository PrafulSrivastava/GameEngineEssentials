# GameEngineEssentials

Reusable Game Engine Modules are stored here.

# Steps to build:

```bash
mkdir build && cd build
cmake ../
make -j8
```

# Steps to build examples:

```bash
mkdir build && cd build
cmake -DEXAMPLE=ON ../
make -j8
```

> All examples will be created in build/example/

## Keyboard Controller:

- Supports one to one mapping of keys to action.
- Supports many to one mapping of keys to action.
- Usage:

```cpp
 using callback = std::function<void()>;
 auto kbCtrlPtr = std::make_unique<GameEngine::CKeyboardCtrl<uint8_t>>();
 uint8_t tempId = 0;
 kbCtrlPtr->mapKeyToAction(sf::Keyboard::Up, [](){ std::cout<<"UP is pressed"<<std::endl; &tempId}, );
 kbCtrlPtr->executeAction(sf::Keyboard::Up);
```

> Console Output: UP is pressed

## Mouse Controller:

- Supports one to one mapping of buttons to action.
- Supports many to one mapping of buttons to action.
- Usage:

```cpp
 using callback = std::function<void()>;
 int32_t tempVal = 0;
 auto mseCtrlPtr = std::make_unique<GameEngine::CMouseCtrl>();
 mseCtrlPtr->mapKeyToAction(sf::Mouse::Left, [](){ std::cout << "Left click is pressed" << std::endl; }, &tempVal);
 mseCtrlPtr->executeAction(sf::Mouse::Left);
```

> Console Output: Left click is pressed

## Attributes:

- Can be set to any entity derived from CEntityWrapper<Entity>
  - Reflection
  - Disappearing
  - Has a tail
  - Explosive
  - Fluid
  - Spining
  - Obeying Gravity
  - Enlarging
  - Shrinking
  - Multiplies
  - Penetrable
  - Penetrative
  - Dispersing

## Projectiles:

- Derived from CEntityWrapper<Entity>
- Supports all shapes(all edges will be of same size).
- Supports all attributes
- Usage:

```cpp
//Param order {Edge length, Edge count, Color of bullet, Border Width, Color of Border}
auto projectile = std::make_shared<GameEngine::Projectile::CProjectile>(6.f, 4, sf::Color::Cyan, 2, sf::Color::Red));
//Param order {Position, Velocity}
auto objKb = std::make_unique<GameEngine::Controller::CKeyboardCtrl<float>>();
projectile->spawn({400, 100}, 5);
projectile->setAttribute(GameEngine::Attribute::AttributeTypes::reflects);
projectile->setAttribute(GameEngine::Attribute::AttributeTypes::spins);
auto fptr = (std::bind(&GameEngine::Projectile::CProjectile::shoot, projectile, std::placeholders::_1));
float angleToShootBullet = rand() % 91;
objKb->mapKeyToAction(sf::Keyboard::Up, fptr, angleToShootBullet);
```

> Console Output:
> A Bullet
>
> - of Cyan color
> - of square shape
> - with border size 2
> - border color Red
> - shot at "angleToShootBullet"
> - with velocity 5
> - that reflects after collision
> - spins while moving

## Distance Trigger

- Supports Trigger for all Entities (e.g. sprites, shapes).

- Usage:

```cpp
//Object creation
auto objDistance = std::make_unique<GameEngine::Trigger::CDistance<sf::Sprite, Action>>();
// Registration
//Param order {Magnitude , Angle between x and y axis, Angle between z plane and xy plane, Object, callback function(std::function<void>())}
// Notify by calling the function when object reaches at distance of Magnitude which is of thetaxy and thetaz from the position of object when registering
auto distanceSubId = objDistance->registerForDistanceNotification(Marker1, 180, 90, sprite, std::bind(&testing1), 1);
//Monitoring the distance
//call it in a loop
objDistance->monitorDistance();
//unregister when functionality is done
objDistance->unregisterFromDistanceNotification(distanceSubId);
```
