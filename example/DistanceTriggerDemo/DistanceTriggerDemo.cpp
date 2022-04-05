#include <iostream>
#include <memory>
#include <functional>
#include "CMouseCtrl.hpp"
#include "CUtility.hpp"
#include "CEntityWrapper.hpp"
#include "CTimer.hpp"
#include "CDistance.hpp"

constexpr float Width = 1300;
constexpr float Height = 600;
constexpr auto Scale = 0.3f;
constexpr auto FrameLimit = 120;
constexpr auto Marker1 = 100;
constexpr auto Marker2 = 200;
constexpr auto Marker3 = 300;
constexpr auto TotalPoints = Width / 100;
constexpr auto WindowName = "TriggerTesting";
constexpr auto TexturePath = "../../assets/graphics/Ghost1_00.png";
std::shared_ptr<GameEngine::Controller::CMouseCtrl> objMse;
int32_t subId;

void menu()
{
    std::cout << "This is Demo to demostrate Distance triggers:" << std::endl;
    std::cout << "Your Objective is to Hit every WHITE point with the GREEN POINT" << std::endl;
    std::cout << "Sucessful hit turns WHITE points to BLUE" << std::endl;
    std::cout << "Each point is 100 units apart" << std::endl;
    std::cout << "3 triggers with ditance 100, 200 and 300 are being monitored" << std::endl;
    std::cout << "Press Left Click to make an object chase the mouse pointer" << std::endl;
    std::cout << "Press Right Click to stop" << std::endl;
}

void testing1()
{
    std::cout << "Another " << Marker1 << " Travelled." << std::endl;
}

void testing2()
{
    std::cout << "Another " << Marker2 << " Travelled." << std::endl;
}

void testing3()
{
    std::cout << "Another " << Marker3 << " Travelled." << std::endl;
}

GameEngine::CEntityWrapper<sf::CircleShape> prepMarkPoint(sf::Vector2f pos, sf::Color color = sf::Color::White)
{
    GameEngine::CEntityWrapper<sf::CircleShape> point;
    point.setRadius(10.f);
    point.setFillColor(color);
    point.setPosition(pos.x, pos.y);
    point.setOrigin(GameEngine::CUtility::getCentreForShape(point.getLocalBounds()));

    return point;
}

void distanceTriggerTesting()
{
    using Action = std::function<void()>;

    std::vector<GameEngine::CEntityWrapper<sf::CircleShape>> points;
    for (int i = 0; i < TotalPoints; i++)
    {
        points.push_back(prepMarkPoint({i * Width / TotalPoints, Height / 2}));
    }

    auto objMse = std::make_shared<GameEngine::Controller::CMouseCtrl>();
    auto objTimer = std::make_unique<GameEngine::Trigger::CDistance<sf::Sprite, Action>>();

    auto sharedPtrWindow = std::make_shared<sf::RenderWindow>(
        sf::VideoMode(Width, Height), WindowName);

    GameEngine::CUtility::setWindow(sharedPtrWindow);

    sf::Texture texture;
    texture.loadFromFile(TexturePath);
    texture.setSmooth(true);

    GameEngine::CEntityWrapper<sf::Sprite> sprite;
    sprite.setTexture(texture);
    sprite.setScale(Scale, Scale);
    sprite.setPosition(Width, Height / 2);
    sprite.setOrigin(GameEngine::CUtility::getCentreForShape(sprite.getLocalBounds()));
    auto spriteCenter = prepMarkPoint(sprite.getPosition(), sf::Color::Green);

    auto subId = objMse->subscribeToMouseCtrl(sprite);
    objMse->lockObjectVisionOnMarker(subId, static_cast<sf::Vector2f>(sf::Mouse::getPosition()));

    auto fptrLeftClick = std::bind(&GameEngine::Controller::CMouseCtrl::chaseMarker, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Left, fptrLeftClick, subId);

    auto fptrRightClick = std::bind(&GameEngine::Controller::CMouseCtrl::stopChasingMarker, objMse, std::placeholders::_1);
    objMse->mapKeyToAction(sf::Mouse::Right, fptrRightClick, subId);

    auto timerSubId1 = objTimer->registerForDistanceNotification(Marker1, 180, 90, sprite, std::bind(&testing1), 1);
    auto timerSubId2 = objTimer->registerForDistanceNotification(Marker2, 180, 90, sprite, std::bind(&testing2), 1);
    auto timerSubId3 = objTimer->registerForDistanceNotification(Marker3, 180, 90, sprite, std::bind(&testing3), 1);

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
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                {
                    sharedPtrWindow->close();
                }
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                objMse->executeAction(event.mouseButton.button);
            }
        }
        if (objTimer != nullptr)
        {
            objTimer->monitorDistance();
        }
        for (auto &point : points)
        {
            if (spriteCenter.getGlobalBounds().intersects(point.getGlobalBounds()))
            {
                point.setFillColor(sf::Color::Blue);
            }
        }

        objMse->setMarkerPos(subId, static_cast<sf::Vector2f>(sf::Mouse::getPosition()));
        objMse->runMainLoop();
        spriteCenter.setPosition(sprite.getPosition());

        sharedPtrWindow->draw(sprite);
        sharedPtrWindow->draw(spriteCenter);
        for (const auto &point : points)
        {
            sharedPtrWindow->draw(point);
        }

        sharedPtrWindow->display();
    }
    objTimer->unregisterFromDistanceNotification(timerSubId1);
    objTimer->unregisterFromDistanceNotification(timerSubId2);
    objTimer->unregisterFromDistanceNotification(timerSubId3);
}

int main()
{
    menu();
    distanceTriggerTesting();
}