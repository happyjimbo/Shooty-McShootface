#ifndef CMAKESFMLPROJECT_ENEMY_AIRCRAFT_CONTROLLER_H
#define CMAKESFMLPROJECT_ENEMY_AIRCRAFT_CONTROLLER_H

#include "Aircraft.h"
#include "ResourceIdentifiers.h"

class EntitySystem;

class EnemyAircraftController final
{
    public:
        EnemyAircraftController(
            EntitySystem& entitySystem,
            const TextureHolder& textures,
            Aircraft::Type type,
            sf::Vector2f position,
            sf::FloatRect worldBounds
        );

    public:

        void                                            tick(const sf::Time&, float speed);
        std::vector<std::shared_ptr<Aircraft>>&         getAircrafts();
        void                                            destroy(std::shared_ptr<Aircraft>& aircraft);

    private:
        void                            accelerate(float speed) const;
        void                            spawn();
        void                            checkBounds() ;

    private:
        std::vector<std::shared_ptr<Aircraft>>          mAircrafts;
        EntitySystem&                                   mEntitySystem;
        const TextureHolder&                            mTexture;
        Aircraft::Type                                  mAircraftType;
        float                                           mTimeSinceLastSpawn;
        sf::Vector2f                                    mStartPosition;
        sf::FloatRect                                   mWorldBounds;
};

#endif // CMAKESFMLPROJECT_ENEMY_AIRCRAFT_CONTROLLER_H