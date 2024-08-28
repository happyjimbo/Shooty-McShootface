#include "ProjectileEntity.h"
#include <Resource/ResourceHolder.h>
#include "../Category.h"
#include "SFML/Graphics/RenderTarget.hpp"
#include "SFML/System/Time.hpp"

void ProjectileEntity::create(const Type type, const TextureHolder &textures)
{
    auto& texture = textures.get(toTextureID(type));
    if (mSprite.getTexture() != &texture)
    {
        mSprite.setTexture(texture);
    }

    mType = type;

    const sf::FloatRect bounds = mSprite.getLocalBounds();
    mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

void ProjectileEntity::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
    states.transform *= getTransform();
    target.draw(mSprite, states);
}

void ProjectileEntity::update(const sf::Time delta)
{
    move(mVelocity * delta.asSeconds());
}

ProjectileEntity::Type ProjectileEntity::getType() const noexcept
{
    return mType;
}

unsigned int ProjectileEntity::getCategory() const noexcept
{
    switch (mType)
    {
        case Player:
            return Category::PlayerProjectile;

        default:
            return Category::EnemyAircraft;
    }
}

Textures::ID ProjectileEntity::toTextureID(const Type type) noexcept
{
    switch (type) {
        case Player:
            return Textures::Bullet;

        // TODO: change this
        case Enemy:
            return Textures::Bullet;
    }

    return Textures::Bullet;
}
