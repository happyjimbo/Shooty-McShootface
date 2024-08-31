#include "Game.h"

const sf::Time Game::TimePerFrame = sf::seconds(mSeconds);

Game::Game()
: mWindow(sf::VideoMode(mScreenWidth, mScreenHeight), mTitle, sf::Style::Close)
, mWorld(mWindow)
{
    mWindow.setKeyRepeatEnabled(false);
}

void Game::run()
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;

    mWindow.setFramerateLimit(144);

    while (mWindow.isOpen())
    {
        const sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            processWindowEvents();
            update(TimePerFrame);

            render();
        }
    }
}

void Game::update(const sf::Time elapsedTime)
{
    mWorld.update(elapsedTime);
}

void Game::processWindowEvents()
{
    sf::Event event;
    while(mWindow.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            mWindow.close();
        }
    }
}

void Game::render()
{
    mWindow.clear();

    mWorld.draw();

    mWindow.setView(mWindow.getDefaultView());
    mWindow.display();
}