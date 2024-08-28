#pragma once

#include "EntityObject.h"
#include <Resource/ResourceIdentifiers.h>
#include "SFML/Graphics/Sprite.hpp"

class ProjectileEntity final : public EntityObject
{
public:
    enum Type {
        Player,
        Enemy
    };

    ProjectileEntity() = default;
    ProjectileEntity(Type type, const TextureHolder& textures) = delete;

    void create(Type type, const TextureHolder& textures);
    void update(sf::Time delta) override;
    Type getType() const noexcept;
    unsigned int getCategory() const noexcept override;

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    virtual Textures::ID toTextureID(Type type) noexcept;

    sf::Sprite mSprite {};
    Type mType {};
};
