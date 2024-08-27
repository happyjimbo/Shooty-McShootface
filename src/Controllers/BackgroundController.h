#pragma once

#include "EntitySystem.h"
#include <Resource/ResourceIdentifiers.h>
#include "SpriteEntity.h"

class BackgroundController final
{
public:
    explicit BackgroundController(
        EntitySystem<SpriteEntity>& entitySystem,
        TextureHolder& texture,
        sf::Vector2u bounds,
        float scrollSpeed
    );
    ~BackgroundController();
    void create();
    void tick(sf::Time delta) const;

private:
    EntitySystem<SpriteEntity>& mEntitySystem;
    std::vector<SpriteEntity*> mSprites {};
    TextureHolder& mTexture;
    sf::IntRect cloudRect;
    const sf::Vector2f mBounds;
    const float mScrollSpeed;


};
