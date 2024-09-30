#pragma once

#include "ResourceHolder.h"
#include "ResourceIdentifiers.h"
#include "SFML/Graphics/View.hpp"
#include "Button.h"
#include "Label.h"

namespace sf
{
    class RenderWindow;
    class Event;
}

class TransitionScreen final
{
public:
    explicit TransitionScreen(sf::RenderWindow& window, const FontHolder& font, const char* title, const char* buttonText) noexcept;
    ~TransitionScreen() noexcept = default;

    void draw() const;
    void handleEvent(const sf::Event& event, const std::function<void()>& callback) const;

private:

    void centerTitle();
    void centerButton();

    sf::RenderWindow& mWindow;
    sf::View mWorldView;

    GUI::Button mButton;
    GUI::Label titleLabel {};
};