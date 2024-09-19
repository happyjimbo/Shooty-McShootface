#pragma once
#include "EntitySystem.h"
#include "ResourceIdentifiers.h"

namespace Aircraft { class AircraftEntity; }
using Aircraft::AircraftEntity;

class SpawnEnemyAircraftSystem final
{
public:
    explicit SpawnEnemyAircraftSystem(
        EntitySystem<AircraftEntity>& entitySystem,
        const TextureHolder& textures,
        float screenWidth
        ) noexcept;

    ~SpawnEnemyAircraftSystem() = default;
    explicit SpawnEnemyAircraftSystem() = delete;

    SpawnEnemyAircraftSystem(const SpawnEnemyAircraftSystem&) = delete;
    SpawnEnemyAircraftSystem& operator=(const SpawnEnemyAircraftSystem&) = delete;

    SpawnEnemyAircraftSystem(SpawnEnemyAircraftSystem&&) = delete;
    SpawnEnemyAircraftSystem& operator=(SpawnEnemyAircraftSystem&&) = delete;

    void execute(const sf::Time& delta);

private:
    float spawnInterval(float delta);
    void spawn(float spawnInterval);

    EntitySystem<AircraftEntity>& mEntitySystem;

    const TextureHolder& mTexture;
    const sf::Vector2f mPosition;

    float mTimeSinceLastSpawn {};
    float mElapsedTime = 0.0f;
};
