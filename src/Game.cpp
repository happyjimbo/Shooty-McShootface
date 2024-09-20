#include "Game.h"

#include <imgui-SFML.h>

#include "MediaFiles.h"
#include "StateHandler.h"
#include "TransitionScreen.h"
#include "SFML/Graphics.hpp"

namespace
{
    constexpr const auto* sTitle = "Shooty McShootface";
    constexpr int sScreenWidth {640};
    constexpr int sScreenHeight {480};
    constexpr float sSeconds {1.f / 60.f};

    const sf::Time TimePerFrame = sf::seconds(sSeconds);
}

struct Game::Impl
{
    sf::RenderWindow mWindow;
    FontHolder mFont;
    std::unique_ptr<StateHandler> mStateHandler;

    Impl() :
    mWindow(sf::VideoMode(sScreenWidth, sScreenHeight), sTitle, sf::Style::Close)
    {
        mFont.load(Fonts::Main, MediaFiles::Font);
        mStateHandler = std::make_unique<StateHandler>(mWindow, mFont);
        mWindow.setKeyRepeatEnabled(false);
    }

    void update(const sf::Time elapsedTime) const
    {
        mStateHandler->update(elapsedTime);
    }

    void processWindowEvents()
    {
        sf::Event event;
        while(mWindow.pollEvent(event))
        {
            ImGui::SFML::ProcessEvent(event);

            if (event.type == sf::Event::Closed)
            {
                mWindow.close();
            }

            mStateHandler->processWindowEvents(event);
        }
    }

    void render()
    {
        mWindow.clear();

        mStateHandler->draw();

        ImGui::SFML::Render(mWindow);

        mWindow.setView(mWindow.getDefaultView());
        mWindow.display();
    }
};

Game::Game() noexcept : mImpl(std::make_unique<Impl>()) {}
Game::~Game() noexcept = default;

void Game::run() const
{
    sf::Clock clock;
    sf::Time timeSinceLastUpdate = sf::Time::Zero;
    mImpl->mWindow.setFramerateLimit(144);

    const auto success = ImGui::SFML::Init(mImpl->mWindow);

    while (success && mImpl->mWindow.isOpen())
    {
        const sf::Time elapsedTime = clock.restart();
        timeSinceLastUpdate += elapsedTime;

        while (timeSinceLastUpdate > TimePerFrame)
        {
            timeSinceLastUpdate -= TimePerFrame;

            mImpl->processWindowEvents();
            mImpl->update(TimePerFrame);
            mImpl->render();
        }
    }

    ImGui::SFML::Shutdown();
}
