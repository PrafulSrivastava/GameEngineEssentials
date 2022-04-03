#include <iostream>
#include <memory>
#include <functional>
#include "CKeyboardCtrl.hpp"
#include "CMouseCtrl.hpp"
#include "CUtility.hpp"
#include "CEntityWrapper.hpp"
#include "CProjectiles.hpp"
#include "CDistance.hpp"
#include "CTimer.hpp"

void RotationTesting()
{
    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(800, 600), "RotationTesting");

    GameEngine::CUtility::setWindow(sharedPtrWindow);

    sf::Texture texture;
    texture.loadFromFile("assets/graphics/Ghost1_00.png");
    texture.setSmooth(true);

    GameEngine::CEntityWrapper<sf::Sprite> obj;
    // obj.setOrigin(GameEngine::CUtility::getCentreForShape(obj.getGlobalBounds()));

    obj.setTexture(texture);
    obj.setScale(0.3f, 0.3f);
    obj.setPosition(200, 200);
    GameEngine::CUtility::setOriginToCenter(obj);

    // obj.setOrigin(GameEngine::CUtility::getCentreForShape(obj.getGlobalBounds()));

    sharedPtrWindow->setFramerateLimit(120);

    auto objKb = std::make_unique<GameEngine::CKeyboardCtrl<float>>();
    float subId = 10;

    objKb->mapKeyToAction(
        sf::Keyboard::Up, [&](float angle)
        { obj.rotate(angle); },
        subId);

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
                objKb->executeAction(event.key.code);
            }
        }

        sharedPtrWindow->draw(obj);
        sharedPtrWindow->display();
    }
}

void spawnBoundry(std::vector<GameEngine::CEntityWrapper<sf::RectangleShape>> &boundry)
{
    GameEngine::CEntityWrapper<sf::RectangleShape> rect(0);
    rect.setSize({50.f, 600.f});
    rect.setFillColor(sf::Color::White);
    rect.setPosition(0, 0);
    boundry.push_back(rect);

    GameEngine::CEntityWrapper<sf::RectangleShape> rect2(180);
    rect2.setSize({50.f, 600.f});
    rect2.setFillColor(sf::Color::White);
    rect2.setPosition(750, 0);
    boundry.push_back(rect2);

    GameEngine::CEntityWrapper<sf::RectangleShape> rect3(90);
    rect3.setSize({800.f, 50.f});
    rect3.setFillColor(sf::Color::White);
    rect3.setPosition(0, 0);
    boundry.push_back(rect3);

    GameEngine::CEntityWrapper<sf::RectangleShape> rect4(-90);
    rect4.setSize({800.f, 50.f});
    rect4.setFillColor(sf::Color::White);
    rect4.setPosition(0, 550);
    boundry.push_back(rect4);
}

void projectileTesting()
{
    std::cout << "Press UP once to start moving the bullets" << std::endl;
    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(800, 600), "MouseTestWindow");

    std::vector<GameEngine::CEntityWrapper<sf::RectangleShape>> boundry;
    spawnBoundry(boundry);

    std::vector<std::shared_ptr<GameEngine::CProjectile>> bullets;
    std::vector<std::function<void(float)>> fptrs;
    std::vector<float> angles;

    bullets.push_back(std::make_shared<GameEngine::CProjectile>(60.f, 4, sf::Color::Cyan, 0, sf::Color::Red));
    bullets[0]->spawn({1, 0, 0, 0, 0, 1, 0, 0, 1, 0}, {400, 100}, 5);
    fptrs.push_back(std::bind(&GameEngine::CProjectile::shoot, bullets[0], std::placeholders::_1));
    angles.push_back(rand() % 91);

    bullets.push_back(std::make_shared<GameEngine::CProjectile>(10.f, 5, sf::Color::Red, 0, sf::Color::Red));
    bullets[1]->spawn({1, 0, 0, 0, 0, 1, 0, 0, 0, 0}, {400, 100}, 10);
    fptrs.push_back(std::bind(&GameEngine::CProjectile::shoot, bullets[1], std::placeholders::_1));
    angles.push_back(rand() % 91);

    bullets.push_back(std::make_shared<GameEngine::CProjectile>(30.f, 3, sf::Color::Green, 0, sf::Color::Red));
    bullets[2]->spawn({1, 0, 0, 0, 0, 0, 0, 1, 0, 0}, {400, 100}, 2);
    fptrs.push_back(std::bind(&GameEngine::CProjectile::shoot, bullets[2], std::placeholders::_1));
    angles.push_back(rand() % 91);

    auto objKb = std::make_unique<GameEngine::CKeyboardCtrl<float>>();

    for (int i = 0; i < fptrs.size(); i++)
    {
        objKb->mapKeyToAction(
            sf::Keyboard::Up, fptrs[i], angles[i]);
    }

    sf::Event event;
    sharedPtrWindow->setFramerateLimit(120);
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
                objKb->executeAction(event.key.code);
            }
        }

        for (auto &bullet : bullets)
        {
            for (auto &side : boundry)
            {
                if (side.getGlobalBounds().intersects(bullet->getGlobalBounds()))
                {
                    bullet->onCollision(GameEngine::ObstructionTypes::reflective, side.getDirectionOfPerpendicular());
                }

                sharedPtrWindow->draw(side);
            }
            bullet->runMainLoop();
            sharedPtrWindow->draw(*bullet);
        }

        sharedPtrWindow->display();
    }
}
void mouseFunctionTesting()
{
    auto objMse = std::make_shared<GameEngine::CMouseCtrl>();

    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(1000, 1000), "MouseFunctionTestWindow");

    GameEngine::CUtility::setWindow(sharedPtrWindow);

    sf::Texture texture;
    texture.loadFromFile("assets/graphics/Ghost1_00.png");
    texture.setSmooth(true);

    GameEngine::CEntityWrapper<sf::Sprite> sprite;
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
    int32_t tempVal = 0;

    objMse->mapKeyToAction(
        sf::Mouse::Left, [](int32_t)
        { std::cout << "Left click" << std::endl; },
        tempVal);
    objMse->mapKeyToAction(
        sf::Mouse::Right, [](int32_t)
        { std::cout << "Right click" << std::endl; },
        tempVal);
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

    auto objKb = std::make_unique<GameEngine::CKeyboardCtrl<uint8_t>>();
    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(800, 600), "KeyBoardTestWindow");

    sf::Event event;
    uint8_t tempVal = 0;
    sharedPtrWindow->setFramerateLimit(120);
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

void testing1()
{
    std::cout << __func__ << std::endl;
}

void testing2()
{
    std::cout << __func__ << std::endl;
}

void testing3()
{
    std::cout << __func__ << std::endl;
}

void vTimerTesting()
{
    using Action = std::function<void()>;
    auto objTimer = std::make_unique<GameEngine::Trigger::CTimer<std::chrono::seconds, Action>>();

    std::this_thread::sleep_for(std::chrono::seconds(0));
    objTimer->ElapseTime(2, std::bind(&testing1), 1);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    objTimer->ElapseTime(1, std::bind(&testing2), 0);
    std::this_thread::sleep_for(std::chrono::seconds(6));
    objTimer->ElapseTime(3, std::bind(&testing3), 1);

    std::this_thread::sleep_for(std::chrono::seconds(20));
    objTimer->StopTimer();
}

void testing4()
{
    std::cout << __func__ << std::endl;
}

void TriggerTesting()
{
    using Action = std::function<void()>;

    auto objMse = std::make_shared<GameEngine::CMouseCtrl>();
    auto objTimer = std::make_unique<GameEngine::Trigger::CDistance<sf::Sprite, Action>>();

    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(1000, 1000), "TriggerTesting");

    GameEngine::CUtility::setWindow(sharedPtrWindow);

    sf::Texture texture;
    texture.loadFromFile("assets/graphics/Ghost1_00.png");
    texture.setSmooth(true);

    GameEngine::CEntityWrapper<sf::Sprite> sprite;
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

    objTimer->RegisterDistanceNotification(300, 45, 90, sprite, std::bind(&testing4), 0);

    sharedPtrWindow->setFramerateLimit(120);
    while (sharedPtrWindow->isOpen())
    {
        auto posx = sprite.getPosition().x;
        auto posy = sprite.getPosition().y;

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
        if (objTimer != nullptr)
        {
            objTimer->StartMoving();

            if (objTimer->isDistanceTriggerRegistered().any())
                sprite.setPosition(++posx, ++posy);

            else
            {
                sprite.setPosition(500, 500);

                objTimer->RegisterDistanceNotification(300, 45, 90, sprite, std::bind(&testing4), 0);
            }
        }

        objMse->setMarkerPos(subId, static_cast<sf::Vector2f>(sf::Mouse::getPosition()));
        objMse->runMainLoop();

        sharedPtrWindow->draw(sprite);
        sharedPtrWindow->display();
    }
}

int main(int args, char **argsList)
{
    srand(time(nullptr));
    // projectileTesting();
    // mouseFunctionTesting();
    vTimerTesting();
    // TriggerTesting();
    // RotationTesting();

    return 0;
}
