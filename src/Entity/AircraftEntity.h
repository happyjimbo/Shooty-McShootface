#ifndef CMAKESFMLPROJECT_AIRCRAFT_H
#define CMAKESFMLPROJECT_AIRCRAFT_H

#include "EntityObject.h"
#include "ProjectileEntity.h"

class ProjectileController;

class AircraftEntity final : public EntityObject
{

    private:
        enum Position {
            Left,
            Right
        };

    public:
        enum Type {
            Eagle,
            Raptor
        };

    public:
                            AircraftEntity(ProjectileController& projectileController, Type type, const TextureHolder& textures);
    unsigned int	        getCategory() const override;
    void                    update(sf::Time delta) override;
    void                    draw(sf::RenderTarget& target, sf::RenderStates states) const override;
    void                    triggerProjectile(ProjectileEntity::Type type) const;
    void                    hit();

    private:
        Type                        mType;
        sf::Sprite                  mSprite;
        ProjectileController&       mProjectileController;

        static constexpr float mXOffsetAmount = 15.f;
        static constexpr float mYOffsetAmount = 5.f;

        mutable Position mPosition = Left;
        mutable float mTimeSinceLastProjectileSpawn = 0;

};

#endif //CMAKESFMLPROJECT_AIRCRAFT_H