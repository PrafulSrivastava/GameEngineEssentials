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
    obj.setTexture(texture);
    obj.setScale(0.3f, 0.3f);
    obj.setPosition(200, 200);
    GameEngine::CUtility::setOriginToCenter(obj);

    sharedPtrWindow->setFramerateLimit(120);

    auto objKb = std::make_unique<GameEngine::Controller::CKeyboardCtrl<float>>();
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

    std::vector<std::shared_ptr<GameEngine::Projectile::CProjectile>> bullets;
    std::vector<std::function<void(float)>> fptrs;
    std::vector<float> angles;

    bullets.push_back(std::make_shared<GameEngine::Projectile::CProjectile>(15.f, 4, sf::Color::Cyan, 0, sf::Color::Red));
    bullets[0]->spawn({200, 300}, 2);
    bullets[0]->setAttribute(GameEngine::Attribute::AttributeTypes::reflects);
    bullets[0]->setAttribute(GameEngine::Attribute::AttributeTypes::spins);
    bullets[0]->setAttribute(GameEngine::Attribute::AttributeTypes::fades);
    fptrs.push_back(std::bind(&GameEngine::Projectile::CProjectile::shoot, bullets[0], std::placeholders::_1));
    angles.push_back(rand() % 91);

    bullets.push_back(std::make_shared<GameEngine::Projectile::CProjectile>(20.f, 5, sf::Color::Red, 0, sf::Color::Red));
    bullets[1]->spawn({400, 100}, 4);
    bullets[1]->setAttribute(GameEngine::Attribute::AttributeTypes::reflects);
    bullets[1]->setAttribute(GameEngine::Attribute::AttributeTypes::shrinks);
    fptrs.push_back(std::bind(&GameEngine::Projectile::CProjectile::shoot, bullets[1], std::placeholders::_1));
    angles.push_back(rand() % 91);

    bullets.push_back(std::make_shared<GameEngine::Projectile::CProjectile>(50.f, 3, sf::Color::Green, 0, sf::Color::Red));
    bullets[2]->spawn({400, 100}, 6);
    bullets[2]->setAttribute(GameEngine::Attribute::AttributeTypes::reflects);
    bullets[2]->setAttribute(GameEngine::Attribute::AttributeTypes::expands);
    fptrs.push_back(std::bind(&GameEngine::Projectile::CProjectile::shoot, bullets[2], std::placeholders::_1));
    angles.push_back(rand() % 91);

    auto objKb = std::make_unique<GameEngine::Controller::CKeyboardCtrl<float>>();

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
                if (event.key.code == sf::Keyboard::Escape)
                {
                    sharedPtrWindow->close();
                }
                objKb->executeAction(event.key.code);
            }
        }

        for (auto &bullet : bullets)
        {
            if (!bullet->isActive())
            {
                continue;
            }
            for (auto &side : boundry)
            {
                if (!side.isActive())
                {
                    continue;
                }
                if (side.getGlobalBounds().intersects(bullet->getGlobalBounds()))
                {
                    bullet->onCollision(GameEngine::Attribute::AttributeTypes::reflects, side.getDirectionOfPerpendicular());
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
    auto objMse = std::make_shared<GameEngine::Controller::CMouseCtrl>();

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

    auto fptrLeftClick = std::bind(&GameEngine::Controller::CMouseCtrl::chaseMarker, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Left, fptrLeftClick, subId);

    auto fptrRightClick = std::bind(&GameEngine::Controller::CMouseCtrl::stopChasingMarker, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Right, fptrRightClick, subId);

    objMse->freeUpKey(sf::Mouse::Left);

    auto fptrLeftClick2 = std::bind(&GameEngine::Controller::CMouseCtrl::lockObjectOnClick, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Left, fptrLeftClick2, subId);

    auto fptrMiddleClick = std::bind(&GameEngine::Controller::CMouseCtrl::unlockObjectOnClick, objMse, std::placeholders::_1);
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
    auto objMse = std::make_unique<GameEngine::Controller::CMouseCtrl>();
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

    auto objKb = std::make_unique<GameEngine::Controller::CKeyboardCtrl<uint8_t>>();
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
    objTimer->registerForTimerNotification(2, std::bind(&testing1), 1);
    std::this_thread::sleep_for(std::chrono::seconds(3));
    objTimer->registerForTimerNotification(1, std::bind(&testing2), 0);
    std::this_thread::sleep_for(std::chrono::seconds(6));
    objTimer->registerForTimerNotification(3, std::bind(&testing3), 1);

    std::this_thread::sleep_for(std::chrono::seconds(20));
    objTimer->stopTimer();
}

void testing4()
{
    std::cout << __func__ << std::endl;
}

void TriggerTesting()
{
    using Action = std::function<void()>;

    auto objMse = std::make_shared<GameEngine::Controller::CMouseCtrl>();
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

    auto fptrLeftClick = std::bind(&GameEngine::Controller::CMouseCtrl::chaseMarker, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Left, fptrLeftClick, subId);

    auto fptrRightClick = std::bind(&GameEngine::Controller::CMouseCtrl::stopChasingMarker, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Right, fptrRightClick, subId);

    objMse->freeUpKey(sf::Mouse::Left);

    auto fptrLeftClick2 = std::bind(&GameEngine::Controller::CMouseCtrl::lockObjectOnClick, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Left, fptrLeftClick2, subId);

    auto fptrMiddleClick = std::bind(&GameEngine::Controller::CMouseCtrl::unlockObjectOnClick, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Middle, fptrMiddleClick, subId);

    objTimer->registerForDistanceNotification(300, 45, 90, sprite, std::bind(&testing4), 0);

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
            objTimer->start();

            if (objTimer->isDistanceTriggerRegistered().any())
                sprite.setPosition(++posx, ++posy);

            else
            {
                sprite.setPosition(500, 500);

                objTimer->registerForDistanceNotification(300, 45, 90, sprite, std::bind(&testing4), 0);
            }
        }

        objMse->setMarkerPos(subId, static_cast<sf::Vector2f>(sf::Mouse::getPosition()));
        objMse->runMainLoop();

        sharedPtrWindow->draw(sprite);
        sharedPtrWindow->display();
    }
}

void colorFadeTest()
{
    GameEngine::CEntityWrapper<sf::RectangleShape> rect(0);
    rect.setSize({400.f, 200.f});
    rect.setPosition(400, 300);

    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(800, 600), "RotationTesting");

    GameEngine::CUtility::setWindow(sharedPtrWindow);
    rect.setOrigin(GameEngine::CUtility::getCentreForShape(rect.getLocalBounds()));

    sharedPtrWindow->setFramerateLimit(120);

    auto objKb = std::make_unique<GameEngine::Controller::CKeyboardCtrl<sf::Color>>();
    auto x = 128;
    sf::Color color = sf::Color(255, 255, 255, 128);

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
        sf::Keyboard::G, [&](sf::Color clr)
        {
            color.g--;
            rect.setFillColor(color); },
        color);

    objKb->mapKeyToAction(
        sf::Keyboard::A, [&](sf::Color clr)
        {
            color.a--;
            rect.setFillColor(color); },
        color);

    objKb->mapKeyToAction(
        sf::Keyboard::T, [&](sf::Color clr)
        {
            color.r++;
            rect.setFillColor(color); },
        color);

    objKb->mapKeyToAction(
        sf::Keyboard::N, [&](sf::Color clr)
        {
            color.b++;
            rect.setFillColor(color); },
        color);

    objKb->mapKeyToAction(
        sf::Keyboard::H, [&](sf::Color clr)
        {
            color.g++;
            rect.setFillColor(color); },
        color);

    objKb->mapKeyToAction(
        sf::Keyboard::S, [&](sf::Color clr)
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

int main(int args, char **argsList)
{
    srand(time(nullptr));
    projectileTesting();

    return 0;
}
