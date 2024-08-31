#include "EnemyAircraftController.h"

EnemyAircraftController::EnemyAircraftController (
    EntitySystem<AircraftEntity>& entitySystem,
    const TextureHolder& textures,
    const AircraftEntity::Type type,
    const sf::Vector2f position,
    const sf::FloatRect worldBounds,
    const float scrollSpeed
    ) noexcept
      : mEntitySystem(entitySystem)
      , mTexture(textures)
      , mAircraftType(type)
      , mStartPosition(position)
      , mWorldBounds(worldBounds)
      , mScrollSpeed(scrollSpeed)
{
    mEntitySystem.prePool(20);
}

void EnemyAircraftController::tick(const sf::Time& delta)
{
    mTimeSinceLastSpawn += delta.asSeconds();
    accelerate(mScrollSpeed / mSpeedDivider);

    const float interval = spawnInterval(delta.asSeconds());
    spawn(interval);

    for (const auto& aircraft : mEntitySystem.getEntities())
    {
        aircraft->update(delta);
        aircraft->triggerProjectile(ProjectileEntity::Enemy, mEnemyProjectileSpawnSpeed);
    }
}

float EnemyAircraftController::spawnInterval(const float delta)
{
    mElapsedTime += delta;

    if (mElapsedTime > mTotalDecreaseTime)
    {
        mElapsedTime = mTotalDecreaseTime;
    }

    const float timeRatio = mElapsedTime / mTotalDecreaseTime;
    return mMinSpawnInterval + (mMaxSpawnInterval - mMinSpawnInterval) * (mMaxSpawnInterval - timeRatio);
}

void EnemyAircraftController::spawn(const float spawnInterval)
{
    if (mTimeSinceLastSpawn > spawnInterval)
    {
        mTimeSinceLastSpawn = 0;

        const int x = mAircraftLeftPadding + rand() % static_cast<int>(mStartPosition.x - mAircraftRightPadding);
        const auto startPosition = sf::Vector2f(x, mStartPosition.y);

        auto* aircraft = mEntitySystem.createObject(mAircraftType, mTexture);
        aircraft->setPosition(startPosition);
    }
}

void EnemyAircraftController::accelerate(float const speed) const
{
    for (const auto& aircraft : mEntitySystem.getEntities())
    {
        aircraft->accelerate(0.f, -speed);
    }
}